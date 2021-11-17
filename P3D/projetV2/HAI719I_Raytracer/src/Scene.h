#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <string>
#include "Mesh.h"
#include "Sphere.h"
#include "Square.h"


#include <GL/glut.h>


enum LightType {
    LightType_Spherical,
    LightType_Quad
};


struct Light {
    Vec3 material;
    bool isInCamSpace;
    LightType type;

    Vec3 pos;
    float radius;

    Mesh quad;

    float powerCorrection;

    Light() : powerCorrection(1.0) {}

};

struct RaySceneIntersection{
    bool intersectionExists;
    //les types : maillage - 0, sphere-1, carre - 2 
    unsigned int typeOfIntersectedObject;
    unsigned int objectIndex;
    //si cest float max -> pas d'intersec
    float t;
    RayTriangleIntersection rayMeshIntersection;
    RaySphereIntersection raySphereIntersection;
    RaySquareIntersection raySquareIntersection;
    RaySceneIntersection() : intersectionExists(false) , t(FLT_MAX) {}
};



class Scene {
    std::vector< Mesh > meshes;
    std::vector< Sphere > spheres;
    std::vector< Square > squares;
    std::vector< Light > lights;

public:


    Scene() {
    }

    void draw() {
        // iterer sur l'ensemble des objets, et faire leur rendu :
        for( unsigned int It = 0 ; It < meshes.size() ; ++It ) {
            Mesh const & mesh = meshes[It];
            mesh.draw();
        }
        for( unsigned int It = 0 ; It < spheres.size() ; ++It ) {
            Sphere const & sphere = spheres[It];
            sphere.draw();
        }
        for( unsigned int It = 0 ; It < squares.size() ; ++It ) {
            Square const & square = squares[It];
            square.draw();
        }
    }




    RaySceneIntersection computeIntersection(Ray const & ray) {
        RaySceneIntersection result;
        //RayTriangleIntersection curr_mesh,tmp_mesh =  new ;//mesh_size <0 ? meshes[0].intersect(ray) : null;
        RaySphereIntersection curr_sphere,tmp_sphere;// = new RaySphereIntersection();//sphere_size <0 ? spheres[0].intersect(ray) : null;
        RaySquareIntersection curr_square,tmp_square;// = new RaySquareIntersection(); //square_size <0 ? squares[0].intersect(ray) : null;
        float t_min = FLT_MAX; 
        int objectIndex = 0; //val par defaut
        //TODO calculer les intersections avec les objets de la scene et garder la plus proche
        /* for (unsigned int mesh = 0; mesh < meshes.size(); mesh++){
            curr_mesh = meshes[mesh].intersect(ray);
            if (curr_mesh.t < t_min.t)
                tmp_mesh = curr_mesh;
                objectIndex = mesh;
                t_min = curr_mesh.t;
        } */
        for (unsigned int sphere = 0; sphere < spheres.size(); sphere++){
            curr_sphere = spheres[sphere].intersect(ray);
            if (curr_sphere.t < t_min){
                tmp_sphere = curr_sphere;
                objectIndex = sphere;
                t_min = curr_sphere.t;
            }
        }
        for (unsigned int square = 0; square < squares.size(); square++){   
            curr_square = squares[square].intersect(ray);
            if (curr_square.t < t_min){
                tmp_square = curr_square; 
                objectIndex = square;
                t_min =  curr_square.t;  
            }  
        }
        //t_min represente l'intersection le plus proche
        //idee - je compare les intersections une apres l'autre donc
        //si tmp_square est toujours null cad que les intersections sont plus loin
        //idem pour tmp_sphere
        //si tmp_mesh est aussi null cad qu'il y a pas d'intersections (aussi si t_min == FLT_MAX)
        
        
        if (! tmp_square.intersectionExists ){
            result.intersectionExists = true;
            result.objectIndex = objectIndex;
            result.typeOfIntersectedObject = 2;
            result.raySquareIntersection = squares[objectIndex].intersect(ray);
            result.t = t_min;
        }else if (! tmp_sphere.intersectionExists){
            result.intersectionExists = true;
            result.objectIndex = objectIndex;
            result.typeOfIntersectedObject = 1;
            result.raySphereIntersection = spheres[objectIndex].intersect(ray);
            result.t = t_min;
        }/* else  if (tmp_mesh != NULL){
            result.intersectionExists = true;
            result.objectIndex = objectIndex;
            result.typeOfIntersectedObject = 0;
            result.rayMeshIntersection = meshes[objectIndex].intersect(ray);
            result.t = t_min;
        } *///sinon result a deja ses valeurs par default

        return result;
    }





    Vec3 rayTraceRecursive( Ray ray , int NRemainingBounces ) {

        //TODO RaySceneIntersection raySceneIntersection = computeIntersection(ray);
        RaySceneIntersection raySceneIntersection = computeIntersection(ray);
        Vec3 color;
        //color par default - fon d'ecran - ex vec3(0.,0,.0.)
        color = Vec3(0.,0.,0.);
        if (raySceneIntersection.intersectionExists){
            switch (raySceneIntersection.typeOfIntersectedObject){
                case 0:
                    color = meshes[raySceneIntersection.objectIndex].material.diffuse_material;
                    break;
                case 1:
                    color = spheres[raySceneIntersection.objectIndex].material.diffuse_material;
                    break;
                case 2:
                    color = squares[raySceneIntersection.objectIndex].material.diffuse_material;
                    break;
                default:
                    break;
            }
        }
        
        return color;
    }


    Vec3 rayTrace( Ray const & rayStart ) {
        //TODO appeler la fonction recursive
        Vec3 color;
        color = rayTraceRecursive(rayStart,1);
        return color;
    }

    void setup_single_sphere() {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize( lights.size() + 1 );
            Light & light = lights[lights.size() - 1];
            light.pos = Vec3(-5,5,5);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1,1,1);
            light.isInCamSpace = false;
        }
        {
            spheres.resize( spheres.size() + 1 );
            Sphere & s = spheres[spheres.size() - 1];
            s.m_center = Vec3(0. , 0. , 0.);
            s.m_radius = 1.f;
            s.build_arrays();
            s.material.type = Material_Mirror;
            s.material.diffuse_material = Vec3( 1.,1.,1 );
            s.material.specular_material = Vec3( 0.2,0.2,0.2 );
            s.material.shininess = 20;
        }
    }

    void setup_single_square() {
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize( lights.size() + 1 );
            Light & light = lights[lights.size() - 1];
            light.pos = Vec3(-5,5,5);
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1,1,1);
            light.isInCamSpace = false;
        }

        {
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 0.8,0.8,0.8 );
            s.material.specular_material = Vec3( 0.8,0.8,0.8 );
            s.material.shininess = 20;
        }
    }

    void setup_cornell_box(){
        meshes.clear();
        spheres.clear();
        squares.clear();
        lights.clear();

        {
            lights.resize( lights.size() + 1 );
            Light & light = lights[lights.size() - 1];
            light.pos = Vec3( 0.0, 1.5, 0.0 );
            light.radius = 2.5f;
            light.powerCorrection = 2.f;
            light.type = LightType_Spherical;
            light.material = Vec3(1,1,1);
            light.isInCamSpace = false;
        }

        { //Back Wall
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.scale(Vec3(2., 2., 1.));
            s.translate(Vec3(0., 0., -2.));
            s.build_arrays();
            s.material.diffuse_material = Vec3( 1.,1.,1. );
            s.material.specular_material = Vec3( 1.,1.,1. );
            s.material.shininess = 16;
        }

        { //Left Wall

            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.scale(Vec3(2., 2., 1.));
            s.translate(Vec3(0., 0., -2.));
            s.rotate_y(90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 1.,0.,0. );
            s.material.specular_material = Vec3( 1.,0.,0. );
            s.material.shininess = 16;
        }

        { //Right Wall
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_y(-90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 0.0,1.0,0.0 );
            s.material.specular_material = Vec3( 0.0,1.0,0.0 );
            s.material.shininess = 16;
        }

        { //Floor
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_x(-90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 1.0,1.0,1.0 );
            s.material.specular_material = Vec3( 1.0,1.0,1.0 );
            s.material.shininess = 16;
        }

        { //Ceiling
            squares.resize( squares.size() + 1 );
            Square & s = squares[squares.size() - 1];
            s.setQuad(Vec3(-1., -1., 0.), Vec3(1., 0, 0.), Vec3(0., 1, 0.), 2., 2.);
            s.translate(Vec3(0., 0., -2.));
            s.scale(Vec3(2., 2., 1.));
            s.rotate_x(90);
            s.build_arrays();
            s.material.diffuse_material = Vec3( 1.0,1.0,1.0 );
            s.material.specular_material = Vec3( 1.0,1.0,1.0 );
            s.material.shininess = 16;
        }

        { //Front Wall
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
        }


        { //GLASS Sphere

            spheres.resize( spheres.size() + 1 );
            Sphere & s = spheres[spheres.size() - 1];
            s.m_center = Vec3(1.0, -1.25, 0.5);
            s.m_radius = 0.75f;
            s.build_arrays();
            s.material.type = Material_Mirror;
            s.material.diffuse_material = Vec3( 1.,0.,0. );
            s.material.specular_material = Vec3( 1.,0.,0. );
            s.material.shininess = 16;
            s.material.transparency = 1.0;
            s.material.index_medium = 1.4;
        }


        { //MIRRORED Sphere
            spheres.resize( spheres.size() + 1 );
            Sphere & s = spheres[spheres.size() - 1];
            s.m_center = Vec3(-1.0, -1.25, -0.5);
            s.m_radius = 0.75f;
            s.build_arrays();
            s.material.type = Material_Glass;
            s.material.diffuse_material = Vec3( 1.,1.,1. );
            s.material.specular_material = Vec3(  1.,1.,1. );
            s.material.shininess = 16;
            s.material.transparency = 0.;
            s.material.index_medium = 0.;
        }

    }

};



#endif
