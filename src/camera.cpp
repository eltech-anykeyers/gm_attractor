#include <cmath>

#include <camera.hpp>
#include <GLFW/glfw3.h>

const double Camera::TO_RADS = M_PI / 180.0;

Camera::Camera(float windowWidth, float windowHeight)
{
    initCamera();

    mWindowWidth  = windowWidth;
    mWindowHeight = windowHeight;

    mWindowMidX = mWindowWidth / 2.0f;
    mWindowMidY = mWindowHeight / 2.0f;

    if (mMouseSetterCallback) mMouseSetterCallback(mWindowMidX, mWindowMidY);
    else throw std::runtime_error("mMouseSetterCallback isn't set");
}

void Camera::initCamera()
{
    mPosition.zero();
    mRotation.zero();
    mSpeed.zero();

    mMovementSpeedFactor = 100.0;

    mPitchSensitivity = 0.2;
    mYawSensitivity   = 0.2;

    IsHoldingForward     = false;
    IsHoldingBackward    = false;
    IsHoldingLeftStrafe  = false;
    IsHoldingRightStrafe = false;
}

void Camera::handleMouseMove(int mouseX, int mouseY)
{
    double horizMovement = (mouseX - mWindowMidX + 1) * mYawSensitivity;
    double vertMovement  = (mouseY - mWindowMidX) * mPitchSensitivity;

    mRotation.addX(vertMovement);
    mRotation.addY(horizMovement);

    if (mRotation.getX() < -90)
    {
        mRotation.setX(-90);
    }

    if (mRotation.getX() > 90)
    {
        mRotation.setX(90);
    }

    /* Looking left and right - keep angles in the range 0.0 to 360.0
     * 0 degrees is looking directly down the negative Z axis "North",
     * 90 degrees is "East", 180 degrees is "South", 270 degrees is "West"
     * We can also do this so that our 360 degrees goes -180 through +180
     * and it works the same, but it's probably best to keep our
     * range to 0 through 360 instead of -180 through +180.
     */
    if (mRotation.getY() < 0)
    {
        mRotation.addY(360);
    }
    if (mRotation.getY() > 360)
    {
        mRotation.addY(-360);
    }

    if (mMouseSetterCallback) mMouseSetterCallback(mWindowMidX, mWindowMidY);
    else throw std::runtime_error("mMouseSetterCallback isn't set");
}

void Camera::move(double deltaTime)
{
    Vec3<double> movement;

    double sinXRot = sin(toRads(mRotation.getX()));
    double cosXRot = cos(toRads(mRotation.getX()));

    double sinYRot = sin(toRads(mRotation.getY()));
    double cosYRot = cos(toRads(mRotation.getY()));

    // This cancels out moving on the Z axis when we're looking up or down.
    double pitchLimitFactor = cosXRot;

    if (IsHoldingForward)
    {
        movement.addX(sinYRot * pitchLimitFactor);
        movement.addY(-sinXRot);
        movement.addZ(-cosYRot * pitchLimitFactor);
    }

    if (IsHoldingBackward)
    {
        movement.addX(-sinYRot * pitchLimitFactor);
        movement.addY(sinXRot);
        movement.addZ(cosYRot * pitchLimitFactor);
    }

    if (IsHoldingLeftStrafe)
    {
        movement.addX(-cosYRot);
        movement.addZ(-sinYRot);
    }

    if (IsHoldingRightStrafe)
    {
        movement.addX(cosYRot);
        movement.addZ(sinYRot);
    }

    movement.normalise();

    /* Calculate our value to keep the movement the same
     * speed regardless of the framerate.
     */
    double framerateIndependentFactor = mMovementSpeedFactor * deltaTime;

    movement *= framerateIndependentFactor;
    mPosition += movement;
}

void Camera::setMouseSetterCallback(const std::function<void(double, double)>& callback)
{
    mMouseSetterCallback = callback;
}
