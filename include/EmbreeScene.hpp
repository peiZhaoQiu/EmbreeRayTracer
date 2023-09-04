#pragma once
#include "Scene.hpp"
#include <embree4/rtcore.h>
#include <string>
#include <unordered_map>
#include "OBJ_Loader_embree.hpp"




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

RTCDevice& getDevice()
{


    static RTCDevice device = initializeDevice();


    return device;
}


RTCScene initializeScene(RTCDevice device) {
    RTCScene scene = rtcNewScene(device);
    return scene;
}


RTCScene& getScene()
{

    auto _device = getDevice();

    static RTCScene scene = initializeScene(_device);


    return scene;
}

Scene::~Scene()
{

    auto _scene = getScene();
    auto _device = getDevice();

    if (_scene != nullptr){
        rtcReleaseScene(_scene);
    }
    if(_device != nullptr){
        rtcReleaseDevice(_device);
    }


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

Scene::Scene()
{

    RTCDevice _device = getDevice();
    RTCScene _scene = getScene();

}

Intersection Scene::castRay(Ray inputRay) 
{
      auto _scene = getScene();
      auto _device = getDevice();
      struct RTCRayHit rayhit;
      rayhit.ray.org_x = inputRay.origin.x;
      rayhit.ray.org_y = inputRay.origin.y;
      rayhit.ray.org_z = inputRay.origin.z;
      rayhit.ray.dir_x = inputRay.direction.x;
      rayhit.ray.dir_y = inputRay.direction.y;
      rayhit.ray.dir_z = inputRay.direction.z;
      rayhit.ray.tnear = 0.0f;
      rayhit.ray.tfar = std::numeric_limits<float>::infinity();
      rayhit.ray.mask = -1;
      rayhit.ray.flags = 0;
      rayhit.hit.geomID = RTC_INVALID_GEOMETRY_ID;
      rayhit.hit.instID[0] = RTC_INVALID_GEOMETRY_ID;


      //auto intersection = castRay(_scene, inputRay);


      rtcIntersect1(_scene, &rayhit);

      //   if (rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID)
      // {
      //   /* Note how geomID and primID identify the geometry we just hit.
      //   * We could use them here to interpolate geometry information,
      //   * compute shading, etc.
      //   * Since there is only a single triangle in this scene, we will
      //   * get geomID=0 / primID=0 for all hits.
      //   * There is also instID, used for instancing. See
      //   * the instancing tutorials for more information */

      //   printf("Found intersection on geometry %d, primitive %d at tfar=%f\n, with normal %f, %f, %f\n, intersection point %f, %f, %f\n", 
      //         rayhit.hit.geomID,
      //         rayhit.hit.primID,
      //         rayhit.ray.tfar,
      //         rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z,
      //         rayhit.ray.org_x + rayhit.ray.dir_x * rayhit.ray.tfar,
      //         rayhit.ray.org_y + rayhit.ray.dir_y * rayhit.ray.tfar,
      //         rayhit.ray.org_z + rayhit.ray.dir_z * rayhit.ray.tfar 
      //         );


      //         Vec3f normal(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z);
      //         normal = glm::normalize(normal);
      //         // std::cout << "Normal: " << normal.x << ", " << normal.y << ", " << normal.z << std::endl;
      // }
      
      // else
      //   {printf("Did not find any intersection.\n");}


      Intersection intersection;
      intersection._hit = rayhit.hit.geomID != RTC_INVALID_GEOMETRY_ID;
      if (!intersection._hit)
      {
          return intersection;
      }
      intersection._position = Vec3f(rayhit.ray.org_x + rayhit.ray.dir_x * rayhit.ray.tfar,
                                        rayhit.ray.org_y + rayhit.ray.dir_y * rayhit.ray.tfar,
                                        rayhit.ray.org_z + rayhit.ray.dir_z * rayhit.ray.tfar);
      intersection._normal = Vec3f(rayhit.hit.Ng_x, rayhit.hit.Ng_y, rayhit.hit.Ng_z);
      intersection._normal = (intersection._normal).normalized();
      intersection._distance = rayhit.ray.tfar;
      intersection._material = _objectMap[rayhit.hit.geomID]._material;



      return intersection;
}


void Scene::addMeshObj(std::string objFilePath, std::string objFile) 
{
  auto _scene = getScene();
  auto _device = getDevice();
  OBJ_result result = OBJ_Loader::addObject( _scene, _device, objFilePath, objFile);

  std::vector<Material*> materialPtrList;

  for (size_t i = 0;i<result.MaterialsInfoList.size();i++)
  {

    //Material(Vec3f emission, Vec3f specular, Vec3f diffuse);
    //Material* materialPtr = new Material(result.Materials[i]._emission,result.Materials[i]._specular,result.Materials[i]._diffuse);

    //materialPtrList.emplace_back(materialPtr);
    MaterialInfo matInfo = result.MaterialsInfoList[i];
    Material* materialPtr = MaterialFactory::createMaterial(matInfo._emission,matInfo._specular,matInfo._diffuse);
    materialPtrList.emplace_back(materialPtr);
    _materialList.emplace_back(materialPtr); 
  }

  for(size_t i = 0; i< result.Triangles.size();i++)
  {
    
      Triangle* triPtr = new Triangle(result.Triangles[i]._v1,result.Triangles[i]._v2,result.Triangles[i]._v3);

      Object new_obj = Object(triPtr,materialPtrList[result.materialIDs[i]]);
      _objectsList.emplace_back(new_obj);
      _objectMap[result.geomIDs[i]] = new_obj;
      // std::cout << result.geomIDs[i] << std::endl;
  }

  
  
  rtcCommitScene(_scene);
}














