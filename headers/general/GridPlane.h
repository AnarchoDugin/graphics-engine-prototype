#ifndef GRID_PLANE_H
#define GRID_PLANE_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <general/ShaderProgram.h>

class GridPlane
{
private:
    unsigned int VAO, VBO;
    int vertexCount;

    ShaderProgram* gridShader;

    float size;
    float step;
    
    void setupGridMesh();

public:
    GridPlane(float size = 10.0f, float step = 1.0f);
    ~GridPlane();

    void draw(const glm::mat4& view, const glm::mat4& projection);
    void setShader(ShaderProgram* shader) { gridShader = shader; }
};

#endif