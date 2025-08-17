#ifndef SCENE_OBJECT_H
#define SCENE_OBJECT_H

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <general/Object3D.h>

class SceneObject
{
private:
    std::string name;
    Object3D* object3D;

    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;

    glm::mat4 modelMatrix;

    bool transformNeedsUpdate;

public:
    SceneObject(const std::string& name, Object3D* object);
    ~SceneObject();

    void setPosition(const glm::vec3& newPosition);
    void setRotation(const glm::vec3& newRotation);
    void setScale(const glm::vec3& newScale);

    const glm::vec3& getPosition() const { return position; }
    const glm::vec3& getRotation() const { return rotation; }
    const glm::vec3& getScale() const { return scale; }

    void setName(const std::string& newName);
    const std::string& getName() const { return name; }

    Object3D* getObject3D() const { return object3D; }
    
    const glm::mat4& getModelMatrix();

    void render() const;
};

#endif