#pragma once

#include "Vec.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Material.hpp"
#include "Object.hpp"
#include <vector>   
#include <string>
#include <cmath>
#include <iostream>



class Scene
{
    public:
        Scene(){};
        ~Scene()
        {
            size_t n_size = _materialList.size();
            for (int i = n_size - 1; i >= 0; i--)
            {
                if (_materialList[i] != nullptr){
                    delete _materialList[i];
                }
            }

            size_t o_size = _objectsList.size();
            for (int i = o_size - 1; i >= 0; i--)
            {
                if (_objectsList[i]._geometry != nullptr){
                    delete _objectsList[i]._geometry;
                }
            }

        }

    virtual Intersection castRay(Ray inputRay)=0;

    virtual void addMeshObj(std::string objFilePath, std::string objFile)=0;


    std::vector<Object> _objectsList;
    std::vector<Material*> _materialList;

    void sampleLight(Intersection &pos, float &pdf)
    {
        static bool first = true;
        static float emitArea = 0;
        if (first){
            for (size_t i = 0; i < _objectsList.size(); i++){
                if (_objectsList[i]._material->hasEmission()){
                    emitArea += _objectsList[i].getArea();
                }
            }
            first = false;
        }

        float p = glm::abs(get_random_float()) * emitArea;
        float area = 0;
        for (size_t i = 0; i < _objectsList.size(); i++)
        {
            if (_objectsList[i]._material->hasEmission())
            {
                area = area + _objectsList[i].getArea();
                if (area >= p){
                    _objectsList[i].Sample(pos, pdf);
                    //pdf /= emitArea;
                    return;
                }
            }
        }


    }


    Vec3f doRendering(const Ray &ray, int depth)
    {
        // if (depth > 5){
        //     return Vec3f(0, 0, 0);
        // }

        Vec3f L_dir = Vec3f(0, 0, 0);
        Vec3f L_indir = Vec3f(0, 0, 0);

        Intersection intersection = castRay(ray);

        if (!intersection._hit){
            return Vec3f(0, 0, 0);
        }

        if (intersection._material->hasEmission()){
            //std::cout << "here" << std::endl;
            return intersection._material->_emission;
            
        }

        Intersection lightInter;
        float lightPdf;
        sampleLight(lightInter, lightPdf);


        Vec3f lightDir = (lightInter._position - intersection._position).normalized(); // object to light
        float lightObjectdistance = (lightInter._position - intersection._position).length();
        Ray shadowRay(intersection._position, lightDir);
        Intersection shadowInter = castRay(shadowRay);

        if(shadowInter._hit && (shadowInter._position - lightInter._position).length()  < 0.1f){
            //auto v = intersection._material->eval(ray.direction, lightDir, intersection._normal);
            //float k = dotProduct(lightDir, intersection._normal);
            //float k1 = dotProduct(-lightDir, lightInter._normal);
            //float k2 = std::pow((lightInter._position - intersection._position).length(), 2);
            // std::cout << "k: " << k << std::endl;
            // std::cout << "v: " << v.x << " " << v.y << " " << v.z << std::endl;
            L_dir = lightInter._material->_emission * 
                                intersection._material->eval(ray.direction, lightDir, intersection._normal) *
                                dotProduct(lightDir, intersection._normal) *
                                dotProduct(-lightDir, lightInter._normal) /
                                (lightObjectdistance * lightObjectdistance) /
                                lightPdf;
        }


        if(depth <3 || get_random_float() < 0.8f)
        {
            Vec3f outDirction = intersection._material->sample(ray.direction, intersection._normal);
            Ray outRay(intersection._position, outDirction);
            Intersection outRayInter = castRay(outRay);
            if (outRayInter._hit && !outRayInter._material->hasEmission())
            {
                L_indir = doRendering(outRay, depth + 1) * 
                            intersection._material->eval(ray.direction, outDirction, intersection._normal) *
                            dotProduct(outDirction, intersection._normal) /
                            intersection._material->pdf(ray.direction, outDirction, intersection._normal) /
                            0.8f;
            }

        }

        return L_dir + L_indir;
        //return L_indir;
    }
    
    

};
