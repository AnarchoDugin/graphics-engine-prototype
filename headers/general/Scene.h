#ifndef SCENE_H
#define SCENE_H

#include <unordered_map>
#include <vector>
#include <string>

#include <general/SceneObject.h>
#include <general/SceneCamera.h>
#include <general/ShaderProgram.h>
#include <general/ResourceManager.h>
#include <general/Skybox.h>
#include <general/GridPlane.h>
#include <general/LightSource.h>

#include <glm/glm.hpp>

class Scene
{
private:
    std::unordered_map<std::string, SceneObject*> objects;
    std::unordered_map<std::string, SceneCamera*> cameras;
    std::unordered_map<std::string, ShaderProgram*> shaders;
    
    SceneObject* selectedObject = nullptr;
    SceneCamera* activeCamera = nullptr;
    ShaderProgram* activeShader = nullptr;

    std::vector<LightSource*> lightSources;
    size_t maxLights = 8;

public:
    Scene();
    ~Scene();

    SceneObject* addObject(const std::string& name, Object3D* object);
    bool removeObject(const std::string& name);

    SceneObject* getObject(const std::string& name) const;
    SceneObject* getSelectedObject() const;

    void setSelectedObject(SceneObject* object);
    void setSelectedObject(const std::string& name);

    std::vector<const char*> getObjectNames() const;
    
    SceneCamera* addCamera(const std::string& name);
    bool removeCamera(const std::string& name);

    SceneCamera* getCamera(const std::string& name) const;
    SceneCamera* getActiveCamera() const;

    void setActiveCamera(SceneCamera* camera);
    void setActiveCamera(const std::string& name);

    std::vector<const char*> getCameraNames() const;
    
    ShaderProgram* addShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
    ShaderProgram* getShader(const std::string& name) const;

    ShaderProgram* getActiveShader() const;

    void setActiveShader(ShaderProgram* shader);
    void setActiveShader(const std::string& name);

    std::vector<const char*> getShaderNames() const;

    bool isObjectUsed(Object3D* object) const;

    LightSource* addLight(const glm::vec3& position, const glm::vec3& color);
    LightSource* getLight(size_t index);
    const LightSource* getLight(size_t index) const;
    
    bool removeLight(size_t index);
    size_t getLightCount() const;

    void renderObjects(const glm::mat4& projectionMatrix);
    void renderLightSources(const glm::mat4& projectionMatrix);
    
    void clear();
};

#endif