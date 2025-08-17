#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <tinyobjloader/tiny_obj_loader.h>

#include <general/TextureObject.h>

class Object3D
{
private:
    unsigned int VertexArrayObject, VertexBufferObject, ElementBufferObject;
    size_t indicesCount;

    struct Vertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 texCoord;
    };

    TextureObject* diffuseMap;
    TextureObject* specularMap;
    
public:
    Object3D();
    ~Object3D();

    bool loadFromFile(const std::string& fileName);
    bool loadTextures(const std::string& textureDirectory);

    void render() const;
};

#endif


