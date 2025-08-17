#ifndef SCENE_CAMERA_H
#define SCENE_CAMERA_H

#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum MovementDirections
{
    MOVE_FORWARD,
    MOVE_BACKWARD,
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN
};

enum AngularDirections
{
    ANGLE_LEFT,
    ANGLE_RIGHT,
    ANGLE_UP,
    ANGLE_DOWN
};

class SceneCamera
{
private:
    bool moveForward = false;
    bool moveBackward = false;
    bool moveLeft = false;
    bool moveRight = false;
    bool moveUp = false;
    bool moveDown = false;

    bool angleLeft = false;
    bool angleRight = false;
    bool angleUp = false;
    bool angleDown = false;

    glm::vec3 positionVector;
    glm::vec3 frontVector;
    glm::vec3 worldUpVector;

    float cameraSpeed;
    float cameraSensitivity;

    float pitchAngle;
    float yawAngle;

    bool active;

    std::string name;

public:
    SceneCamera(const std::string& name);
    ~SceneCamera();

    void setMovementState(MovementDirections direction, bool state);
    void setAngularState(AngularDirections direction, bool state);

    void updateCamera(float deltaTime);

    glm::mat4 getViewMatrix() const;

    float getCameraSpeed() const { return cameraSpeed; }
    float getCameraSensitivity() const { return cameraSensitivity; }

    float getPitchAngle() const { return pitchAngle; }
    float getYawAngle() const { return yawAngle; }

    void setCameraSpeed(float newSpeed) { cameraSpeed = newSpeed; }
    void setCameraSensitivity(float newSensitivity) { cameraSensitivity = newSensitivity; }

    void setPitch(float angle) { pitchAngle = angle; }
    void setYaw(float angle) { yawAngle = angle; }

    void enableCamera() { active = true; }
    void disableCamera() { active = false; }
    
    void setPositionVector(const glm::vec3& newPosition) { positionVector = newPosition; }
    const glm::vec3& getPositionVector() const { return positionVector; }

    bool isEnabled() const { return active; }

    const std::string& getName() const { return name; }
};

#endif