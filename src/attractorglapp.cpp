#include <attractorglapp.hpp>

void AttractorGLApp::mainLoop()
{
    while (!glfwWindowShouldClose(mWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT);

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    terminate();
}
