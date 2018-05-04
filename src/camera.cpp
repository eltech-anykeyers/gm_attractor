#include <camera.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    : mFront(glm::vec3(0.0f, 0.0f, -1.0f))
    , mMovementSpeed(DFLT_SPEED)
    , mMouseSensitivity(DFLT_SENSITIVITY)
    , mZoom(DFLT_ZOOM)
{
    mPosition = position;
    mWorldUp  = up;
    mYaw      = yaw;
    mPitch    = pitch;

    updateCameraVectors();
}

void Camera::processKeyboard(CameraMovement direction, GLfloat deltaTime)
{
    GLfloat velocity = mMovementSpeed * deltaTime;

    if (direction == FORWARD)  mPosition += mFront * velocity;
    if (direction == BACKWARD) mPosition -= mFront * velocity;
    if (direction == LEFT)     mPosition -= mRight * velocity;
    if (direction == RIGHT)    mPosition += mRight * velocity;
}

void Camera::processMouseMovement(
        GLfloat xPos, GLfloat yPos, GLboolean constrainPitch)
{
    if (mFirstMouse)
    {
        mLastXPos = xPos;
        mLastYPos = yPos;
        mFirstMouse = false;
    }

    GLfloat xOffset = xPos - mLastXPos;
    /// Reversed since y-coordinates go from bottom to top.
    GLfloat yOffset = mLastYPos - yPos;

    mLastXPos = xPos;
    mLastYPos = yPos;

    xOffset *= mMouseSensitivity;
    yOffset *= mMouseSensitivity;

    mYaw   += xOffset;
    mPitch += yOffset;

    if (constrainPitch)
    {
        if (mPitch >  89.0f) mPitch =  89.0f;
        if (mPitch < -89.0f) mPitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::processMouseScroll(GLfloat yOffset)
{
    if (mZoom >= 1.0f && mZoom <= 45.0f) mZoom -= yOffset;
    if (mZoom <= 1.0f)                   mZoom =  1.0f;
    if (mZoom >= 45.0f)                  mZoom =  45.0f;
}

glm::mat4 Camera::getViewMatrix()
{
    return glm::lookAt(mPosition, mPosition + mFront, mUp);
}

void Camera::updateCameraVectors()
{
    /// Calculate the new Front vector
    glm::vec3 front;

    front.x = cos(glm::radians(mYaw)) * cos(glm::radians(mPitch));
    front.y = sin(glm::radians(mPitch));
    front.z = sin(glm::radians(mYaw)) * cos(glm::radians(mPitch));

    mFront = glm::normalize(front);
    /**
     * Normalize the vectors, because their length gets closer
     * to 0 the more you look up or down which results in slower movement.
     */
    mRight = glm::normalize(glm::cross(mFront, mWorldUp));
    mUp    = glm::normalize(glm::cross(mRight, mFront));
}
