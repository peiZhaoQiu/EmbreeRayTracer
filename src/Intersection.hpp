#pragma once    

#include "Vec.hpp"
#include "Ray.hpp"
#include "Material.hpp"

struct Intersection{

    Vec3f _position = Vec3f(0,0,0);
    Vec3f _normal = Vec3f(0,0,0);
    float _distance = 0;
    Material* _material = nullptr;
    bool _hit =false;
};