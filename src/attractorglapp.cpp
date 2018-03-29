#include <attractorglapp.hpp>
#include <GL/gl.h>

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
