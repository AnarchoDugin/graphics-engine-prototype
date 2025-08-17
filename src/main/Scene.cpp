#include <general/Scene.h>
#include <general/Settings.h>

#include <algorithm>

Scene::Scene()
{
    addCamera("Default Camera");

    addShader("BasicShader", FilePaths::VertexShader, FilePaths::FragmentShader);
    addShader("LightShader", FilePaths::LightVert, FilePaths::LightFrag);
}

Scene::~Scene() 
{
    clear();
}

SceneObject* Scene::addObject(const std::string& name, Object3D* object)
{
    if (objects.find(name) != objects.end() || !object)
    {
        return nullptr;
    }
    
    SceneObject* newObj = new SceneObject(name, object);
    objects[name] = newObj;

    return newObj;
}

bool Scene::removeObject(const std::string& name)
{
    auto it = objects.find(name);
    
    if (it == objects.end())
    {
        return false;
    }
    
    if (selectedObject == it->second)
    {
        selectedObject = nullptr;
    }
    
    delete it->second;
    objects.erase(it);

    ResourceManager::cleanupUnused(*this);

    return true;
}

SceneObject* Scene::getObject(const std::string& name) const 
{
    auto it = objects.find(name);
    return it != objects.end() ? it->second : nullptr;
}

SceneObject* Scene::getSelectedObject() const 
{
    return selectedObject;
}

void Scene::setSelectedObject(SceneObject* object) 
{
    selectedObject = object;
}

void Scene::setSelectedObject(const std::string& name) 
{
    selectedObject = getObject(name);
}

std::vector<const char*> Scene::getObjectNames() const 
{
    std::vector<const char*> names;
    for (const auto& pair : objects)
    {
        names.push_back(pair.first.c_str());
    }

    return names;
}

bool Scene::isObjectUsed(Object3D* object) const 
{
    for (const auto& pair : objects)
    {
        if (pair.second->getObject3D() == object)
        {
            return true;
        }
    }

    return false;
}

SceneCamera* Scene::addCamera(const std::string& name) 
{
    if (cameras.find(name) != cameras.end())
    {
        return nullptr;
    }
    
    SceneCamera* newCamera = new SceneCamera(name);
    
    if (!activeCamera)
    {
        activeCamera = newCamera;
    }

    cameras[name] = newCamera;

    return newCamera;
}

bool Scene::removeCamera(const std::string& name) 
{
    auto it = cameras.find(name);

    if (it == cameras.end()) 
    {
        return false;
    }
    
    if (activeCamera == it->second) 
    {
        activeCamera = cameras.empty() ? nullptr : cameras.begin()->second;
    }
    
    delete it->second;
    cameras.erase(it);

    return true;
}

SceneCamera* Scene::getCamera(const std::string& name) const 
{
    auto it = cameras.find(name);
    return it != cameras.end() ? it->second : nullptr;
}

SceneCamera* Scene::getActiveCamera() const 
{
    return activeCamera;
}

void Scene::setActiveCamera(SceneCamera* camera)
{
    activeCamera = camera;
}

void Scene::setActiveCamera(const std::string& name) 
{
    activeCamera = getCamera(name);
}

std::vector<const char*> Scene::getCameraNames() const 
{
    std::vector<const char*> names;
    for (const auto& pair : cameras)
    {
        names.push_back(pair.first.c_str());
    }

    return names;
}

ShaderProgram* Scene::addShader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath) 
{
    if (shaders.find(name) != shaders.end())
    {
        return nullptr;
    }
    
    ShaderProgram* shader = new ShaderProgram(name, vertexPath, fragmentPath);
    shaders[name] = shader;
    
    return shader;
}

ShaderProgram* Scene::getShader(const std::string& name) const 
{
    auto it = shaders.find(name);
    return it != shaders.end() ? it->second : nullptr;
}

ShaderProgram* Scene::getActiveShader() const 
{
    return activeShader;
}

void Scene::setActiveShader(ShaderProgram* shader)
{
    activeShader = shader;
}

void Scene::setActiveShader(const std::string& name) 
{
    activeShader = getShader(name);
}

std::vector<const char*> Scene::getShaderNames() const 
{
    std::vector<const char*> names;
    for (const auto& pair : shaders)
    {
        names.push_back(pair.first.c_str());
    }

    return names;
}

void Scene::renderObjects(const glm::mat4& projectionMatrix)
{
    if (!activeCamera || objects.size() < 1)
    {
        return;
    }
    
    setActiveShader("BasicShader");
    activeShader->use();

    activeShader->setMat4("ViewMatrix", activeCamera->getViewMatrix());
    activeShader->setMat4("ProjectionMatrix", projectionMatrix);
    activeShader->setInt("lightCount", static_cast<int>(lightSources.size()));
    activeShader->setVec3("viewPos", activeCamera->getPositionVector());
    
    for (size_t i = 0; i < lightSources.size(); ++i) 
    {
        const LightSource* light = lightSources[i];

        std::string prefix = "lights[" + std::to_string(i) + "]";
            
        activeShader->setVec3(prefix + ".position", light->getPosition());
        activeShader->setVec3(prefix + ".color", light->getColor());
            
        float constant, linear, quadratic;
        light->getAttenuation(constant, linear, quadratic);

        activeShader->setFloat(prefix + ".constant", constant);
        activeShader->setFloat(prefix + ".linear", linear);
        activeShader->setFloat(prefix + ".quadratic", quadratic);
    }

    
    for (const auto& pair : objects)
    {
        auto modelMatrix = pair.second->getModelMatrix();
        activeShader->setMat4("ModelMatrix", modelMatrix);
        pair.second->render();
    }
}

void Scene::clear() 
{
    for (auto& pair : objects)
    {
        delete pair.second;
    }

    objects.clear();
    
    for (auto& pair : cameras)
    {
        delete pair.second;
    }

    cameras.clear();
    
    for (auto& pair : shaders)
    {
        delete pair.second;
    }

    shaders.clear();

    for (auto& light : lightSources)
    {
        delete light;
    }
    
    selectedObject = nullptr;
    activeCamera = nullptr;
    activeShader = nullptr;
}

LightSource* Scene::addLight(const glm::vec3& position, const glm::vec3& color)
{
    if (lightSources.size() >= maxLights) 
    {
        return nullptr;
    }

    LightSource* lightSource = new LightSource(position, color, ResourceManager::getLightMesh());

    lightSource->setAttenuation(1.0, 0.07, 0.017);

    //TODO: Make custom attenuation available

    lightSources.push_back(lightSource);

    return lightSources.back();
}

bool Scene::removeLight(size_t index) 
{
    if (index >= lightSources.size()) 
    {
        return false;
    }

    delete lightSources[index];
    lightSources.erase(lightSources.begin() + index);

    return true;
}

LightSource* Scene::getLight(size_t index)
{
    return lightSources[index];
}

const LightSource* Scene::getLight(size_t index) const 
{
    return lightSources[index];
}

size_t Scene::getLightCount() const 
{
    return lightSources.size();
}

void Scene::renderLightSources(const glm::mat4& projectionMatrix) 
{
    if (!activeCamera || getLightCount() < 1)
    {
        return;
    }
    
    setActiveShader("LightShader");
    activeShader->use();

    activeShader->setMat4("ViewMatrix", activeCamera->getViewMatrix());
    activeShader->setMat4("ProjectionMatrix", projectionMatrix);
    
    for (size_t i = 0; i < lightSources.size(); ++i) 
    {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, lightSources[i]->getPosition());
        model = glm::scale(model, glm::vec3(0.5f));

        activeShader->setMat4("ModelMatrix", model);
        activeShader->setVec3("lightColor", lightSources[i]->getColor());

        lightSources[i]->draw();
    }
}