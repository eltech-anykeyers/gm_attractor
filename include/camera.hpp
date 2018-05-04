#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera
{
public:
    glm::vec3 mPosition;
    glm::vec3 mFront;
    glm::vec3 mUp;
    glm::vec3 mRight;
    glm::vec3 mWorldUp;

    GLfloat mYaw;
    GLfloat mPitch;

    GLfloat mMovementSpeed;
    GLfloat mMouseSensitivity;
    GLfloat mZoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
           GLfloat yaw          = DFLT_YAW,
           GLfloat pitch        = DFLT_PITCH);

    void processKeyboard(CameraMovement direction, GLfloat deltaTime);

    void processMouseMovement(GLfloat xPos, GLfloat yPos,
                              GLboolean constrainPitch = true);

    void processMouseScroll(GLfloat yOffset);

    glm::mat4 getViewMatrix();

private:
    constexpr static const GLfloat DFLT_YAW         = -90.0f;
    constexpr static const GLfloat DFLT_PITCH       =  0.0f;
    constexpr static const GLfloat DFLT_SPEED       =  2.5f;
    constexpr static const GLfloat DFLT_SENSITIVITY =  0.1f;
    constexpr static const GLfloat DFLT_ZOOM        =  45.0f;

    GLdouble mLastXPos;
    GLdouble mLastYPos;

    GLboolean mFirstMouse = true;

    void updateCameraVectors();

};

#endif // CAMERA_HPP
