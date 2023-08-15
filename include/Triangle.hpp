#pragma once
#include "Geometry.hpp"
#include "common.hpp"


class Triangle : public Object
{
    public:
         
        Vec3f _v1,_v2,_v3;
        Material* material; 
        Triangle (const Vec3f &v1,const Vec3f &v2, const Vec3f &v3, Material* m):_v1(v1),_v2(v2),_v3(v3),material(m)
        {
            e1 = _v2 - _v1;
            e2 = _v3 - _v1;
            normal = glm::normalize(glm::cross(e1,e2));
            area = glm::length(glm::cross(e1,e2))*0.5f;
        }

    bool intersect(const Ray& ray) override;
    Intersection getIntersection(const Ray& ray) override;
        
         
    float getArea() override{
        return this->area;
    }

    void Sample(Intersection &pos, float &pdf) override{
        pdf = 1.0f / area;
    }
 

    private:

        float area;
        Vec3f normal;
        Vec3f e1,e2;

};



