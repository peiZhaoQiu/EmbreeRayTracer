#pragma once

#include "Vec.hpp"


struct Ray{

    Vec3f origin;
    Vec3f direction;
    Ray(Vec3f o, Vec3f d):origin(o), direction(d){}



};