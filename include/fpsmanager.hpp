#ifndef FPSMANAGER_HPP
#define FPSMANAGER_HPP

#include <unistd.h>

#include <GLFW/glfw3.h>

class FpsManager
{
public:
    FpsManager(int targetFps = FpsManager::MIN_TARGET_FPS);

    double enforceFPS();

    inline double getFrameDuration();

    inline int getTargetFps() const;
    void setTargetFps(int fpsLimit);

private:
    constexpr static const double MIN_TARGET_FPS = 20;
    constexpr static const double MAX_TARGET_FPS = 60;

    double mFrameStartTime;
    double mFrameEndTime;
    double mFrameDuration;

    double mTargetFps;
    double mCurrentFps;
    int mFrameCount;

    double mTargetFrameDuration;
    double mSleepDuration;
};

#endif // FPSMANAGER_HPP
