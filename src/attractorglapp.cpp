#include <attractorglapp.hpp>
#include <GL/gl.h>

void AttractorGLApp::render()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(mWindow);
    glfwPollEvents();
}
