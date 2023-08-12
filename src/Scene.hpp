#pragma once

#include "Vec.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Material.hpp"
#include <vector>   
#include <string>



class Scene
{
    public:
        Scene();
        ~Scene();

    Intersection castRay(Ray inputRay);

    std::vector<int> addMeshObj(std::string objFilePath, std::string objFile);


    
    

};
