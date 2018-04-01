#include <attractorglapp.hpp>

std::shared_ptr<Camera> AttractorGLApp::sCamera = std::make_shared<Camera>();
std::shared_ptr<FpsManager> AttractorGLApp::sFpsManager = std::make_shared<FpsManager>(20);

void AttractorGLApp::configure()
{
    IGLApp::configure();

    setFrameBufferSizeCallback([](GLFWwindow*, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    setCursorPosCallback([](GLFWwindow*, double xPos, double yPos)
    {
        sCamera->processMouseMovement(xPos, yPos);
    });

}

void AttractorGLApp::mainLoop()
{
    while (!glfwWindowShouldClose(mWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    terminate();
}

void AttractorGLApp::setFrameBufferSizeCallback(void (* func)(GLFWwindow*, int, int))
{
    glfwSetFramebufferSizeCallback(mWindow, func);
}

void AttractorGLApp::setCursorPosCallback(void (* func)(GLFWwindow*, double, double))
{
    glfwSetCursorPosCallback(mWindow, func);
}
