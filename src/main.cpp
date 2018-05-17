#include <attractorglapp.hpp>

int main(int argc, const char** argv)
{
    AttractorGLApp app(640, 480, "Attractor Viewer");

    if (argc == 5) /// Four args to define attractors trajectories and sections.
    {
        app.setFirstAttractorTrajectory  (argv[1]);
        app.setFirstAttractorSection     (argv[2]);
        app.setSecondAttractorTrajectory (argv[3]);
        app.setSecondAttractorSection    (argv[4]);
    }

    app.run();

    return 0;
}
