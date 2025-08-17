#include <general/ResourceManager.h>

std::unordered_map<std::string, Object3D*> ResourceManager::objectCache;
Object3D* ResourceManager::lightSourceMesh;

Object3D* ResourceManager::loadObject(const std::string& path)
{
    auto it = objectCache.find(path);

    if (it != objectCache.end())
    {
        return it->second;
    }

    Object3D* obj = new Object3D();

    if (!obj->loadFromFile(path))
    {
        delete obj;
        return nullptr;
    }

    objectCache[path] = obj;

    return obj;
}

Object3D* ResourceManager::loadLightSourceObject(const std::string& path)
{
    Object3D* obj = new Object3D();

    if (!obj->loadFromFile(path))
    {
        delete obj;
        return nullptr;
    }

    return obj;
}

void ResourceManager::cleanupUnused(Scene& scene)
{
    for (auto it = objectCache.begin(); it != objectCache.end();)
    {
        if (!scene.isObjectUsed(it->second))
        {
            delete it->second;    
            it = objectCache.erase(it);
        } 
        
        else 
        {
            ++it;
        }
    }
}

Object3D* ResourceManager::preloadDefaultResource(const std::string& path)
{
    Object3D* objectPointer = loadObject(path);
    return objectPointer;
}

void ResourceManager::clearAll()
{
    for (auto& pair : objectCache)
    {
        delete pair.second;
    }

    objectCache.clear();

    delete lightSourceMesh;
}