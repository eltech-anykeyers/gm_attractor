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

void AttractorGLApp::processInput()
{
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);

    if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
        sCamera->processKeyboard(FORWARD, sFpsManager->enforceFPS());
    if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
        sCamera->processKeyboard(BACKWARD, sFpsManager->enforceFPS());
    if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
        sCamera->processKeyboard(LEFT, sFpsManager->enforceFPS());
    if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
        sCamera->processKeyboard(RIGHT, sFpsManager->enforceFPS());
}

void AttractorGLApp::mainLoop()
{
    while (!glfwWindowShouldClose(mWindow))
    {
        processInput();

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
