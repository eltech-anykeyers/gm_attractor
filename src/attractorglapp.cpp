#include <attractorglapp.hpp>

std::shared_ptr<Camera> AttractorGLApp::sCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 10.0f));
std::shared_ptr<FpsManager> AttractorGLApp::sFpsManager = std::make_shared<FpsManager>();

AttractorGLApp::AttractorGLApp(GLint width, GLint height, const std::string& title)
    : IGLApp(width, height, title)
{
    configure();
}

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

    mMainShader = std::make_shared<Shader>("shaders/main.vs", "shaders/main.fs");

    float vert[] =
    {
        0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f
    };

    glGenVertexArrays(1, &mVertexArrayObject);
    glGenBuffers(1, &mVertexBufferObject);

    glBindVertexArray(mVertexArrayObject);

    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vert), vert, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), static_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    mProjectionMat = glm::perspective(glm::radians(mFieldOfView),
                                      static_cast<float>(mWindowWidth) /
                                      static_cast<float>(mWindowHeight),
                                      mNearDistance, mFarDistance);
    mViewMat = sCamera->getViewMatrix();
}

void AttractorGLApp::mainLoop()
{
    mMainShader->use();
    mMainShader->setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));

    while (!glfwWindowShouldClose(mWindow))
    {
        mTimeDelta = sFpsManager->enforceFPS();
        processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mViewMat = sCamera->getViewMatrix();

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.1f, 0.5f, 0.0f));

        setMVPViaVec(std::move(mProjectionMat * mViewMat * model));

        glBindVertexArray(mVertexArrayObject);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(mWindow);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &mVertexArrayObject);
    glDeleteBuffers(1, &mVertexBufferObject);

    terminate();
}

void AttractorGLApp::processInput()
{
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);

    if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
        sCamera->processKeyboard(FORWARD, mTimeDelta);
    if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
        sCamera->processKeyboard(BACKWARD, mTimeDelta);
    if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
        sCamera->processKeyboard(LEFT, mTimeDelta);
    if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
        sCamera->processKeyboard(RIGHT, mTimeDelta);
}

void AttractorGLApp::setMVPViaVec(const glm::mat4& mvp) const
{
    /// Only for main.vs.
    mMainShader->setVec4("trans_0", mvp[0][0], mvp[0][1], mvp[0][2], mvp[0][3]);
    mMainShader->setVec4("trans_1", mvp[1][0], mvp[1][1], mvp[1][2], mvp[1][3]);
    mMainShader->setVec4("trans_2", mvp[2][0], mvp[2][1], mvp[2][2], mvp[2][3]);
    mMainShader->setVec4("trans_3", mvp[3][0], mvp[3][1], mvp[3][2], mvp[3][3]);
}

void AttractorGLApp::setFrameBufferSizeCallback(void (* func)(GLFWwindow*, int, int))
{
    glfwSetFramebufferSizeCallback(mWindow, func);
}

void AttractorGLApp::setCursorPosCallback(void (* func)(GLFWwindow*, double, double))
{
    glfwSetCursorPosCallback(mWindow, func);
}
