#ifndef LIGHT_SOURCE_H
#define LIGHT_SOURCE_H

#include <glm/glm.hpp>

#include <general/ShaderProgram.h>
#include <general/Object3D.h>

#include <vector>

class LightSource
{
private:
    glm::vec3 position;
    glm::vec3 color;

    Object3D* lightModel;
    
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;

public:
    LightSource(const glm::vec3& position, const glm::vec3& color, Object3D* model);
    ~LightSource();

    void draw();

    void setPosition(const glm::vec3& newPosition) { position = newPosition; }
    void setColor(const glm::vec3& newColor) { color = newColor; }

    void setAttenuation(float constant, float linear, float quadratic); 
    void getAttenuation(float& constant, float& linear, float& quadratic) const;

    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getColor() const { return color; }
};

#endif