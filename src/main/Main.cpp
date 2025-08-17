#include <iostream>

#include <glad/glad.h>
#include <stb/stb_image.h>

#include <general/Settings.h>
#include <general/ShaderProgram.h>
#include <general/SceneObject.h>
#include <general/Scene.h>
#include <general/SceneCamera.h>
#include <general/GridPlane.h>
#include <general/Skybox.h>
#include <general/UIManager.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <GLFW/glfw3.h>

GLFWwindow* CreateWindow();
void ResizeCallback(GLFWwindow* WindowContext, int width, int height);

void InitializeGlfwContext();
void InitializeOpenGLContext();

void SetupWidgetParameters();
void SetupGlViewport();
void SetupRenderProperties();

void ProcessWindowEvents(GLFWwindow* WindowContext);
void ProcessCameraUpdate(GLFWwindow* WindowContext, SceneCamera* camera);

int main()
{
    InitializeGlfwContext();
    GLFWwindow* WindowContext = CreateWindow();
    InitializeOpenGLContext();

    SetupWidgetParameters();
    SetupGlViewport();

    Scene scene;
    GridPlane grid(10.0f, 1.0f);
    Skybox skybox;
    UIManager ui;

    Object3D* lightSource = ResourceManager::loadLightSourceObject(FilePaths::LightModel);
    ResourceManager::setLightMesh(lightSource);
    
    ShaderProgram* gridProgram = scene.addShader("GridShader", FilePaths::GridVert, FilePaths::GridFrag);
    ShaderProgram* skyboxProgram = scene.addShader("SkyboxShader", FilePaths::SkyboxVert, FilePaths::SkyboxFrag);

    grid.setShader(gridProgram);
    skybox.setShader(skyboxProgram);
    skybox.loadCubemap(FilePaths::skyboxFaces);
    ui.initialize(WindowContext);

    Environment::AddLog("[SYSTEM]::Camera system created... [OK]");
    Environment::AddLog("[SYSTEM]::Shaders compiled... [OK]");

    float deltaTime = 0.0f;
    float lastFrameTime = 0.0f;

    glm::mat4 projectionMatrix = glm::perspective(glm::radians(45.0f), WidgetSettings::WINDOW_WIDTH / WidgetSettings::WINDOW_HEIGHT, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(WindowContext))
    {
        SceneCamera* currentCamera = scene.getActiveCamera();

        float currentFrameTime = glfwGetTime();
        deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        SetupRenderProperties();
        ProcessWindowEvents(WindowContext);

        if (Environment::Skybox)
            skybox.draw(currentCamera->getViewMatrix(), projectionMatrix);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        if (Environment::Grid)
            grid.draw(currentCamera->getViewMatrix(), projectionMatrix);

        if (currentCamera && currentCamera->isEnabled())
        {
            currentCamera->updateCamera(deltaTime);
            ProcessCameraUpdate(WindowContext, currentCamera);
        }

        scene.renderObjects(projectionMatrix);
        scene.renderLightSources(projectionMatrix);

        glDisable(GL_BLEND);

        ui.render(scene);

        glfwPollEvents();
        glfwSwapBuffers(WindowContext);
    }

    ui.cleanup();
    ResourceManager::clearAll();

    glfwDestroyWindow(WindowContext);
    glfwTerminate();

    return 0;
}

GLFWwindow* CreateWindow()
{
    const GLFWvidmode* WindowMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    const float WindowWidth = WindowMode->width * 0.95f;
    const float WindowHeight = WindowMode->height * 0.95f;

    WidgetSettings::WINDOW_WIDTH = WindowWidth;
    WidgetSettings::WINDOW_HEIGHT = WindowHeight;

    GLFWwindow* WindowContext = glfwCreateWindow(WindowWidth, WindowHeight, "Scene Visualizer", NULL, NULL);

    if (WindowContext == NULL)
    {
        std::cerr << "Failed to create GLFW window..." << std::endl;
        glfwTerminate();
        exit(1);
    }

    GLFWimage logotype[1];
	logotype[0].pixels = stbi_load(FilePaths::Icon.c_str(), &logotype[0].width, &logotype[0].height, 0, 4);
    
	glfwSetWindowIcon(WindowContext, 1, logotype);
	stbi_image_free(logotype[0].pixels);

    glfwMakeContextCurrent(WindowContext);
    glfwSetFramebufferSizeCallback(WindowContext, ResizeCallback);

    return WindowContext;
}

void InitializeGlfwContext()
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
}

void InitializeOpenGLContext()
{
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "ERROR::FAILED_TO_INIT_GLAD" << std::endl;
        exit(1);
    }
}

void SetupWidgetParameters()
{
    WidgetSettings::TOOL_WINDOW_WIDTH = WidgetSettings::WINDOW_WIDTH * 0.3f;
    WidgetSettings::TOOL_WINDOW_HEIGHT = WidgetSettings::WINDOW_HEIGHT;

    WidgetSettings::LOG_WINDOW_POS_X = WidgetSettings::TOOL_WINDOW_WIDTH;
    WidgetSettings::LOG_WINDOW_POS_Y = WidgetSettings::WINDOW_HEIGHT * 0.7f;

    WidgetSettings::LOG_WINDOW_WIDTH = WidgetSettings::WINDOW_WIDTH * 0.7f;
    WidgetSettings::LOG_WINDOW_HEIGHT = WidgetSettings::WINDOW_HEIGHT * 0.3f;
}

void SetupGlViewport()
{
    glViewport(WidgetSettings::LOG_WINDOW_POS_X, WidgetSettings::LOG_WINDOW_HEIGHT, WidgetSettings::LOG_WINDOW_WIDTH, WidgetSettings::LOG_WINDOW_POS_Y);
}

void SetupRenderProperties()
{
    if (Environment::WireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (!Environment::WireframeMode) glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    if (Environment::Vsync) glfwSwapInterval(1);
    if (!Environment::Vsync) glfwSwapInterval(0);

    if (Environment::DepthTesting)
    {
        glEnable(GL_DEPTH_TEST); 
        glClear(GL_DEPTH_BUFFER_BIT);
    }

    if (!Environment::DepthTesting) 
    {
        glDisable(GL_DEPTH_TEST);
    }

    glClearColor(Environment::BackgroundColor[0], Environment::BackgroundColor[1], Environment::BackgroundColor[2], 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void ProcessWindowEvents(GLFWwindow* WindowContext)
{
    if (glfwGetKey(WindowContext, GLFW_KEY_ESCAPE) == GLFW_PRESS) 
    {
        glfwSetWindowShouldClose(WindowContext, true);
    }
}

void ProcessCameraUpdate(GLFWwindow* WindowContext, SceneCamera* camera)
{
    camera->setMovementState(MOVE_FORWARD, glfwGetKey(WindowContext, GLFW_KEY_W) == GLFW_PRESS);
    camera->setMovementState(MOVE_BACKWARD, glfwGetKey(WindowContext, GLFW_KEY_S) == GLFW_PRESS);

    camera->setMovementState(MOVE_LEFT, glfwGetKey(WindowContext, GLFW_KEY_A) == GLFW_PRESS);
    camera->setMovementState(MOVE_RIGHT, glfwGetKey(WindowContext, GLFW_KEY_D) == GLFW_PRESS);

    camera->setMovementState(MOVE_UP, glfwGetKey(WindowContext, GLFW_KEY_Q) == GLFW_PRESS);
    camera->setMovementState(MOVE_DOWN, glfwGetKey(WindowContext, GLFW_KEY_E) == GLFW_PRESS);
    
    camera->setAngularState(ANGLE_LEFT, glfwGetKey(WindowContext, GLFW_KEY_LEFT) == GLFW_PRESS);
    camera->setAngularState(ANGLE_RIGHT, glfwGetKey(WindowContext, GLFW_KEY_RIGHT) == GLFW_PRESS);

    camera->setAngularState(ANGLE_UP, glfwGetKey(WindowContext, GLFW_KEY_UP) == GLFW_PRESS);
    camera->setAngularState(ANGLE_DOWN, glfwGetKey(WindowContext, GLFW_KEY_DOWN) == GLFW_PRESS);
}

void ResizeCallback(GLFWwindow* WindowContext, int width, int height)
{
    WidgetSettings::WINDOW_WIDTH = static_cast<float>(width);
    WidgetSettings::WINDOW_HEIGHT = static_cast<float>(height);

    SetupWidgetParameters();
    SetupGlViewport();
}
