#ifndef FPSMANAGER_HPP
#define FPSMANAGER_HPP

class FpsManager
{
public:
    FpsManager(int targetFps);

    void init(double targetFps);
    double enforceFps();

    inline double getFrameDuration()
    {
        return frameDuration;
    }

    inline int getTargetFps() const
    {
        return mTargetFps;
    }

    void setTargetFps(int fpsLimit)
    {
        if (fpsLimit < MIN_TARGET_FPS)
        {
            fpsLimit = MIN_TARGET_FPS;
        }
        else if (fpsLimit > MAX_TARGET_FPS)
        {
            fpsLimit = MAX_TARGET_FPS;
        }

        mTargetFps = fpsLimit;
        mTargetFrameDuration = 1.0 / mTargetFps;
    }

private:
    static const double MIN_TARGET_FPS;
    static const double MAX_TARGET_FPS;

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
