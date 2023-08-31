// Copyright 2009-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0

#include <embree4/rtcore.h>
#include <stdio.h>
#include <math.h>
#include <limits>
#include <stdio.h>
#include <tiny_obj_loader.h>
#include <iostream>
#include <glm/glm.hpp>
#include "EmbreeScene.hpp"
#include "Vec.hpp"
#include "Camera.hpp" 
#include <fstream>
#include "common.hpp"


/* 
 * This is only required to make the tutorial compile even when
 * a custom namespace is set.
 */
#if defined(RTC_NAMESPACE_USE)
RTC_NAMESPACE_USE
#endif


int compoentToint(double x){
return (int)(255*(std::pow(clamp(x/255, 0.0, 1.0), 1/2.2)));
//return (int)(x);  
}


int main(){

  std::string file_path = __FILE__;
  std::string file_path_1 = file_path.substr(0, file_path.rfind("/"));
  std::string dir_path = file_path_1.substr(0, file_path_1.rfind("/"));
  std::cout<< file_path_1<< std::endl;
  std::cout<<dir_path<<std::endl;
  std::string ModelDir = dir_path + "/Model/";


  EmbreeScene scene;
  scene.addMeshObj(ModelDir, "floor.obj");
  scene.addMeshObj(ModelDir, "tallbox.obj");
  scene.addMeshObj(ModelDir, "shortbox.obj");
  scene.addMeshObj(ModelDir, "left.obj");
  scene.addMeshObj(ModelDir, "right.obj");
  scene.addMeshObj(ModelDir, "light.obj");

  Ray ray1(Vec3f(0.33f,0.33f,10.0f), Vec3f(0,0,1));
  Ray ray2(Vec3f(1.00f,100.00f,1.0f), Vec3f(0,-1,0));
  Ray ray3(Vec3f(150.00f,240.00f,167.0f), Vec3f(0,-1,0));
  Ray ray4(Vec3f(278.0f, 278.0f, -800.0f), Vec3f(0,0,1));

  // auto l = scene.castRay(ray1);
  // auto k = scene.castRay(ray2);
  // auto m = scene.castRay(ray3);
  auto n = scene.castRay(ray4);
  // Set up the camera parameters
  int imageWidth = 200;
  int imageHeight = 200;
  float fov = 40.0f; // Field of view in degrees

  
  // Camera position and look direction for the Cornell Box
  Vec3f cameraPosition(278.0f, 278.0f, -800.0f); // Example camera position
  Vec3f lookAt(278.0f, 278.0f, 0.0f); // Look at the center of the Cornell Box
  Vec3f up(0.0f, 1.0f, 0.0f); // Up direction
  
  Camera camera(imageWidth, imageHeight, fov, cameraPosition, lookAt, up);

    std::string filename = "image.ppm"; 
    std::ofstream file;
    file.open(filename, std::ios::binary); 
    file << "P3\n" << imageWidth << " " << imageHeight << "\n255\n";
    if(!file.is_open()){
        std::cout << "Error: Could not open file " << filename << std::endl;
        return -1;
    }
    int ssp = 64;
  // Render the image
  for (int j = 0; j < imageHeight; ++j) 
  {
      for (int i = 0; i < imageWidth; ++i) 
      {
        Vec3f pixelColor(0.0f, 0.0f, 0.0f);
        for (int s = 0; s < ssp; ++s) 
        {

          Vec3f rayDir = camera.getRayDirection(i, j);
           
          Ray ray(camera.getPosition(), rayDir);
          
         
          
          auto tem = scene.doRendering(ray, 0);
          // auto intersection = scene.castRay(ray);
          // if (intersection._hit) {
          //     pixelColor = intersection._material->_diffuse;
          // }
          // else{
          //     pixelColor = Vec3f(0.0f, 0.0f, 0.0f);
          // }
          //std::cout << tem.x << " " << tem.y << " " << tem.z << std::endl;
          pixelColor = pixelColor + tem;
        }
       // std::cout << "i :" << i << "  "<< "j :" << j << std::endl;
        pixelColor = pixelColor/ ssp;
        //std::cout << pixelColor.x << " " << pixelColor.y << " " << pixelColor.z << std::endl;
          // // Cast ray
          // Ray ray(camera.getPosition(), rayDir);
          // //std::cout << "Ray direction: " << rayDir.x << ", " << rayDir.y << ", " << rayDir.z << std::endl;
          
          // auto intersection = scene.castRay(ray);
          // //Vec3f pixelColor(0.0f, 0.0f, 0.0f);

          // if (intersection._hit) {
          //     pixelColor = intersection._material->_diffuse;
          // }
          std::cout << "progress : " << (float)(i + j * imageWidth) / (float)(imageWidth * imageHeight) * 100 << "%\r" << std::flush;

          auto r = compoentToint(pixelColor.x);
          auto g = compoentToint(pixelColor.y);
          auto b = compoentToint(pixelColor.z);

          file << r << " " << g << " " << b << " "; 

      }
  }

  file.close();
  std::cout << "Wrote image file " << filename << std::endl;


  return 0;
}


