#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "Sphere.h"
#include "Square.h"

#include <GL/glut.h>

enum LightType
{
    LightType_Spherical,
    LightType_Quad
};

struct Light
{
    Vec3 material;
    bool isInCamSpace;
    LightType type;

    Vec3 pos;
    float radius;

    Mesh quad;

    float powerCorrection;

    Light() : powerCorrection(1.0) {}
};

struct RaySceneIntersection
{
    bool intersectionExists;
    //les types : maillage - 0, sphere-1, carre - 2
    unsigned int typeOfIntersectedObject;
    unsigned int objectIndex;
    //si cest float max -> pas d'intersec
    float t;
    RayTriangleIntersection rayMeshIntersection;
    RaySphereIntersection raySphereIntersection;
    RaySquareIntersection raySquareIntersection;
    RaySceneIntersection() : intersectionExists(false), t(FLT_MAX) {}
};

class Scene
{
    std::vector<Mesh> meshes;
    std::vector<Sphere> spheres;
    std::vector<Square> squares;
    std::vector<Light> lights;

public:
    Scene()
    {
    }

    void draw()
    {
        // iterer sur l'ensemble des objets, et faire leur rendu :
        for (unsigned int It = 0; It < meshes.size(); ++It)
        {
            Mesh const &mesh = meshes[It];
            mesh.draw();
        }
        for (unsigned int It = 0; It < spheres.size(); ++It)
        {
            Sphere const &sphere = spheres[It];
            sphere.draw();
        }
        for (unsigned int It = 0; It < squares.size(); ++It)
        {
            Square const &square = squares[It];
            square.draw();
        }
    }

    RaySceneIntersection computeIntersection(Ray const &ray)
    {
        RaySceneIntersection intersection;
        unsigned int  sphereSIze = spheres.size();
        unsigned int squareSize = squares.size();

        for (unsigned int i = 0; i < sphereSIze; i++)
        {
            RaySphereIntersection sphereIntersection = spheres[i].intersect(ray); 

            if (sphereIntersection.intersectionExists)
            {
                if (sphereIntersection.t < intersection.t)
                {
                    intersection.t = sphereIntersection.t;
                    intersection.objectIndex = i;
                    intersection.typeOfIntersectedObject = 1; 
                    intersection.intersectionExists = true;
                    intersection.raySphereIntersection = sphereIntersection;
                }
            }
        }

        for (unsigned int i = 0; i < squareSize; i++)
        {
            RaySquareIntersection squareIntersection = squares[i].intersect(ray); 

            if (squareIntersection.intersectionExists)
            {
                if (squareIntersection.t < intersection.t)
                {
                    intersection.t = squareIntersection.t;
                    intersection.objectIndex = i;
                    intersection.typeOfIntersectedObject = 2;
                    intersection.intersectionExists = true;
                    intersection.raySquareIntersection = squareIntersection;
                }
            }
        }

        return intersection;
    }

    float searchFirstIntersection(Ray const &ray)
    {

        unsigned int  sphereSIze = spheres.size();
        unsigned int squareSize = squares.size();
               
        for (unsigned i = 0; i < sphereSIze; i++)
        {
            RaySphereIntersection raySphere = spheres[i].intersect(ray);
            if (raySphere.intersectionExists)
                return raySphere.t;
        }
        for (unsigned i = 0; i < squareSize; i++)
        {
            RaySquareIntersection raySquare = squares[i].intersect(ray);
            if (raySquare.intersectionExists)
                return raySquare.t;
        }
        return 0;
    }

    float calculateCoef(int i, int echant, Vec3 intersection, float minimiseBy)
    {
        int nb_shadows = 0;
        float radius = lights[i].radius;
        float diametre = 2 * lights[i].radius/2 ,//* minimiseBy,
              x0 = lights[i].pos[0] - radius/2 ,
              y0 = lights[i].pos[1] - radius/2,
              z0 = lights[i].pos[2] - radius/2 ;
        Vec3 L;
        Ray shadowRay;

        //float pas = diametre / (echant - 1);
        float pasX, pasZ;
        for (int x = 0; x < echant; x++)
        {
            //for (int z = 0; z < echant; z++)
            //{
                pasX = (float) (rand() / (float)(RAND_MAX/(diametre)));
                pasZ = (float) (rand() / (float)(RAND_MAX/(diametre)));
                L = Vec3(x0 +  pasX, y0 , z0 + pasZ) - intersection;

                L.normalize();
                shadowRay = Ray(intersection, L);
                float ombre = searchFirstIntersection(shadowRay);
                if (ombre < 1 && ombre > 0.00001) nb_shadows++;

            //}
        }
        return (float)nb_shadows / echant ;//(echant * echant);
    }

    Vec3 rayTraceRecursive(Ray ray, int NRemainingBounces)
    {

        RaySceneIntersection raySceneIntersection = computeIntersection(ray);
        Ray shadowRay;
        Vec3 color;     
        int echant = 10;
        float minimiseBy = .5;
        //color par default - fon d'ecran - ex vec3(0.,0,.0.)
        color = Vec3(0., 0., 0.);
        if (raySceneIntersection.intersectionExists)
        {
            switch (raySceneIntersection.typeOfIntersectedObject)
            {
            case 0:
                color = meshes[raySceneIntersection.objectIndex].material.diffuse_material;
                break;
            case 1:
            {
                unsigned int index = raySceneIntersection.objectIndex;
                float Id, Is, Ia, LN, RV,
                    shininess = spheres[index].material.shininess;
                Vec3 normal = raySceneIntersection.raySphereIntersection.normal,
                     intersection = raySceneIntersection.raySphereIntersection.intersection;

                for (unsigned int i = 0; i < lights.size(); i++)
                {

                    Vec3 L;
                    L = lights[i].pos - intersection;
                    L.normalize();

                    float coef = calculateCoef(i, echant, intersection, minimiseBy);

                    if (coef != 1.)
                    {

                        for (unsigned int k = 0; k < 3; k++)
                        {
                            //𝐼𝑎 = 𝐼𝑠𝑎 * 𝐾𝑎
                            Ia = lights[i].material[k] * spheres[index].material.ambient_material[k];
                            //𝐼𝑑 = 𝐼𝑠𝑑 ∗ 𝐾𝑑 ∗ (𝐿. 𝑁)
                            LN = std::max((float)Vec3::dot(L, normal),0.f);
                            Id = lights[i].material[k] * spheres[index].material.diffuse_material[k] * LN;
                            //𝐼𝑠 = 𝐼𝑠𝑠 ∗ 𝐾𝑠 ∗ (𝑅. 𝑉)^𝑛
                            //𝑅 = 2 (𝑁. 𝐿) * 𝑁 − L
                            Vec3 R = 2 * LN * normal - L;
                            R.normalize();
                            Vec3 V = ray.origin() - intersection;
                            V.normalize();
                            RV = std::max((float)Vec3::dot(R, V), 0.f);
                            Is = lights[i].material[k] * spheres[index].material.specular_material[k] * pow(RV, shininess);
                            
                            color[k] += Ia + Is + Id;
                            color[k] *= 1 - coef;
                            
                            //if (coef != 0.) color[k] *= coef;
                            if (color[k] < 0.001)
                            {
                                color[k] = 0;
                            }
                            
                        }
                    }
                }
            }
            break;
            case 2:
            {
                unsigned int index = raySceneIntersection.objectIndex;
                float Id, Is, Ia, LN, RV,
                    shininess = squares[index].material.shininess;
                Vec3 normal = raySceneIntersection.raySquareIntersection.normal,
                     intersection = raySceneIntersection.raySquareIntersection.intersection;
                for (unsigned int i = 0; i < lights.size(); i++)
                {

                    Vec3 L;
                    L = lights[i].pos - intersection;
                    L.normalize();

                    float coef = calculateCoef(i, echant, intersection, minimiseBy);
                    if (coef != 1.)
                    {

                        for (unsigned int k = 0; k < 3; k++)
                        {

                            //𝐼𝑎 = 𝐼𝑠𝑎 * 𝐾𝑎
                            Ia = lights[i].material[k] * squares[index].material.ambient_material[k];
                            //𝐼𝑑 = 𝐼𝑠𝑑 ∗ 𝐾𝑑 ∗ (𝐿. 𝑁)

                            LN = std::max((float)Vec3::dot(L, normal),0.f);
                            Id = lights[i].material[k] * squares[index].material.diffuse_material[k] * LN;
                            //𝐼𝑠 = 𝐼𝑠𝑠 ∗ 𝐾𝑠 ∗ (𝑅. 𝑉)^𝑛
                            //𝑅 = 2 (𝑁. 𝐿) * 𝑁 − L
                            Vec3 R = 2 * LN * normal - L;
                            R.normalize();
                            Vec3 V = ray.origin() - intersection;
                            V.normalize();
                            RV = std::max((float)Vec3::dot(R, V), 0.f);
                            Is = lights[i].material[k] * squares[index].material.specular_material[k] * pow(RV, shininess);

                            color[k] += Ia + Is + Id;

                            color[k] *= 1 - coef;
                            if (color[k] < 0.001)
                            {
                                color[k] = 0;
                            }
                        }
                    }
                }
            }
            break;
            default:
                break;
            }
        }

        return color;
    }

    Vec3 rayTrace(Ray const &rayStart)
    {
        
        Vec3 color;
        color = rayTraceRecursive(rayStart, 1);
        return color;
    }

    void setup_single_sphere()
    {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize(lights.size() + 1);
            Light &light = lights[lights.size() - 1];
            light.pos = Vec3(-5, 5, 5);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1, 1, 1);
            light.isInCamSpace = false;
        }
        {
            spheres.resize(spheres.size() + 1);
            Sphere &s = spheres[spheres.size() - 1];
            s.m_center = Vec3(1., 0., 0.);
            s.m_radius = 1.f;
            s.build_arrays();
            s.material.type = Material_Mirror;
            s.material.diffuse_material = Vec3(1., 0., 0.);
            s.material.specular_material = Vec3(0.2, 0.2, 0.2);
            s.material.shininess = 20;
        }
        {
            spheres.resize(spheres.size() + 1);
            Sphere &s = spheres[spheres.size() - 1];
            s.m_center = Vec3(0., 0., 0.);
            s.m_radius = 1.f;
            s.build_arrays();
            s.material.type = Material_Mirror;
            s.material.diffuse_material = Vec3(0., 1., 0.);
            s.material.specular_material = Vec3(0.2, 0.2, 0.2);
            s.material.shininess = 20;
        }
    }

    void setup_single_square()
    {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize(lights.size() + 1);
            Light &light = lights[lights.size() - 1];
            light.pos = Vec3(-5, 5, 5);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1, 1, 1);
            light.isInCamSpace = false;
        }

        {
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.build_arrays();
            s.material.diffuse_material = Vec3(1., 0., 0.); //Vec3( 0.8,0.8,0.8 );
            s.material.specular_material = Vec3(0.8, 0.8, 0.8);
            s.material.shininess = 20;
        }
    }

    void setup_cornell_box()
    {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize(lights.size() + 1);
            Light &light = lights[lights.size() - 1];
            light.pos = Vec3(0.0, 1.5, 0.0);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1, 1, 1);
            light.isInCamSpace = false;
        }

        { //Back Wall
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.scale(Vec3(2., 2., 1.));
            s.translate(Vec3(0., 0., -2.));
            s.build_arrays();
            s.material.diffuse_material = Vec3(1., 1., 1.);
            s.material.specular_material = Vec3(1., 1., 1.);
            s.material.shininess = 16;
        }

        { //Left Wall

            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.scale(Vec3(2., 2., 1.));
            s.translate(Vec3(0., 0., -2.));
            s.rotate_y(90);
            s.build_arrays();
            s.material.diffuse_material = Vec3(1., 0., 0.);
            s.material.specular_material = Vec3(1., 0., 0.);
            s.material.shininess = 16;
        }

        { //Right Wall
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_y(-90);
            s.build_arrays();
            s.material.diffuse_material = Vec3(0.0, 1.0, 0.0);
            s.material.specular_material = Vec3(0.0, 1.0, 0.0);
            s.material.shininess = 16;
        }

        { //Floor
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_x(-90);
            s.build_arrays();
            s.material.diffuse_material = Vec3(1.0, 1.0, 1.0);
            s.material.specular_material = Vec3(1.0, 1.0, 1.0);
            s.material.shininess = 16;
        }

        { //Ceiling
            squares.resize(squares.size() + 1);
            Square &s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_x(90);
            s.build_arrays();
            s.material.diffuse_material = Vec3(1.0, 1.0, 1.0);
            s.material.specular_material = Vec3(1.0, 1.0, 1.0);
            s.material.shininess = 16;
        }

        /*{ //Front Wall
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_y(180);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 1.0,1.0,1.0 );
            s.material.specular_material = Vec3( 1.0,1.0,1.0 );
            s.material.shininess = 16;
        }*/

        { //GLASS Sphere

            spheres.resize(spheres.size() + 1);
            Sphere &s = spheres[spheres.size() - 1];
            s.m_center = Vec3(1.0, -1.25, 0.5);
            s.m_radius = 0.75f;
            s.build_arrays();
            s.material.type = Material_Mirror;
            s.material.diffuse_material = Vec3(1., 0., 0.);
            s.material.specular_material = Vec3(1., 0., 0.);
            s.material.shininess = 16;
            s.material.transparency = 1.0;
            s.material.index_medium = 1.4;
        }

        { //MIRRORED Sphere
            spheres.resize(spheres.size() + 1);
            Sphere &s = spheres[spheres.size() - 1];
            s.m_center = Vec3(-1.0, -1.25, -0.5);
            s.m_radius = 0.75f;
            s.build_arrays();
            s.material.type = Material_Glass;
            s.material.diffuse_material = Vec3(1., 1., 1.);
            s.material.specular_material = Vec3(1., 1., 1.);
            s.material.shininess = 16;
            s.material.transparency = 0.;
            s.material.index_medium = 0.;
        }
    }
};

#endif
