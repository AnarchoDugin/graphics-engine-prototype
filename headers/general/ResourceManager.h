#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <unordered_map>
#include <vector>
#include <string>

#include <general/Object3D.h>
#include <general/Scene.h>

class Scene;

class ResourceManager
{
private:
    static Object3D* lightSourceMesh;
    static std::unordered_map<std::string, Object3D*> objectCache;

public:
    static Object3D* loadObject(const std::string& path);
    static Object3D* loadLightSourceObject(const std::string& path);
    static Object3D* preloadDefaultResource(const std::string& path);

    static void setLightMesh(Object3D* mesh) { lightSourceMesh = mesh; }
    static Object3D* getLightMesh() { return lightSourceMesh; }

    static void cleanupUnused(Scene& scene);
    static void clearAll();
};

#endif