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
//#include "OBJ_Loader.hpp"
#include "Scene.hpp"


/*
 * A minimal tutorial. 
 *
 * It demonstrates how to intersect a ray with a single triangle. It is
 * meant to get you started as quickly as possible, and does not output
 * an image. 
 *
 * For more complex examples, see the other tutorials.
 *
 * Compile this file using
 *   
 *   gcc -std=c99 \
 *       -I<PATH>/<TO>/<EMBREE>/include \
 *       -o minimal \
 *       minimal.c \
 *       -L<PATH>/<TO>/<EMBREE>/lib \
 *       -lembree4 
 *
 * You should be able to compile this using a C or C++ compiler.
 */

/* 
 * This is only required to make the tutorial compile even when
 * a custom namespace is set.
 */
#if defined(RTC_NAMESPACE_USE)
RTC_NAMESPACE_USE
#endif

/*
 * We will register this error handler with the device in initializeDevice(),
 * so that we are automatically informed on errors.
 * This is extremely helpful for finding bugs in your code, prevents you
 * from having to add explicit error checking to each Embree API call.
 */
// void errorFunction(void* userPtr, enum RTCError error, const char* str)
// {
//   printf("error %d: %s\n", error, str);
// }

/*
 * Embree has a notion of devices, which are entities that can run 
 * raytracing kernels.
 * We initialize our device here, and then register the error handler so that 
 * we don't miss any errors.
 *
 * rtcNewDevice() takes a configuration string as an argument. See the API docs
 * for more information.
 *
 * Note that RTCDevice is reference-counted.
 */
// RTCDevice initializeDevice()
// {
//   RTCDevice device = rtcNewDevice(NULL);

//   if (!device)
//     printf("error %d: cannot create device\n", rtcGetDeviceError(NULL));

//   rtcSetDeviceErrorFunction(device, errorFunction, NULL);
//   return device;
// }

/*
 * Cast a single ray with origin (ox, oy, oz) and direction
 * (dx, dy, dz).
 */
void castRay(RTCScene &scene, 
             float ox, float oy, float oz,
             float dx, float dy, float dz)
{
  /*
   * The ray hit structure holds both the ray and the hit.
   * The user must initialize it properly -- see API documentation
   * for rtcIntersect1() for details.
   */
  struct RTCRayHit rayhit;
  rayhit.ray.org_x = ox;
  rayhit.ray.org_y = oy;
  rayhit.ray.org_z = oz;
  rayhit.ray.dir_x = dx;
  rayhit.ray.dir_y = dy;
  rayhit.ray.dir_z = dz;
  rayhit.ray.tnear = 0;
  rayhit.ray.tfar = std::numeric_limits<float>::infinity();
  rayhit.ray.mask = -1;
  rayhit.ray.flags = 0;
  rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
  rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;


  /*
   * There are multiple variants of rtcIntersect. This one
   * intersects a single ray with the scene.
   */
  rtcIntersect1(scene, &rayhit);

  printf("%f, %f, %f: ", ox, oy, oz);
  if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
  {
    /* Note how geomID and primID identify the geometry we just hit.
     * We could use them here to interpolate geometry information,
     * compute shading, etc.
     * Since there is only a single triangle in this scene, we will
     * get geomID=0 / primID=0 for all hits.
     * There is also instID, used for instancing. See
     * the instancing tutorials for more information */
    printf("Found intersection on geometry %d, primitive %d at tfar=%f\n, with normal %f, %f, %f\n, intersection point %f, %f, %f\n", 
           rayhit.hit.geomID,
           rayhit.hit.primID,
           rayhit.ray.tfar,
           rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z,
           rayhit.ray.org_x + rayhit.ray.dir_x * rayhit.ray.tfar,
           rayhit.ray.org_y + rayhit.ray.dir_y * rayhit.ray.tfar,
           rayhit.ray.org_z + rayhit.ray.dir_z * rayhit.ray.tfar 
           );


           glm::vec3 normal(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z);
           normal = glm::normalize(normal);
           std::cout << "Normal: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
  }
  
  else
    printf("Did not find any intersection.\n");
}



/* -------------------------------------------------------------------------- */
/**/
// int main()
// {
//   /* Initialization. All of this may fail, but we will be notified by
//    * our errorFunction. */
//   RTCDevice device = initializeDevice();

//   RTCScene scene = rtcNewScene(device);
//   // get source file directory path and convert to string

//   std::string file_path = __FILE__;
//   std::string file_path_1 = file_path.substr(0, file_path.rfind("/"));
//   std::string dir_path = file_path_1.substr(0, file_path_1.rfind("/"));
//   std::cout<< file_path_1<< std::endl;
//   std::cout<<dir_path<<std::endl;
//   std::string ModelDir = dir_path + "/Model/";

//   //RTCScene scene = initializeScene(device);
//   auto is = OBJ_Loader::addObject(scene, device, ModelDir , "floor.obj");
//   //std::cout << is.size() <<" "<<is[0] << std::endl;
//   auto js = OBJ_Loader::addObject(scene, device, ModelDir , "tallbox.obj");
//   // std::cout << js.size() << " "<<js[0] << std::endl;
//   auto ks = OBJ_Loader::addObject(scene, device, ModelDir , "shortbox.obj");
//   // std::cout << ks.size() <<" "<< ks[0] << std::endl;

//   auto ls = OBJ_Loader::addObject(scene, device, ModelDir , "left.obj");
//   auto rs = OBJ_Loader::addObject(scene, device, ModelDir , "right.obj");

//   rtcCommitScene(scene);
//   /* This will hit the triangle at t=1. */
//   castRay(scene, 0.33f, 0.33f, 10.0f, 0, 0, 1);

//   /* This will not hit anything. */
//   castRay(scene, 1.00f, 100.00f, 1.0f, 0, -1, 0);

//   castRay(scene, 150.00f, 240.00f, 167.0f, 0, -1, 0);



//   /* Though not strictly necessary in this example, you should
//    * always make sure to release resources allocated through Embree. */
//   rtcReleaseScene(scene);
//   rtcReleaseDevice(device);
  
  
//   return 0;
// }



int main(){

  std::string file_path = __FILE__;
  std::string file_path_1 = file_path.substr(0, file_path.rfind("/"));
  std::string dir_path = file_path_1.substr(0, file_path_1.rfind("/"));
  std::cout<< file_path_1<< std::endl;
  std::cout<<dir_path<<std::endl;
  std::string ModelDir = dir_path + "/Model/";


  Scene scene;
  auto a = scene.addMeshObj(ModelDir, "floor.obj");
  auto b = scene.addMeshObj(ModelDir, "tallbox.obj");
  auto c = scene.addMeshObj(ModelDir, "shortbox.obj");
  auto d = scene.addMeshObj(ModelDir, "left.obj");
  auto e = scene.addMeshObj(ModelDir, "right.obj");

  Ray ray1(Vec3f(0.33f,0.33f,10.0f), Vec3f(0,0,1));
  Ray ray2(Vec3f(1.00f,100.00f,1.0f), Vec3f(0,-1,0));
  Ray ray3(Vec3f(150.00f,240.00f,167.0f), Vec3f(0,-1,0));


  auto l = scene.castRay(ray1);
  auto k = scene.castRay(ray2);
  auto m = scene.castRay(ray3);

  return 0;
}


