#pragma once

#include "Vec.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"

class Object
{
public:
    Object() {}
    virtual ~Object(){}
    virtual bool intersect(const Ray& ray) = 0;
    virtual Intersection getIntersection(const Ray& ray) = 0;
    virtual float getArea()= 0;
    virtual void Sample(Intersection &pos,float &pdf)=0;
    //virtual bool hasEmit() = 0; 
};