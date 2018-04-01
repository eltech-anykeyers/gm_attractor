#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <vector>

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

    float mYaw;
    float mPitch;

    float mMovementSpeed;
    float mMouseSensitivity;
    float mZoom;

    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
           glm::vec3 up       = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw          = DFLT_YAW,
           float pitch        = DFLT_PITCH)
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

    void processKeyboard(CameraMovement direction, float deltaTime)
    {
        float velocity = mMovementSpeed * deltaTime;

        if (direction == FORWARD)  mPosition += mFront * velocity;
        if (direction == BACKWARD) mPosition -= mFront * velocity;
        if (direction == LEFT)     mPosition -= mRight * velocity;
        if (direction == RIGHT)    mPosition += mRight * velocity;
    }

    void processMouseMovement(float xPos, float yPos,
                              GLboolean constrainPitch = true)
    {
        if (mFirstMouse)
        {
            mLastXPos = xPos;
            mLastYPos = yPos;
            mFirstMouse = false;
        }

        float xOffset = xPos - mLastXPos;
        /// Reversed since y-coordinates go from bottom to top.
        float yOffset = mLastYPos - yPos;

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

    void processMouseScroll(float yOffset)
    {
        if (mZoom >= 1.0f && mZoom <= 45.0f) mZoom -= yOffset;
        if (mZoom <= 1.0f)                   mZoom =  1.0f;
        if (mZoom >= 45.0f)                  mZoom =  45.0f;
    }

    glm::mat4 getViewMatrix()
    {
        return glm::lookAt(mPosition, mPosition + mFront, mUp);
    }

private:
    constexpr static const float DFLT_YAW         = -90.0f;
    constexpr static const float DFLT_PITCH       =  0.0f;
    constexpr static const float DFLT_SPEED       =  2.5f;
    constexpr static const float DFLT_SENSITIVITY =  0.1f;
    constexpr static const float DFLT_ZOOM        =  45.0f;

    double mLastXPos;
    double mLastYPos;

    bool mFirstMouse = true;

    void updateCameraVectors()
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
};

#endif // CAMERA_HPP
