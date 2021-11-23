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
        RaySceneIntersection result;
        //TODO calculer les intersections avec les objets de la scene et garder la plus proche
        //parcourir tous les objets de la scene, garder celui dont le t est le plus petit

        int sphSize = spheres.size();
        int sqSize = squares.size();

        for (int i = 0; i < sphSize; i++)
        {
            RaySphereIntersection sInter = spheres[i].intersect(ray); //intersection avec single sphere

            if (sInter.intersectionExists)
            {
                if (sInter.t < result.t)
                {
                    result.t = sInter.t;
                    result.objectIndex = i;
                    result.typeOfIntersectedObject = 1; // intersection avec single sphere
                    result.intersectionExists = true;
                    result.raySphereIntersection = sInter;
                }
            }
        }

        for (int i = 0; i < sqSize; i++)
        {
            // std::cout<<"dans le carré "<<i<<std::endl;
            RaySquareIntersection sqInter = squares[i].intersect(ray); //intersection avec single square

            if (sqInter.intersectionExists)
            {
                // std::cout<<"inter square"<<std::endl;
                if (sqInter.t < result.t)
                {
                    result.t = sqInter.t;
                    result.objectIndex = i;
                    result.typeOfIntersectedObject = 2; //intersection avec single square
                    result.intersectionExists = true;
                    result.raySquareIntersection = sqInter;
                }
            }
        }

        return result;
    }

    float intersectSmth(Ray const &ray)
    {

        //Meshes
        for (unsigned int i = 0; i < meshes.size(); i++)
        {
            RayTriangleIntersection rayMesh = meshes[i].intersect(ray);
            if (rayMesh.intersectionExists)
                return rayMesh.t;
        }

        // Spheres
        for (unsigned int i = 0; i < spheres.size(); i++)
        {
            RaySphereIntersection raySphere = spheres[i].intersect(ray);
            if (raySphere.intersectionExists)
                return raySphere.t;
        }
        // Squares
        for (unsigned int i = 0; i < squares.size(); i++)
        {
            RaySquareIntersection raySquare = squares[i].intersect(ray);
            if (raySquare.intersectionExists)
                return raySquare.t;
        }
        return 0;
    }

    Vec3 rayTraceRecursive(Ray ray, int NRemainingBounces)
    {

        //TODO RaySceneIntersection raySceneIntersection = computeIntersection(ray);
        RaySceneIntersection raySceneIntersection = computeIntersection(ray);
        Ray shadowRay;
        Vec3 color;
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

                for (unsigned int k = 0; k < 3; k++)
                {
                    for (unsigned int i = 0; i < lights.size(); i++)
                    {

                        Vec3 L;

                        /*                         int echant = 4, nb_shadows = 0;
                        float diametre = 2 * lights[i].radius,
                            x0 = lights[i].pos[0] - lights[i].radius,
                            y0 = lights[i].pos[1] - lights[i].radius,
                            z0 = lights[i].pos[3] - lights[i].radius;

                       
                        float pas = (lights[i].pos[0] - lights[i].radius)/echant;
                        Vec3 quad_lighths[echant*echant] ;

                        for (int x = 0; x < echant; x++){
                            for (int y = 0; y < echant; y++){
                                 L = Vec3(x0 + pas*x, y0 + pas*y, z0) - intersection;
                                L.normalize();
                                shadowRay = Ray(intersection, L);
                                float ombre = intersectSmth(shadowRay);
                                if (ombre < 1 && ombre > 0.0001){ nb_shadows ++; }
                            }
                        }
                        //std::cout << "nb_shadows : " <<nb_shadows << std::endl;  
                        float coef = (float) nb_shadows/(echant*echant); */
                        //if (nb_shadows > 0) std::cout << "coef : " << coef << std::endl;
                        L = lights[i].pos - intersection;
                        L.normalize();
                        shadowRay = Ray(intersection, L);
                        float ombre = intersectSmth(shadowRay);
                        if (!(ombre < 1 && ombre > 0.0001))
                        {
                            //𝐼𝑎 = 𝐼𝑠𝑎 * 𝐾𝑎
                            Ia = lights[i].material[k] * spheres[index].material.ambient_material[k];
                            //𝐼𝑑 = 𝐼𝑠𝑑 ∗ 𝐾𝑑 ∗ (𝐿. 𝑁)
                            LN = Vec3::dot(L, normal);
                            Id = lights[i].material[k] * spheres[index].material.diffuse_material[k] * LN;
                            //𝐼𝑠 = 𝐼𝑠𝑠 ∗ 𝐾𝑠 ∗ (𝑅. 𝑉)^𝑛
                            //𝑅 = 2 (𝑁. 𝐿) * 𝑁 − L
                            Vec3 R = 2 * LN * normal - L;
                            R.normalize();
                            Vec3 V = ray.origin() - intersection;
                            V.normalize();
                            RV = Vec3::dot(R, V);
                            Is = lights[i].material[k] * spheres[index].material.specular_material[k] * pow(RV, shininess);

                            color[k] += Ia + Is + Id;
                            //if (coef != 0.) color[k] *= coef;
                            if (color[k] < 0.000001)
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

                    int echant = 3, nb_shadows = 0;
                    float diametre = 2 * lights[i].radius,
                          x0 = lights[i].pos[0] - lights[i].radius * 0.2,
                          y0 = lights[i].pos[1], //lights[i].pos[1] - lights[i].radius*0.1,
                          z0 = lights[i].pos[2] - lights[i].radius * 0.2;

                    float pas = (diametre * 0.2) / (echant-1); /*, 
                              pasX = ((lights[i].pos[0] - lights[i].radius*0.1)/echant),
                              pasY = ((lights[i].pos[1] - lights[i].radius*0.1)/echant),
                              pasZ = ((lights[i].pos[2] - lights[i].radius*0.1)/echant)  
                        std::cout << "pas" << pas <<" diametre" << diametre << std::endl; 
                        std::cout << "lights[i].pos" << lights[i].pos << std::endl; */
                    for (int x = 0; x < echant ; x++)
                    {
                        //for (int y = 0; y < echant; y++){
                        for (int z = 0; z < echant ; z++)
                        {
                            // std::cout << "      Vec3(x0 + pas*x, y0, z0 + pas*z)" << Vec3(x0 + pas*x, y0, z0 + pas*z) << std::endl;
                            L = Vec3(x0 + pas * x, y0, z0 + pas * z) - intersection;
                            L.normalize();
                            shadowRay = Ray(intersection, L);
                            float ombre = intersectSmth(shadowRay);
                            if (ombre < 1 && ombre > 0.0001)
                            {
                                nb_shadows++;
                            }
                        }
                        //}
                    }
                    //std::cout << "nb_shadows : " <<nb_shadows << std::endl;
                    float coef = (float)nb_shadows / (echant * echant);
                    //if (coef != 0.) std::cout << "coef : " <<coef << std::endl;

                    /* L = lights[i].pos - intersection;
                        L.normalize();
                        shadowRay = Ray(intersection, L);
                        float ombre = intersectSmth(shadowRay);
                        if (!((ombre < 1 && ombre > 0.0001))){ */
                    if (coef != 1.)
                    {
                        
                        for (unsigned int k = 0; k < 3; k++)
                        {

                            //𝐼𝑎 = 𝐼𝑠𝑎 * 𝐾𝑎
                            Ia = lights[i].material[k] * squares[index].material.ambient_material[k];
                            //𝐼𝑑 = 𝐼𝑠𝑑 ∗ 𝐾𝑑 ∗ (𝐿. 𝑁)

                            LN = Vec3::dot(L, normal);
                            Id = lights[i].material[k] * squares[index].material.diffuse_material[k] * LN;
                            //𝐼𝑠 = 𝐼𝑠𝑠 ∗ 𝐾𝑠 ∗ (𝑅. 𝑉)^𝑛
                            //𝑅 = 2 (𝑁. 𝐿) * 𝑁 − L
                            Vec3 R = 2 * LN * normal - L;
                            R.normalize();
                            Vec3 V = ray.origin() - intersection;
                            V.normalize();
                            RV = Vec3::dot(R, V);
                            Is = lights[i].material[k] * squares[index].material.specular_material[k] * pow(RV, shininess);

                            color[k] += Ia + Is + Id;

                            color[k] *= 1 - coef*0.5;
                            if (color[k] < 0.000001)
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
        //TODO appeler la fonction recursive
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
