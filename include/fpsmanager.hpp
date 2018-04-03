#ifndef FPSMANAGER_HPP
#define FPSMANAGER_HPP

#include <unistd.h>

#include <GLFW/glfw3.h>

class FpsManager
{
public:
    FpsManager(int targetFps = FpsManager::MIN_TARGET_FPS)
        : mFrameCount(0)
        , mCurrentFps(0.0)
        , mSleepDuration(0.0)
        , mFrameStartTime(glfwGetTime())
        , mFrameEndTime(mFrameStartTime + 1)
        , mFrameDuration(1)
    {
        setTargetFps(targetFps);
    }

    double enforceFPS()
    {
        /// Get current time.
        mFrameEndTime = glfwGetTime();

        /**
         * Calculate how long it's been since
         * the mFrameStartTime was set (at the end of this method).
         */
        mFrameDuration = mFrameEndTime - mFrameStartTime;

        /**
         * Calculate how long we should sleep for
         * to stick to our target frame rate.
         */
        mSleepDuration = mTargetFrameDuration - mFrameDuration;

        /// If we're running faster than our target duration, sleep until we catch up!
        if (mSleepDuration > 0.0) usleep(mSleepDuration);

        /**
         * Reset the frame start time to be now - this means
         * we only need put a single call into the main loop.
         */
        mFrameStartTime = glfwGetTime();

        /**
         * Pass back our total frame duration (including any sleep and
         * the time it took to run this function) to be used as
         * our deltaTime value.
         */
        return mFrameDuration + (mFrameStartTime - mFrameEndTime);
    }

    inline double getFrameDuration()
    {
        return mFrameDuration;
    }

    inline int getTargetFps() const
    {
        return mTargetFps;
    }

    void setTargetFps(int fpsLimit)
    {
        if      (fpsLimit < MIN_TARGET_FPS) fpsLimit = MIN_TARGET_FPS;
        else if (fpsLimit > MAX_TARGET_FPS) fpsLimit = MAX_TARGET_FPS;

        mTargetFps = fpsLimit;
        mTargetFrameDuration = 1.0 / mTargetFps;
    }

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
