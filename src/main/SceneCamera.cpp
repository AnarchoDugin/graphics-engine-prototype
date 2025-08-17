#include <general/SceneCamera.h>

SceneCamera::SceneCamera(const std::string& name) : 
    positionVector(0.0f, 4.25f, 20.0f),
    frontVector(0.0f, 0.0f, -1.0f),
    worldUpVector(0.0f, 1.0f, 0.0f),
    cameraSpeed(2.5f),
    cameraSensitivity(45.0f),
    pitchAngle(0.0f),
    yawAngle(-90.0f),
    active(false),
    name(name)
{}

SceneCamera::~SceneCamera() {}

void SceneCamera::setMovementState(MovementDirections direction, bool state)
{
    switch (direction)
    {
    case MOVE_FORWARD: 
        moveForward = state;
        break;

    case MOVE_BACKWARD:
        moveBackward = state;
        break;

    case MOVE_LEFT: 
        moveLeft = state; 
        break;

    case MOVE_RIGHT: 
        moveRight = state;
        break;

    case MOVE_UP: 
        moveUp = state; 
        break;

    case MOVE_DOWN:
        moveDown = state;
        break;
    }
}

void SceneCamera::setAngularState(AngularDirections direction, bool state)
{
    switch (direction)
    {
        case ANGLE_LEFT: 
            angleLeft = state;
            break;

        case ANGLE_RIGHT:
            angleRight = state;
            break;

        case ANGLE_UP:
            angleUp = state;
            break;

        case ANGLE_DOWN:
            angleDown = state; 
            break;
    }
}

void SceneCamera::updateCamera(float deltaTime) 
{
    if (moveForward) positionVector += cameraSpeed * deltaTime * frontVector;
    if (moveBackward) positionVector -= cameraSpeed * deltaTime * frontVector;

    if (moveLeft) positionVector -= cameraSpeed * deltaTime * glm::normalize(glm::cross(frontVector, worldUpVector));
    if (moveRight) positionVector += cameraSpeed * deltaTime * glm::normalize(glm::cross(frontVector, worldUpVector));

    if (moveUp) positionVector += cameraSpeed * deltaTime * worldUpVector;
    if (moveDown) positionVector -= cameraSpeed * deltaTime * worldUpVector;

    if (angleLeft) yawAngle -= cameraSensitivity * deltaTime;
    if (angleRight) yawAngle += cameraSensitivity * deltaTime;
    
    if (angleUp) pitchAngle += cameraSensitivity * deltaTime;
    if (angleDown) pitchAngle -= cameraSensitivity * deltaTime;

    if (pitchAngle > 89.0f) pitchAngle = 89.0f;
    if (pitchAngle < -89.0f) pitchAngle = -89.0f;

    glm::vec3 newCameraDirection;
    
    newCameraDirection.x = cos(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));
    newCameraDirection.y = sin(glm::radians(pitchAngle));
    newCameraDirection.z = sin(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle));

    frontVector = glm::normalize(newCameraDirection);
}

glm::mat4 SceneCamera::getViewMatrix() const
{
    return glm::lookAt(positionVector, positionVector + frontVector, worldUpVector);
}

