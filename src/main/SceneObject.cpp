#include <general/SceneObject.h>

SceneObject::SceneObject(const std::string& name, Object3D* object) 
    : name(name), object3D(object), position(0.0f), rotation(0.0f), scale(1.0f), transformNeedsUpdate(false) 
{
    modelMatrix = glm::mat4(1.0f);
}

SceneObject::~SceneObject() {}

void SceneObject::setPosition(const glm::vec3& newPosition) 
{
    position = newPosition;
    transformNeedsUpdate = true;
}

void SceneObject::setRotation(const glm::vec3& newRotation)
{
    rotation = newRotation;
    transformNeedsUpdate = true;
}

void SceneObject::setScale(const glm::vec3& newScale)
{
    scale = newScale;
    transformNeedsUpdate = true;
}

void SceneObject::setName(const std::string& newName)
{
    name = newName;
}

const glm::mat4& SceneObject::getModelMatrix()
{
    if (transformNeedsUpdate)
    {
        modelMatrix = glm::mat4(1.0f);

        modelMatrix = glm::translate(modelMatrix, position);
        
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

        modelMatrix = glm::scale(modelMatrix, scale);

        transformNeedsUpdate = false;
    }

    return modelMatrix;
}

void SceneObject::render() const 
{
    if (object3D) 
    {
        object3D->render();
    }
}