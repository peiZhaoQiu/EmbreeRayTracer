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
        Scene();
        ~Scene();
  

    Intersection castRay(Ray inputRay);

    void addMeshObj(std::string objFilePath, std::string objFile);


    std::vector<Object> _objectsList;
    std::vector<Material*> _materialList;


    std::unordered_map<int,Object> _objectMap;

    void sampleLight(Intersection &pos, float &pdf)
    {
        float emitArea = 0;

        for (size_t i = 0; i < _objectsList.size(); i++){
            if (_objectsList[i]._material->hasEmission())
            {
                    emitArea += _objectsList[i].getArea();
            }
        }

    

        float p = std::abs(get_random_float()) * emitArea;
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


Vec3f doRendering(const Ray &initialRay)
{
    Vec3f L_total = Vec3f(0, 0, 0);
    //Vec3f L_indir = Vec3f(0, 0, 0);
    Ray currentRay = initialRay;
    int maxDepth = 50;

    Vec3f indirLightParam[maxDepth];
    Vec3f dirLight[maxDepth];

    for (int i = 0; i < maxDepth; i++)
    {
        indirLightParam[i] = Vec3f(0, 0, 0);
        dirLight[i] = Vec3f(0, 0, 0);
    }

    int depth = 0;
    
    for (depth = 0; depth < maxDepth; ++depth)
    {
        Vec3f L_dir = Vec3f(0, 0, 0);
        Vec3f LIndirParam = Vec3f(0, 0, 0);
        Vec3f outDirction;
        Intersection intersection = castRay(currentRay);

        if (!intersection._hit)
        {
            break; // Terminate if no intersection
        }

        if (intersection._material->hasEmission())
        {
            dirLight[depth] = intersection._material->_emission;
            break; // Terminate if the material has emission
        }

        Intersection lightInter;
        float lightPdf;
        sampleLight(lightInter, lightPdf);

        Vec3f lightDir = (lightInter._position - intersection._position).normalized();
        float lightObjectdistance = (lightInter._position - intersection._position).length();
        Ray shadowRay(intersection._position, lightDir);
        Intersection shadowInter = castRay(shadowRay);

        if (shadowInter._hit && (shadowInter._position - lightInter._position).length() < 0.1f)
        {
            L_dir = lightInter._material->_emission *
                     intersection._material->eval(currentRay.direction, lightDir, intersection._normal) *
                     dotProduct(lightDir, intersection._normal) *
                     dotProduct(-lightDir, lightInter._normal) /
                     (lightObjectdistance * lightObjectdistance) /
                     lightPdf;
        }

        dirLight[depth] = L_dir;

        if (depth < 3 || get_random_float() < 0.8f)
        {
            outDirction = intersection._material->sample(currentRay.direction, intersection._normal);
            Ray outRay(intersection._position, outDirction);
            Intersection outRayInter = castRay(outRay);

            if (outRayInter._hit && !outRayInter._material->hasEmission())
            {
                LIndirParam = intersection._material->eval(currentRay.direction, outDirction, intersection._normal) *
                               dotProduct(outDirction, intersection._normal) /
                               intersection._material->pdf(currentRay.direction, outDirction, intersection._normal) /
                               0.8f;
            }
        }
        else
        {
            indirLightParam[depth] = Vec3f(0, 0, 0);
            break;
        }

        //L_indir = LIndirParam;
        indirLightParam[depth] = LIndirParam;
        //L_total += L_dir + L_indir;

        currentRay = Ray(intersection._position, outDirction); // Update the ray for the next iteration
    }


    for (int i = depth; i > 0; i--)
    {
        //L_indir = indirLightParam[i] + dirLight[i] * L_indir;
        auto tem = (L_total + dirLight[i]); 
        L_total = tem * indirLightParam[i-1];
    }

    L_total = L_total * indirLightParam[0] + dirLight[0];
    return L_total;
}
    
    

};
