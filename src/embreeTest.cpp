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


int compoentToint(float x){
return (int)(255*(std::pow(clamp(x, 0.0, 1.0), 1/2.2)));
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


  // Set up the camera parameters
  int imageWidth = 1200/10;
  int imageHeight = 960/10;
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
  // for (int j = 0; j < imageHeight; ++j) 
  // {
  //     for (int i = 0; i < imageWidth; ++i) 
  //     {
  //       Vec3f pixelColor(0.0f, 0.0f, 0.0f);
  //       for (int s = 0; s < ssp; ++s) 
  //       {

  //         Vec3f rayDir = camera.getRayDirection(i, j);
  //         Ray ray(camera.getPosition(), rayDir);
          
         
          
  //         pixelColor +=scene.doRendering(ray);
  //         //pixelColor = pixelColor + tem;
  //       }
  //       pixelColor = pixelColor/ ssp;
  //        std::cout << "progress : " << (float)(i + j * imageWidth) / (float)(imageWidth * imageHeight) * 100 << "%\r" << std::flush;

  //         auto r = compoentToint(pixelColor.x);
  //         auto g = compoentToint(pixelColor.y);
  //         auto b = compoentToint(pixelColor.z);

  //         file << r << " " << g << " " << b << " "; 


  //     }
  // }

  file.close();
  std::cout << "Wrote image file " << filename << std::endl;
  std::cout << "sycl" << std::endl;

  return 0;
}


