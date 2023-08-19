#pragma once

#include "Vec.hpp"
#include "common.hpp"


enum MaterialType {DIFFUSE, EMISSION};

class Material
{

    public:

        MaterialType _type;
        //Vector3f _color;
        //Vector3f _emission;
        //Vector3f _specular;
        //virtual
        //Material();
        Material(Vec3f emission, Vec3f specular, Vec3f diffuse): _emission(emission), _specular(specular), _diffuse(diffuse) {}     

        Vec3f _specular;
        Vec3f _diffuse;
        Vec3f _emission;

        bool hasEmission(){
            return glm::length(_emission) > EPSILON;
        }

    
};








