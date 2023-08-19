#pragma once

#include "Vec.hpp"
#include "Ray.hpp"
#include "Intersection.hpp"
#include "Material.hpp"
#include "Object.hpp"
#include <vector>   
#include <string>



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


    
    

};
