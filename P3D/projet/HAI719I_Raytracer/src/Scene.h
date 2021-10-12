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

//intersection le plus proche
struct RaySceneIntersection{
	//cest possible de ne pas avoir unr intersection
    bool intersectionExists;
//les types : maillage - 0, sphere-1, carre - 2  
    unsigned int typeOfIntersectedObject;
//identifiqnt dans les vecteurs de la scene
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
        //calculer les intersections avec les objets de la scene et garder la plus proche
        RaySceneIntersection result;
        int mesh_size = (int) meshes.size(), sphere_size = (int) spheres.size(), square_size = (int) squares.size();
        RayTriangleIntersection curr_mesh, tmp_mesh =  meshes[0].intersect(ray);//mesh_size <0 ? meshes[0].intersect(ray) : null;
        RaySphereIntersection curr_sphere, tmp_sphere = spheres[0].intersect(ray);//sphere_size <0 ? spheres[0].intersect(ray) : null;
        RaySquareIntersection curr_square, tmp_square =  squares[0].intersect(ray); //square_size <0 ? squares[0].intersect(ray) : null;
        //parcourur les objets pour trouver l'intersection la plus proche
        for (int mesh = 0; mesh < mesh_size; mesh++){
            curr_mesh = meshes[mesh].intersect(ray);
            if (curr_mesh.t < tmp_mesh.t)
                tmp_mesh = curr_mesh;
        }
        for (int sphere = 0; sphere < sphere_size; sphere++){
            curr_sphere = spheres[sphere].intersect(ray);
            if (curr_sphere.t < tmp_sphere.t)
                tmp_sphere = curr_sphere;
        }
        for (int square = 0; square < square_size; square++){   
            curr_square = squares[square].intersect(ray);
            if (curr_square.t < tmp_square.t)
                tmp_square = curr_square;      
        }

        //trouver la plus proche
        //a detailler
        if (tmp_mesh.t < tmp_sphere.t){
                result.t = tmp_mesh.t < tmp_square.t ? tmp_mesh.t : tmp_square.t;
        }else if (tmp_sphere.t < tmp_square.t){
                result.t = tmp_square.t < tmp_mesh.t ? tmp_square.t : tmp_mesh.t;  
        }
        
        return result;
    }





    Vec3 rayTraceRecursive( Ray ray , int NRemainingBounces ) {

        //TODO RaySceneIntersection raySceneIntersection = computeIntersection(ray);
        //coul par default - coul de fon d'ecran - ex vec3(0.,0,.0.)
        //verifier sily a une intersection
        //coul de l'objet et son material - > on l'a parce aue dans le resultat de computeIntersection il y a aussi l'index dans le bon vecteur de la mesh en question
        //
        Vec3 color = Vec3(0.,1.,0.);
        return color;
    }


    Vec3 rayTrace( Ray const & rayStart ) {
        //TODO appeler la fonction recursive
//parcourir tout la scene (tout les objets - les vecteurs des objets)et toruver le plus proche - le float le plus petit
//utilise computeIntersection
//le rayon reflechi est calcule par rapport de la normale

        Vec3 color = rayTraceRecursive(rayStart, 1);
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
};



#endif
