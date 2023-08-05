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
#include <filesystem>


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
void errorFunction(void* userPtr, enum RTCError error, const char* str)
{
  printf("error %d: %s\n", error, str);
}

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
RTCDevice initializeDevice()
{
  RTCDevice device = rtcNewDevice(NULL);

  if (!device)
    printf("error %d: cannot create device\n", rtcGetDeviceError(NULL));

  rtcSetDeviceErrorFunction(device, errorFunction, NULL);
  return device;
}

/*
 * Create a scene, which is a collection of geometry objects. Scenes are 
 * what the intersect / occluded functions work on. You can think of a 
 * scene as an acceleration structure, e.g. a bounding-volume hierarchy.
 *
 * Scenes, like devices, are reference-counted.
 */
RTCScene initializeScene(RTCDevice device)
{
  RTCScene scene = rtcNewScene(device);

  /* 
   * Create a triangle mesh geometry, and initialize a single triangle.
   * You can look up geometry types in the API documentation to
   * find out which type expects which buffers.
   *
   * We create buffers directly on the device, but you can also use
   * shared buffers. For shared buffers, special care must be taken
   * to ensure proper alignment and padding. This is described in
   * more detail in the API documentation.
   */
  RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);
  float* vertices = (float*) rtcSetNewGeometryBuffer(geom,
                                                     RTC_BUFFER_TYPE_VERTEX,
                                                     0,
                                                     RTC_FORMAT_FLOAT3,
                                                     3*sizeof(float),
                                                     3);

  unsigned* indices = (unsigned*) rtcSetNewGeometryBuffer(geom,
                                                          RTC_BUFFER_TYPE_INDEX,
                                                          0,
                                                          RTC_FORMAT_UINT3,
                                                          3*sizeof(unsigned),
                                                          1);

  if (vertices && indices)
  {
    vertices[0] = 0.f; vertices[1] = 0.f; vertices[2] = 0.f;
    vertices[3] = 1.f; vertices[4] = 0.f; vertices[5] = 0.f;
    vertices[6] = 0.f; vertices[7] = 1.f; vertices[8] = 0.f;

    indices[0] = 0; indices[1] = 1; indices[2] = 2;
  }

  /*
   * You must commit geometry objects when you are done setting them up,
   * or you will not get any intersections.
   */
  rtcCommitGeometry(geom);

  /*
   * In rtcAttachGeometry(...), the scene takes ownership of the geom
   * by increasing its reference count. This means that we don't have
   * to hold on to the geom handle, and may release it. The geom object
   * will be released automatically when the scene is destroyed.
   *
   * rtcAttachGeometry() returns a geometry ID. We could use this to
   * identify intersected objects later on.
   */
  rtcAttachGeometry(scene, geom);
  rtcReleaseGeometry(geom);

  /*
   * Like geometry objects, scenes must be committed. This lets
   * Embree know that it may start building an acceleration structure.
   */
  rtcCommitScene(scene);

  return scene;
}

/*
 * Cast a single ray with origin (ox, oy, oz) and direction
 * (dx, dy, dz).
 */
void castRay(RTCScene scene, 
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

std::vector<int> addObject(RTCScene &scene, RTCDevice device, std::string objFilePath)
{
  std::vector<int> geomIDs;
  tinyobj::attrib_t attrib;
  std::vector<tinyobj::shape_t> shapes;
  std::vector<tinyobj::material_t> materials;
  std::string err;
  std::string warn;
  bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, objFilePath.c_str());

  if (!warn.empty()) {
    std::cout << warn << std::endl;
  }

  if(!err.empty()) {
    std::cerr << err << std::endl;
    return geomIDs;
  }

  if (!ret) {
    std::cerr << "Failed to load " << objFilePath << std::endl;
    return geomIDs;
  }
  std::cout << "Loaded " << objFilePath << shapes.size()<< std::endl;
  for (const auto& shape : shapes) {
    std::cout << shape.name << std::endl;
      for (size_t i = 0; i < shape.mesh.indices.size(); i += 3) {
        std::vector<glm::vec3> vertices;
        for (int j = 0; j < 3; ++j) {
            unsigned int index = shape.mesh.indices[i + j].vertex_index;
            float vx = attrib.vertices[3 * index];
            float vy = attrib.vertices[3 * index + 1];
            float vz = attrib.vertices[3 * index + 2];
            vertices.push_back(glm::vec3(vx, vy, vz));
          }

          RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);


            //RTCBuffer vertexBuffer = rtcNewBuffer(device, sizeof(glm::vec3), 3);
            glm::vec3* verticesPtr = (glm::vec3*)rtcSetNewGeometryBuffer(geom, 
                                                                        RTC_BUFFER_TYPE_VERTEX, 
                                                                        0, 
                                                                        RTC_FORMAT_FLOAT3, 
                                                                        sizeof(glm::vec3), 
                                                                        3);

                                                                      


            //RTCBuffer indexBuffer = rtcNewBuffer(device, sizeof(unsigned int), 3);
            unsigned* indicesPtr = (unsigned*)rtcSetNewGeometryBuffer(geom, 
                                                                      RTC_BUFFER_TYPE_INDEX, 
                                                                      0, 
                                                                      RTC_FORMAT_UINT3, 
                                                                      3*sizeof(unsigned), 
                                                                      1);
                                                    
            if  (verticesPtr && indicesPtr) 
            {
                    for (int j = 0; j < 3; ++j) {
                        verticesPtr[j] = vertices[j];
                    }
                    indicesPtr[0] = 0;
                    indicesPtr[1] = 1;
                    indicesPtr[2] = 2;
            }
            else
            {
                std::cout << "Failed to create buffers" << std::endl;
                return geomIDs;
            } 

            rtcCommitGeometry(geom);
            int geomID = rtcAttachGeometry(scene, geom);
            rtcReleaseGeometry(geom);
            geomIDs.push_back(geomID);
      }

  }


  return geomIDs;
}

/* -------------------------------------------------------------------------- */

int main()
{
  /* Initialization. All of this may fail, but we will be notified by
   * our errorFunction. */
  RTCDevice device = initializeDevice();

  RTCScene scene = rtcNewScene(device);
  // get source file directory path and convert to string

  std::string file_path = __FILE__;
  std::string dir_path = file_path.substr(0, file_path.rfind("/"));
  std::cout<<file_path<<std::endl;
  std::string ModelDir = dir_path + "/Model/";

  //RTCScene scene = initializeScene(device);
  auto is = addObject(scene, device, ModelDir + "floor.obj");
  std::cout << is.size() <<" "<<is[0] << std::endl;
  auto js = addObject(scene, device, ModelDir + "tallbox.obj");
  std::cout << js.size() << " "<<js[0] << std::endl;
  auto ks = addObject(scene, device, ModelDir + "shortbox.obj");
  std::cout << ks.size() <<" "<< ks[0] << std::endl;


  rtcCommitScene(scene);
  /* This will hit the triangle at t=1. */
  castRay(scene, 0.33f, 0.33f, -100.0f, 0, 0, 1);

  /* This will not hit anything. */
  castRay(scene, 1.00f, 1.00f, -100.0f, 0, 0, 1);

  castRay(scene, 150.00f, 240.00f, 167.0f, 0, -1, 0);



  /* Though not strictly necessary in this example, you should
   * always make sure to release resources allocated through Embree. */
  rtcReleaseScene(scene);
  rtcReleaseDevice(device);
  
  
  return 0;
}


