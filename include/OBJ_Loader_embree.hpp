#pragma once

#include <glm/glm.hpp>
#include <tiny_obj_loader.h>
#include <string>
#include <vector>
#include "Material.hpp"
#include "Vec.hpp"
#include <iostream>
#include "Triangle.hpp"

struct OBJ_result{
    std::vector<Triangle> Triangles;
    std::vector<MaterialInfo> MaterialsInfoList;
    std::vector<int> geomIDs;
    std::vector<int> materialIDs;
};

namespace OBJ_Loader
{

    OBJ_result addObject(RTCScene &scene, RTCDevice &device, std::string objFilePath, std::string objFile)
    {
        std::vector<int> geomIDs;
        // tinyobj::attrib_t attrib;
        // std::vector<tinyobj::shape_t> shapes;
        // std::vector<tinyobj::material_t> materials;
        // std::string err;
        // std::string warn;

        OBJ_result result;

        std::string inputfile = objFilePath + objFile;
        tinyobj::ObjReaderConfig reader_config;
        reader_config.mtl_search_path = objFilePath; // Path to material files

        tinyobj::ObjReader reader;

        if (!reader.ParseFromFile(inputfile, reader_config)) {
            if (!reader.Error().empty()) {
                std::cerr << "TinyObjReader: " << reader.Error();
            }
            return result;
        }

        if (!reader.Warning().empty()) {
            std::cout << "TinyObjReader: " << reader.Warning();
        }

        auto& attrib = reader.GetAttrib();
        auto& shapes = reader.GetShapes();
        auto& materials = reader.GetMaterials();
    std::cout << "Loading " << inputfile << std::endl;
        std::cout << shapes.size() << "  "<< materials.size() <<std::endl;
        for(size_t i = 0; i< materials.size();i++)
        {
            //std::cout << materials[0].diffuse << " " << std::endl;
            Vec3f diffuseVec(materials[i].diffuse[0],materials[i].diffuse[1],materials[i].diffuse[2]);
            Vec3f specularVec(materials[i].specular[0],materials[i].specular[1],materials[i].specular[2]);
            Vec3f emissionVec(materials[i].emission[0],materials[i].emission[1],materials[i].emission[2]);
            MaterialInfo mat = MaterialInfo(emissionVec,specularVec,diffuseVec);
            result.MaterialsInfoList.push_back(mat);

            std::cout << materials[i].emission[0] << " " << materials[i].emission[1] << " " << materials[i].emission[2] << std::endl;
            std::cout << materials[i].diffuse[0] << " " << materials[i].diffuse[1] << " " << materials[i].diffuse[2] << std::endl;
            std::cout << std::endl;
        }

       

            for (size_t i = 0; i<shapes.size(); i++) {
                tinyobj::shape_t shape = shapes[i];
                size_t index_offset = 0;

                std::cout << shape.name << std::endl;
                for (size_t i = 0; i < shape.mesh.indices.size(); i += 3) {
                    std::vector<Vec3f> vertices;
                    for (int j = 0; j < 3; ++j) {
                        unsigned int index = shape.mesh.indices[i + j].vertex_index;
                        float vx = attrib.vertices[3 * index];
                        float vy = attrib.vertices[3 * index + 1];
                        float vz = attrib.vertices[3 * index + 2];
                        vertices.push_back(Vec3f(vx, vy, vz));
                    }

                    RTCGeometry geom = rtcNewGeometry(device, RTC_GEOMETRY_TYPE_TRIANGLE);


                        //RTCBuffer vertexBuffer = rtcNewBuffer(device, sizeof(glm::vec3), 3);
                                Vec3f*  verticesPtr = (Vec3f*)rtcSetNewGeometryBuffer(geom, 
                                                                                    RTC_BUFFER_TYPE_VERTEX, 
                                                                                    0, 
                                                                                    RTC_FORMAT_FLOAT3, 
                                                                                    sizeof(Vec3f), 
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
                            return result;
                        } 

                        Triangle tri(vertices[0],vertices[1],vertices[2]);
                        result.Triangles.push_back(tri);
                        rtcCommitGeometry(geom);
                        int geomID = rtcAttachGeometry(scene, geom);
                        rtcReleaseGeometry(geom);
                        result.geomIDs.push_back(geomID);
                
                }
                for(auto& id:shape.mesh.material_ids)
                {

                    std::cout<< id << "  ";
                    result.materialIDs.push_back(id);
                }

                std::cout << std::endl;
                

            }
        std::cout << "Loaded " << inputfile << std::endl;

        return result;
    }

 







};