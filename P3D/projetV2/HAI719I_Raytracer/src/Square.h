#ifndef SQUARE_H
#define SQUARE_H
#include "Vec3.h"
#include <vector>
#include "Mesh.h"
#include <cmath>

struct RaySquareIntersection
{
    bool intersectionExists;
    float t;
    float u, v;
    Vec3 intersection;
    Vec3 normal;
    RaySquareIntersection() : intersectionExists(false), t(FLT_MAX) {}
};

class Square : public Mesh
{
public:
    Vec3 m_normal;
    Vec3 m_bottom_left;
    Vec3 m_right_vector;
    Vec3 m_up_vector;

    Square() : Mesh() {}
    Square(Vec3 const &bottomLeft, Vec3 const &rightVector, Vec3 const &upVector, float width = 1., float height = 1.,
           float uMin = 0.f, float uMax = 1.f, float vMin = 0.f, float vMax = 1.f) : Mesh()
    {
        setQuad(bottomLeft, rightVector, upVector, width, height, uMin, uMax, vMin, vMax);
    }

    void setQuad(Vec3 const &bottomLeft, Vec3 const &rightVector, Vec3 const &upVector, float width = 1., float height = 1.,
                 float uMin = 0.f, float uMax = 1.f, float vMin = 0.f, float vMax = 1.f)
    {
        m_right_vector = rightVector;
        m_up_vector = upVector;
        m_normal = Vec3::cross(rightVector, upVector);
        m_bottom_left = bottomLeft;

        m_normal.normalize();
        m_right_vector.normalize();
        m_up_vector.normalize();

        m_right_vector = m_right_vector * width;
        m_up_vector = m_up_vector * height;

        vertices.clear();
        vertices.resize(4);
        vertices[0].position = bottomLeft;
        vertices[0].u = uMin;
        vertices[0].v = vMin;
        vertices[1].position = bottomLeft + m_right_vector;
        vertices[1].u = uMax;
        vertices[1].v = vMin;
        vertices[2].position = bottomLeft + m_right_vector + m_up_vector;
        vertices[2].u = uMax;
        vertices[2].v = vMax;
        vertices[3].position = bottomLeft + m_up_vector;
        vertices[3].u = uMin;
        vertices[3].v = vMax;
        vertices[0].normal = vertices[1].normal = vertices[2].normal = vertices[3].normal = m_normal;
        triangles.clear();
        triangles.resize(2);
        triangles[0][0] = 0;
        triangles[0][1] = 1;
        triangles[0][2] = 2;
        triangles[1][0] = 0;
        triangles[1][1] = 2;
        triangles[1][2] = 3;
    }

    RaySquareIntersection intersect(const Ray &ray) const
    {
        RaySquareIntersection intersection;

        //TODO calculer l'intersection rayon quad

        //𝑡 = (𝐷 − 𝒐. 𝒏) / (𝒅. 𝒏)
        /*  
        ▪ t infini ⟹ rayon parallèle et distinct du plan
        ▪ t non-défini ⟹ rayon confondu avec le plan
        ▪ t < 0 ⟹ intersection derrière la caméra
        ▪ t > 0 ⟹ intersection devant la caméra */

        Vec3 r = vertices[1].position - vertices[0].position;
        Vec3 u = vertices[3].position - vertices[0].position;
        Vec3 d = ray.direction(),
             o = ray.origin(),
             a = vertices[0].position,
             n = Vec3::cross(r, u);
        n.normalize();
        float D = Vec3::dot(a, n);

        float t = (D - Vec3::dot(o, n)) / Vec3::dot(d, n);
        Vec3 p = o + t * d;
        if (t > 0)
        {
            Vec3 v0, v1, v2, v3;
            v0 = Vec3::cross(vertices[1].position - vertices[0].position, p - vertices[0].position);
            v1 = Vec3::cross(vertices[2].position - vertices[1].position, p - vertices[1].position);
            v2 = Vec3::cross(vertices[3].position - vertices[2].position, p - vertices[2].position);
            v3 = Vec3::cross(vertices[0].position - vertices[3].position, p - vertices[3].position);
            bool positif = Vec3::dot(v0, n) > 0;
            if ((Vec3::dot(v1, n) > 0) == positif)
                if ((Vec3::dot(v2, n) > 0) == positif)
                    if ((Vec3::dot(v3, n) > 0) == positif)
                    {

                        intersection.t = t;
                        intersection.intersectionExists = true;
                        intersection.intersection = p; 
                        intersection.normal = n;
                    }
        }

        return intersection;
    }
};
#endif // SQUARE_H
