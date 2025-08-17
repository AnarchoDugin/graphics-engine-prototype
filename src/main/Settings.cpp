#include <general/Settings.h>

namespace WidgetSettings 
{
    float WINDOW_WIDTH = 0.0f;
    float WINDOW_HEIGHT = 0.0f;

    float TOOL_WINDOW_POS_X = 0.0f;
    float TOOL_WINDOW_POS_Y = 0.0f;

    float TOOL_WINDOW_HEIGHT = 0.0f;
    float TOOL_WINDOW_WIDTH = 0.0f;

    float LOG_WINDOW_POS_X = 0.0f;
    float LOG_WINDOW_POS_Y = 0.0f;

    float LOG_WINDOW_HEIGHT = 0.0f;
    float LOG_WINDOW_WIDTH = 0.0f;
}

namespace Environment 
{
    float BackgroundColor[3] = { 0.0f, 0.0f, 0.0f };

    bool WireframeMode = false;
    bool FpsCounter = false;
    bool Vsync = false;
    bool DepthTesting = false;
    bool Skybox = false;
    bool Grid = false;

    std::vector<std::string> LogMessages;

    void AddLog(const std::string& message) 
    {
        LogMessages.push_back(message);
        if (LogMessages.size() > 100)
            LogMessages.erase(LogMessages.begin());
    }
}

namespace FilePaths 
{
    std::string VertexShader = "src/shaders/Basic.vert";
    std::string FragmentShader = "src/shaders/Basic.frag";

    std::string GridVert = "src/shaders/GridPlane.vert";
    std::string GridFrag = "src/shaders/GridPlane.frag";

    std::string SkyboxVert = "src/shaders/Skybox.vert";
    std::string SkyboxFrag = "src/shaders/Skybox.frag";

    std::string LightVert = "src/shaders/LightSource.vert";
    std::string LightFrag = "src/shaders/LightSource.frag";

    std::string LightModel = "assets/models/Sphere/Sphere.obj";

    std::string ModelRepository = "assets/models/";
    std::string Icon = "assets/icons/icon.png";

    std::vector<std::string> skyboxFaces = 
    {
        "assets/skyboxes/space/right.jpg",
        "assets/skyboxes/space/left.jpg",
        "assets/skyboxes/space/top.jpg",
        "assets/skyboxes/space/bottom.jpg",
        "assets/skyboxes/space/front.jpg",
        "assets/skyboxes/space/back.jpg"
    };
}