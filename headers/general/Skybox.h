#ifndef SKYBOX_H
#define SKYBOX_H

#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <general/ShaderProgram.h>

class Skybox
{
private:
    unsigned int cubemapTexture;
    unsigned int VAO, VBO;

    ShaderProgram* skyboxShader;

    void setupSkyboxMesh();

public:
    Skybox();
    ~Skybox();

    bool loadCubemap(const std::vector<std::string>& faces);
    void draw(const glm::mat4& view, const glm::mat4& projection);
    
    void setShader(ShaderProgram* shader) { skyboxShader = shader; }

};

#endif