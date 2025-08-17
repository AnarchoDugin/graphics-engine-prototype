#include <general/UIManager.h>
#include <general/Settings.h>

UIManager::UIManager() {}
UIManager::~UIManager() {}

void UIManager::initialize(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = NULL;

    setupStyle();
    
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    Environment::AddLog("[SYSTEM]::GUI initialized... [OK]");
}

void UIManager::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UIManager::setupStyle() 
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowRounding = 2.0f;
    style.FrameRounding = 3.0f;
    style.GrabRounding = 1.0f;
    style.WindowBorderSize = 1.0f;
    style.FrameBorderSize = 0.5f;
    style.PopupBorderSize = 1.0f;

    style.Alpha = 0.95f;
    ImVec4* colors = style.Colors;
    
    colors[ImGuiCol_Text] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextDisabled] = ImVec4(0.00f, 0.40f, 0.41f, 1.00f);
	colors[ImGuiCol_WindowBg] = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
	colors[ImGuiCol_Border] = ImVec4(0.00f, 1.00f, 1.00f, 0.65f);
	colors[ImGuiCol_FrameBg] = ImVec4(0.44f, 0.80f, 0.80f, 0.18f);
	colors[ImGuiCol_FrameBgHovered] = ImVec4(0.44f, 0.80f, 0.80f, 0.27f);
	colors[ImGuiCol_FrameBgActive] = ImVec4(0.44f, 0.81f, 0.86f, 0.66f);
	colors[ImGuiCol_TitleBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.00f, 0.00f, 0.00f, 0.54f);
	colors[ImGuiCol_TitleBgActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.27f);
	colors[ImGuiCol_MenuBarBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
	colors[ImGuiCol_ScrollbarBg] = ImVec4(0.22f, 0.29f, 0.30f, 0.71f);
	colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.44f);
	colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
	colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_CheckMark] = ImVec4(0.00f, 1.00f, 1.00f, 0.68f);
	colors[ImGuiCol_SliderGrab] = ImVec4(0.00f, 1.00f, 1.00f, 0.36f);
	colors[ImGuiCol_SliderGrabActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.76f);
	colors[ImGuiCol_Button] = ImVec4(0.00f, 0.65f, 0.65f, 0.46f);
	colors[ImGuiCol_ButtonHovered] = ImVec4(0.01f, 1.00f, 1.00f, 0.43f);
	colors[ImGuiCol_ButtonActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.62f);
	colors[ImGuiCol_Header] = ImVec4(0.00f, 1.00f, 1.00f, 0.33f);
	colors[ImGuiCol_HeaderHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.42f);
	colors[ImGuiCol_HeaderActive] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
	colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 1.00f, 1.00f, 0.54f);
	colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.00f, 1.00f, 1.00f, 0.74f);
	colors[ImGuiCol_ResizeGripActive] = ImVec4(0.00f, 1.00f, 1.00f, 1.00f);
	colors[ImGuiCol_TextSelectedBg] = ImVec4(0.00f, 1.00f, 1.00f, 0.22f);
}

void UIManager::render(Scene& scene) 
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();

    ImGui::NewFrame();

    renderToolWidget(scene);
    renderLogWidget();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UIManager::renderToolWidget(Scene& scene) 
{
    ImGui::SetNextWindowPos(ImVec2(WidgetSettings::TOOL_WINDOW_POS_X, WidgetSettings::TOOL_WINDOW_POS_Y));
    ImGui::SetNextWindowSize(ImVec2(WidgetSettings::TOOL_WINDOW_WIDTH, WidgetSettings::TOOL_WINDOW_HEIGHT));
    ImGui::Begin("Application Toolbox", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize);

    renderAboutSection();
    renderObjectSection(scene);
    renderSelectionSection(scene);
    renderTransformSection(scene);
    renderLightingSection(scene);
    renderCameraSection(scene);
    renderPropertiesSection();
    renderFpsOverlay();
    
    ImGui::End();
}

void UIManager::renderLogWidget()
{
    ImGui::SetNextWindowPos(ImVec2(WidgetSettings::LOG_WINDOW_POS_X, WidgetSettings::LOG_WINDOW_POS_Y));
    ImGui::SetNextWindowSize(ImVec2(WidgetSettings::LOG_WINDOW_WIDTH, WidgetSettings::LOG_WINDOW_HEIGHT));
    ImGui::Begin("Application Logs", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysVerticalScrollbar | ImGuiWindowFlags_NoResize);
    
    for (const auto& log : Environment::LogMessages) 
    {
        ImGui::TextWrapped("%s", log.c_str());
    }
    
    if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
    {
        ImGui::SetScrollHereY(1.0f);
    }
    
    ImGui::End();
}

void UIManager::renderAboutSection()
{
    if (ImGui::CollapsingHeader("About"))
	{
		ImGui::SeparatorText("ABOUT THIS APP:");
		ImGui::BulletText("Title: Scene Visualizer Engine");
		ImGui::BulletText("Version: 1.0.0");
		ImGui::BulletText("Author: Abramov Ilya");
		ImGui::BulletText("License: MIT");

		ImGui::SeparatorText("GLFW:");
		ImGui::BulletText("Version: 3.3.8");
		ImGui::BulletText("Controls: Keyboard Controls");

		ImGui::SeparatorText("OpenGL:");
		ImGui::BulletText("Version: 3.3.0");
		ImGui::BulletText("Profile: CORE_PROFILE");
		ImGui::BulletText("Bindings: OpenGL Loading Library (GLAD)");
	}
}

void UIManager::renderObjectSection(Scene& scene)
{
    if (ImGui::CollapsingHeader("Create Model"))
    {
        ImGui::SeparatorText("Model Type");

        static const char* modelRepository[] = { "Asteroid", "Carrier", "Cruiser", "Box", "Plane" };
        static int currentModelIndex = -1;
        static char sceneObjectName[128] = "";

        ImGui::SetNextItemWidth(WidgetSettings::TOOL_WINDOW_WIDTH * 0.925);
        ImGui::ListBox("##ModelType", &currentModelIndex, modelRepository, IM_ARRAYSIZE(modelRepository));

        ImGui::SeparatorText("Object Name");

        ImGui::SetNextItemWidth(WidgetSettings::TOOL_WINDOW_WIDTH * 0.925);
        ImGui::InputText("##ObjectName", sceneObjectName, IM_ARRAYSIZE(sceneObjectName));
        
        if (ImGui::Button("Add Object", ImVec2(ImGui::GetContentRegionAvail().x, 0)))
        {
            if (strlen(sceneObjectName) == 0)
            {
                Environment::AddLog("[ERROR]::Object name cannot be empty...");
            } 
            
            else 
            {
                std::string modelFilename = std::string(modelRepository[currentModelIndex]) + ".obj";

                std::string modelPath = FilePaths::ModelRepository + modelRepository[currentModelIndex] + "/" + modelFilename;
                std::string texturePath = FilePaths::ModelRepository + modelRepository[currentModelIndex];

                Object3D* object = ResourceManager::loadObject(modelPath);
                object->loadTextures(texturePath);
                SceneObject* newObject = scene.addObject(sceneObjectName, object);
                
                if (newObject) 
                {
                    scene.setSelectedObject(newObject);
                    Environment::AddLog("[INFO]::Created object: " + std::string(sceneObjectName));
                } 
                
                else 
                {
                    Environment::AddLog("[ERROR]::Object with this name already exists...");
                }
            }
        }
    }
}

void UIManager::renderSelectionSection(Scene& scene)
{
    if (ImGui::CollapsingHeader("Scene Objects"))
	{
        auto objects = scene.getObjectNames();
        static int currentObjIndex = -1;
            
        if (objects.empty()) 
        {
            ImGui::Text("No objects in scene...");
        } 
        
        else 
        {
            ImGui::SeparatorText("Objects in Scene");
            ImGui::SetNextItemWidth(WidgetSettings::TOOL_WINDOW_WIDTH * 0.925);

            if (ImGui::ListBox("##Objects", &currentObjIndex, objects.data(), static_cast<int>(objects.size()))) 
            {
                scene.setSelectedObject(objects[currentObjIndex]);
            }
            
            if (currentObjIndex >= 0) 
            {
                if (ImGui::Button("Select", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0))) 
                {
                    scene.setSelectedObject(objects[currentObjIndex]);
                }

                ImGui::SameLine();

                if (ImGui::Button("Delete", ImVec2(ImGui::GetContentRegionAvail().x, 0))) 
                {
                    std::string name = objects[currentObjIndex];
                    if (scene.removeObject(name)) 
                    {
                        Environment::AddLog("[INFO]::Deleted object: " + name);
                        currentObjIndex = -1;
                    }
                }
            }
        }
    }
}

void UIManager::renderLightingSection(Scene& scene) 
{
    if (ImGui::CollapsingHeader("Lighting System")) 
    {
        static glm::vec3 lightColor(1.0f);
        static glm::vec3 lightPos(0.0f);
        
        ImGui::SeparatorText("Create New Light");
        ImGui::InputFloat3("Position", glm::value_ptr(lightPos));
        ImGui::ColorEdit3("Color", glm::value_ptr(lightColor));
        
        if (ImGui::Button("Add Light", ImVec2(ImGui::GetContentRegionAvail().x, 0))) 
        {
            auto light = scene.addLight(lightPos, lightColor);

            if (light) 
            {
                Environment::AddLog("[INFO]::Added new light source");
            } 
            
            else
            {
                Environment::AddLog("[Error]::Max lights reached");
            }
        }
        
        ImGui::SeparatorText("Existing Lights");
        
        for (size_t i = 0; i < scene.getLightCount(); ++i) 
        {
            auto light = scene.getLight(i);
            
            ImGui::PushID(static_cast<int>(i));
            
            if (ImGui::TreeNodeEx(("Light " + std::to_string(i)).c_str(), ImGuiTreeNodeFlags_DefaultOpen)) 
            {
                glm::vec3 pos = light->getPosition();

                float posX = pos.x;
                float posY = pos.y;
                float posZ = pos.z;

                ImGui::InputFloat("Position X", &posX, 0.5f, 50.0f);
                ImGui::InputFloat("Position Y", &posY, 0.5f, 50.0f); 
                ImGui::InputFloat("Position Z", &posZ, 0.5f, 50.0f); 

                light->setPosition(glm::vec3(posX, posY, posZ));
                
                glm::vec3 color = light->getColor();

                if (ImGui::ColorEdit3("Color", glm::value_ptr(color))) 
                {
                    light->setColor(color);
                }
                
                if (ImGui::Button("Delete", ImVec2(ImGui::GetContentRegionAvail().x, 0))) 
                {
                    scene.removeLight(i);

                    ImGui::TreePop();
                    ImGui::PopID();

                    break;
                }
                
                ImGui::TreePop();
            }
            
            ImGui::PopID();
        }
    }
}

void UIManager::renderCameraSection(Scene& scene) 
{
    if (ImGui::CollapsingHeader("Camera System")) 
    {
        if (ImGui::TreeNodeEx("Camera Management", ImGuiTreeNodeFlags_DefaultOpen)) 
        {
            static char newCameraName[128] = "";

            ImGui::SetNextItemWidth(WidgetSettings::TOOL_WINDOW_WIDTH * 0.87);
            ImGui::InputText("##NewCameraName", newCameraName, IM_ARRAYSIZE(newCameraName));
            
            if (ImGui::Button("Add Camera", ImVec2(-1, 0))) 
            {
                if (strlen(newCameraName) == 0) 
                {
                    Environment::AddLog("[ERROR]::Camera name cannot be empty");
                } 
                
                else 
                {
                    SceneCamera* newCamera = scene.addCamera(newCameraName);
                    if (newCamera) 
                    {
                        Environment::AddLog("[INFO]::Created camera: " + std::string(newCameraName));
                    } 
                    
                    else 
                    {
                        Environment::AddLog("[ERROR]::Camera with this name already exists...");
                    }
                }
            }
            
            ImGui::TreePop();
        }
        
        if (ImGui::TreeNodeEx("Camera Control", ImGuiTreeNodeFlags_DefaultOpen)) 
        {
            auto cameras = scene.getCameraNames();
            static int currentCamIndex = 0;
            
            if (cameras.empty()) 
            {
                ImGui::Text("No cameras in scene...");
            } 
            
            else 
            {
                ImGui::SeparatorText("Available Cameras");
                ImGui::SetNextItemWidth(WidgetSettings::TOOL_WINDOW_WIDTH * 0.87);

                if (ImGui::ListBox("##Cameras", &currentCamIndex, cameras.data(), static_cast<int>(cameras.size()))) 
                {
                    scene.setActiveCamera(cameras[currentCamIndex]);
                }
            
                if (currentCamIndex >= 0) 
                {
                    if (ImGui::Button("Select", ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 0))) 
                    {
                        scene.setActiveCamera(cameras[currentCamIndex]);
                    }

                    ImGui::SameLine();

                    if (ImGui::Button("Delete", ImVec2(ImGui::GetContentRegionAvail().x, 0))) 
                    {
                        std::string name = cameras[currentCamIndex];
                        if (name != "Default Camera" && scene.removeCamera(name)) 
                        {
                            Environment::AddLog("[INFO]::Deleted camera: " + name);
                            currentCamIndex = -1;
                        }
                    }
                }   
                
                SceneCamera* camera = scene.getActiveCamera();

                if (camera) 
                {
                    ImGui::SeparatorText("Camera State");
                    bool cameraEnabled = camera->isEnabled();

                    if (ImGui::Checkbox("Enable Camera", &cameraEnabled)) 
                    {
                        if (cameraEnabled) 
                            camera->enableCamera();
                        else 
                            camera->disableCamera();
                    }
                    
                    glm::vec3 pos = camera->getPositionVector();

                    ImGui::SeparatorText("Camera Position");
                    ImGui::Text("X: %.2f, Y: %.2f, Z: %.2f", pos.x, pos.y, pos.z);
                    
                    ImGui::SeparatorText("Camera Movement");
                    ImGui::BulletText("[W] FORWARD");
                    ImGui::BulletText("[S] BACKWARD");
                    ImGui::BulletText("[A] LEFT");
                    ImGui::BulletText("[D] RIGHT");
                    ImGui::BulletText("[Q] MOVE UP");
                    ImGui::BulletText("[E] MOVE DOWN");
                    
                    ImGui::SeparatorText("Camera Rotation");
                    ImGui::BulletText("[LEFT ARROW]  LEFT");
                    ImGui::BulletText("[RIGHT ARROW] RIGHT");
                    ImGui::BulletText("[UP ARROW]    UP");
                    ImGui::BulletText("[DOWN ARROW]  DOWN");
                }
            }

            ImGui::TreePop();
        }
    }
}

void UIManager::renderTransformSection(Scene& scene)
{
    if (ImGui::CollapsingHeader("Transformations")) 
    {
        auto selectedObject = scene.getSelectedObject();

        if (selectedObject) 
        {
            glm::vec3 objectPosition = selectedObject->getPosition();
            glm::vec3 objectScaling = selectedObject->getScale();
            glm::vec3 objectRotation = selectedObject->getRotation();

            float x_coord = objectPosition.x;
            float y_coord = objectPosition.y;
            float z_coord = objectPosition.z;

            ImGui::SeparatorText("Translation");
            ImGui::InputFloat("X Axis", &x_coord, 0.5f, 50.0f);
            ImGui::InputFloat("Y Axis", &y_coord, 0.5f, 50.0f);
            ImGui::InputFloat("Z Axis", &z_coord, 0.5f, 50.0f);

            float x_scale = objectScaling.x;
            float y_scale = objectScaling.y;
            float z_scale = objectScaling.z;

            ImGui::SeparatorText("Scaling");
            ImGui::SliderFloat("X Axis Scale", &x_scale, 0.0f, 100.0f);
            ImGui::SliderFloat("Y Axis Scale", &y_scale, 0.0f, 100.0f);
            ImGui::SliderFloat("Z Axis Scale", &z_scale, 0.0f, 100.0f);

            float x_rotation = objectRotation.x;
            float y_rotation = objectRotation.y;
            float z_rotation = objectRotation.z;

            ImGui::SeparatorText("Rotation");
            ImGui::SliderFloat("Pitch", &z_rotation, 0.0f, 360.0f);
            ImGui::SliderFloat("Yaw", &y_rotation, 0.0f, 360.0f);
            ImGui::SliderFloat("Roll", &x_rotation, 0.0f, 360.0f);

            glm::vec3 newPositionVector = glm::vec3(x_coord, y_coord, z_coord);
            glm::vec3 newRotationVector = glm::vec3(x_rotation, y_rotation, z_rotation);
            glm::vec3 newScalingVector = glm::vec3(x_scale, y_scale, z_scale);

            selectedObject->setPosition(newPositionVector);
            selectedObject->setRotation(newRotationVector);
            selectedObject->setScale(newScalingVector);
        } 
        
        else 
        {
            ImGui::Text("No object selected...");
        }
    }
}

void UIManager::renderPropertiesSection()
{
    if (ImGui::CollapsingHeader("Scene Properties"))
	{
		ImGui::SeparatorText("Background Color");
		ImGui::ColorEdit3("Scene Colors", Environment::BackgroundColor, ImGuiColorEditFlags_PickerHueWheel);

		ImGui::SeparatorText("Rendering Mode");
		ImGui::Checkbox("Wireframe Rendering", &Environment::WireframeMode);
		ImGui::Checkbox("Enable V-Sync", &Environment::Vsync);
		ImGui::Checkbox("Enable Z-Buffer", &Environment::DepthTesting);

		ImGui::SeparatorText("Extensions");
		ImGui::Checkbox("Enable FPS Counter", &Environment::FpsCounter);
		ImGui::Checkbox("Enable Skybox", &Environment::Skybox);
		ImGui::Checkbox("Enable Grid", &Environment::Grid);
		ImGui::Separator();
	}
}

void UIManager::renderFpsOverlay()
{
    if (Environment::FpsCounter)
	{
		ImGui::SetNextWindowPos(ImVec2(WidgetSettings::WINDOW_WIDTH - 150.0, 0.0));
		ImGui::SetNextWindowSize(ImVec2(150.0, 70.0));
		ImGui::SetNextWindowBgAlpha(0.35f);

		ImGui::Begin("FPS Overlay", NULL, ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse |  ImGuiWindowFlags_NoResize);
		ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
        ImGui::Text("Cleared Buffers [X]");
		ImGui::Separator();
        ImGui::End();
	}
}