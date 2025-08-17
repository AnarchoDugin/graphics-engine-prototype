#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <string>

namespace WidgetSettings {
    extern float WINDOW_WIDTH;
    extern float WINDOW_HEIGHT;

    extern float TOOL_WINDOW_POS_X;
    extern float TOOL_WINDOW_POS_Y;

    extern float TOOL_WINDOW_HEIGHT;
    extern float TOOL_WINDOW_WIDTH;

    extern float LOG_WINDOW_POS_X;
    extern float LOG_WINDOW_POS_Y;

    extern float LOG_WINDOW_HEIGHT;
    extern float LOG_WINDOW_WIDTH;
}

namespace Environment {
    extern float BackgroundColor[3];
    extern bool WireframeMode;
    extern bool FpsCounter;
    extern bool Vsync;
    extern bool DepthTesting;
    extern bool Skybox;
    extern bool Grid;

    extern std::vector<std::string> LogMessages;
    void AddLog(const std::string& message);
}

namespace FilePaths {
    extern std::string VertexShader;
    extern std::string FragmentShader;

    extern std::string GridVert;
    extern std::string GridFrag;

    extern std::string SkyboxVert;
    extern std::string SkyboxFrag;

    extern std::string LightVert;
    extern std::string LightFrag;

    extern std::string ModelRepository;
    extern std::string Icon;

    extern std::string LightModel;

    extern std::vector<std::string> skyboxFaces;
}

#endif