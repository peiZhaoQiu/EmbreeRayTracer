#pragma once
#include "Geometry.hpp"
#include "common.hpp"


class Triangle : public Geometry
{
    public:
         
        Vec3f _v1,_v2,_v3;
        Triangle (const Vec3f &v1,const Vec3f &v2, const Vec3f &v3):_v1(v1),_v2(v2),_v3(v3)
        {
            e1 = _v2 - _v1;
            e2 = _v3 - _v1;
            normal = crossProduct(e1,e2).normalized();
            area = (crossProduct(e1,e2)).length()*0.5f;
        }
        ~Triangle(){}

    bool intersect(const Ray& ray) override{};
    Intersection getIntersection(const Ray& ray) override{};
        
         
    float getArea() override{
        return this->area;
    }

    void Sample(Intersection &pos, float &pdf) override{
        pdf = 1.0f / area;
        float x = get_random_float();
        float y = get_random_float();
        pos._position = _v1 * (1.0f - x) + _v2 * (x * (1.0f - y)) + _v3 * (x * y);
        pos._normal = normal;
    }
 

    private:

        float area;
        Vec3f normal;
        Vec3f e1,e2;

};



