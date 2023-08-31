#pragma once

#include "Vec.hpp"
#include "common.hpp"


enum MaterialType {DIFFUSE, EMISSION};

class Material
{

    public:

        MaterialType _type;
        //Vec3f _color;
        //Vec3f _emission;
        //Vec3f _specular;
        //virtual
        //Material();
        virtual float pdf(const Vec3f &wi, const Vec3f &wo, const Vec3f &N) = 0;
        virtual Vec3f sample(const Vec3f &wi, const Vec3f &N) = 0;
        Material(Vec3f emission, Vec3f specular, Vec3f diffuse): _emission(emission), _specular(specular), _diffuse(diffuse) {} 
        virtual Vec3f eval(const Vec3f &wi, const Vec3f &wo, const Vec3f &N) = 0;               

        Vec3f _specular;
        Vec3f _diffuse;
        Vec3f _emission;

        bool hasEmission(){
            return _emission.length() > MyEPSILON;
        }

    
};


struct MaterialInfo{
    Vec3f _emission;
    Vec3f _specular;
    Vec3f _diffuse;
    MaterialInfo(Vec3f emission, Vec3f specular, Vec3f diffuse): _emission(emission), _specular(specular), _diffuse(diffuse) {}
};

Vec3f toWorld(const Vec3f &a, const Vec3f &N){
    Vec3f B, C;
    if (std::fabs(N.x) > std::fabs(N.y)){
        float invLen = 1.0f / std::sqrt(N.x * N.x + N.z * N.z);
        C = Vec3f(N.z * invLen, 0.0f, -N.x *invLen);
    }
    else {
        float invLen = 1.0f / std::sqrt(N.y * N.y + N.z * N.z);
        C = Vec3f(0.0f, N.z * invLen, -N.y *invLen);
    }
    B = crossProduct(C, N);
    return a.x * B + a.y * C + a.z * N;
}


class diffuseMaterial: public Material{
    public:
        diffuseMaterial(Vec3f emission, Vec3f specular, Vec3f diffuse): Material(emission, specular, diffuse) {
            _type = DIFFUSE;
        }

        Vec3f sample(const Vec3f &wi, const Vec3f &N) override{
            // uniform sample on the hemisphere
            float x_1 = get_random_float(), x_2 = get_random_float();
            float z = std::fabs(1.0f - 2.0f * x_1);
            float r = std::sqrt(1.0f - z * z), phi = 2 * M_PI * x_2;
            Vec3f localRay(r*std::cos(phi), r*std::sin(phi), z);
            return toWorld(localRay, N);
        }

        Vec3f eval(const Vec3f &wi, const Vec3f &wo, const Vec3f &N)override{

            float cosTheta = dotProduct(N, wo);
            if (cosTheta <= 0.0f){
                return Vec3f(0.0f, 0.0f, 0.0f);
            }
            return _diffuse / M_PI;
        }

        float pdf(const Vec3f &wi, const Vec3f &wo, const Vec3f &N)override{
            return 0.5/M_PI;
        }
};

class MaterialFactory{
    public:
    static Material* createMaterial(Vec3f emission, Vec3f specular, Vec3f diffuse){
        return new diffuseMaterial(emission, specular, diffuse);
    } 
};







