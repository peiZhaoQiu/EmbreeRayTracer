#pragma once
#include "Geometry.hpp"
#include "Material.hpp"
#include <iostream>

class Object
{
public:
    Object(){};
    ~Object(){
        // std::cout << "Object destructor called" << std::endl;
        // if (_geometry != nullptr){
        //     delete _geometry;
        // }

        // if (_material != nullptr){
        //     delete _material;
        // }
    };
    Geometry* _geometry;
    Material* _material;
    Object(Geometry* geometry, Material* material): _geometry(geometry), _material(material) {}

    bool intersect(const Ray& ray){return _geometry->intersect(ray);}
    Intersection getIntersection(const Ray& ray){return _geometry->getIntersection(ray);}
    float getArea(){return _geometry->getArea();}
    void Sample(Intersection &pos,float &pdf)
    {
        pos._material = _material;
        _geometry->Sample(pos,pdf);
    }


};