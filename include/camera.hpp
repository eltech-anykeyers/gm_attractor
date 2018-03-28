#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <functional>

#include <vec3.hpp>

class Camera
{
public:
    static const double TO_RADS;

    bool IsHoldingForward;
    bool IsHoldingBackward;
    bool IsHoldingLeftStrafe;
    bool IsHoldingRightStrafe;

    Camera(float windowWidth, float windowHeight);

    void move(double deltaTime);
    void handleMouseMove(int mouseX, int mouseY);

    const double toRads(const double& angleInDegrees) const
    { return angleInDegrees * TO_RADS; }

    float getPitchSensitivity()            { return mPitchSensitivity;  }
    void  setPitchSensitivity(float value) { mPitchSensitivity = value; }

    float getYawSensitivity()              { return mYawSensitivity;    }
    void  setYawSensitivity(float value)   { mYawSensitivity   = value; }

    Vec3<double> getPosition() const { return mPosition;        }
    double getXPosition()      const { return mPosition.getX(); }
    double getYPosition()      const { return mPosition.getY(); }
    double getZPosition()      const { return mPosition.getZ(); }

    Vec3<double> getRotation() const { return mRotation;        }
    double getXRotation()      const { return mRotation.getX(); }
    double getYRotation()      const { return mRotation.getY(); }
    double getZRotation()      const { return mRotation.getZ(); }

    void setMouseSetterCallback(const std::function<void(double, double)>& callback);

private:
    Vec3<double> mPosition;
    Vec3<double> mRotation;
    Vec3<double> mSpeed;

    double mMovementSpeedFactor;
    double mPitchSensitivity;
    double mYawSensitivity;

    int mWindowWidth;
    int mWindowHeight;
    int mWindowMidX;
    int mWindowMidY;

    std::function<void(double, double)> mMouseSetterCallback;

    void initCamera();
};

#endif // CAMERA_HPP
