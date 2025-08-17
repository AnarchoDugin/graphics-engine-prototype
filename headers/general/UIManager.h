#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include <imgui/imgui.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/imgui_impl_opengl3.h>

#include <general/Scene.h>

class UIManager 
{
private:
    void setupStyle();
    void renderToolWidget(Scene& scene);
    void renderLogWidget();

    void renderAboutSection();
    void renderObjectSection(Scene& scene);
    void renderSelectionSection(Scene& scene);
    void renderLightingSection(Scene& scene);
    void renderCameraSection(Scene& scene);
    void renderTransformSection(Scene& scene);
    void renderPropertiesSection();
    void renderFpsOverlay();

public:
    UIManager();
    ~UIManager();

    void initialize(GLFWwindow* window);
    void cleanup();
    void render(Scene& scene);
};

#endif