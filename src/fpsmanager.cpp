#include <stdexcept>
#include <unistd.h>

#include <fpsmanager.hpp>
#include <GLFW/glfw3.h>

const double FpsManager::MIN_TARGET_FPS = 20;
const double FpsManager::MAX_TARGET_FPS = 60;

FpsManager::FpsManager(int targetFps)
{
    init(targetFps);
}


void FpsManager::init(double targetFps)
{
    setTargetFps(targetFps);

    mFrameCount = 0;
    mCurrentFps = 0.0;
    mSleepDuration = 0.0;
    mFrameStartTime = glfwGetTime();
    mFrameEndTime = mFrameStartTime + 1;
    mFrameDuration = 1;
}

double FpsManager::enforceFPS()
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
