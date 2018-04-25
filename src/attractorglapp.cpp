#include <attractorglapp.hpp>
#include <utils.hpp>

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

    setFrameBufferSizeCallback([](GLFWwindow*, GLint width, GLint height)
    {
        glViewport(0, 0, width, height);
    });

    setCursorPosCallback([](GLFWwindow*, GLdouble xPos, GLdouble yPos)
    {
        sCamera->processMouseMovement(xPos, yPos);
    });

    /// Background.
    mBackgroundShader = std::make_shared<Shader>("shaders/background.vs",
                                                 "shaders/background.fs");
    mBackgroundVerticesSize = 36;
    mBackgroundVertices = new GLfloat[mBackgroundVerticesSize]
    {
        /**  vertices  **/  /**  colors  **/
        -1.0f,  1.0f, 0.0f, 0.7f, 0.7f, 0.7f, /// Top-left.
         1.0f,  1.0f, 0.0f, 0.7f, 0.7f, 0.7f, /// Top-right.
         1.0f, -1.0f, 0.0f, 0.2f, 0.2f, 0.2f, /// Bottom-right.
        -1.0f, -1.0f, 0.0f, 0.2f, 0.2f, 0.2f  /// Bottom-left.
    };
    glGenVertexArrays(1, &mBackgroundArrayObject);
    glGenBuffers(1, &mBackgroundBufferObject);
    glBindVertexArray(mBackgroundArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, mBackgroundBufferObject);
    glBufferData(GL_ARRAY_BUFFER, mBackgroundVerticesSize * sizeof(GLfloat),
                 mBackgroundVertices, GL_STATIC_DRAW);
    glGenBuffers(1, &mBackgroundElementsBufferObject);

    mBackgroundVerticesOrderSize = 6;
    mBackgroundVerticesOrder = new int[mBackgroundVerticesOrderSize]
    {
        0, 1, 2, 2, 3, 0
    };
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mBackgroundElementsBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 mBackgroundVerticesOrderSize * sizeof(GLint),
                 mBackgroundVerticesOrder, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat),
                          reinterpret_cast<void*>(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /// Attractor.
    mAttractorTime = 1;
    mAttractorColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    mAttractorShader = std::make_shared<Shader>("shaders/attractor.vs",
                                                "shaders/attractor.fs");
    try
    {
        auto x = Utils::readPoints("res/x.txt");
        auto y = Utils::readPoints("res/y.txt");
        auto z = Utils::readPoints("res/z.txt");

        mAttractorVerticesSize = 3 * x.size();
        mAttractorVertices = new GLfloat[mAttractorVerticesSize];
        for (GLsizei idx = 0, outerIdx = 0; idx < x.size(); idx += 3, ++outerIdx)
        {
            mAttractorVertices[idx + 0] = x[outerIdx];
            mAttractorVertices[idx + 1] = y[outerIdx];
            mAttractorVertices[idx + 2] = z[outerIdx];
        }
    }
    catch (std::runtime_error&)
    {
        exit(-ERR_FILE_EXIST);
    }

    glGenVertexArrays(1, &mAttractorArrayObject);
    glGenBuffers(1, &mAttractorBufferObject);
    glBindVertexArray(mAttractorArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, mAttractorBufferObject);
    glBufferData(GL_ARRAY_BUFFER, mAttractorVerticesSize * sizeof(GLfloat),
                 mAttractorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    /// Transformation matrices.
    mProjectionMat = glm::perspective(glm::radians(mFieldOfView),
                                      static_cast<GLfloat>(mWindowWidth) /
                                      static_cast<GLfloat>(mWindowHeight),
                                      mNearDistance, mFarDistance);
    mViewMat = sCamera->getViewMatrix();
}

void AttractorGLApp::mainLoop()
{
    while (!glfwWindowShouldClose(mWindow))
    {
        mFpsTimeDelta = sFpsManager->enforceFPS();

        glfwPollEvents();
        processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /// Background drawing.
        mBackgroundShader->use();
        glBindVertexArray(mBackgroundArrayObject);
        glDisable(GL_DEPTH_TEST);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glEnable(GL_DEPTH_TEST);
        glBindVertexArray(0);

        /// Attractor drawing.
        mAttractorShader->use();
        mAttractorShader->setVec4("color", mAttractorColor);
        mViewMat = sCamera->getViewMatrix();
        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.1f, 0.5f, 0.0f));
        setMVPViaVec(std::move(mProjectionMat * mViewMat * model));
        glLineWidth(2.0f);
        glBindVertexArray(mAttractorArrayObject);
        glDrawArrays(GL_LINE_STRIP, 0, mAttractorTime);
        glBindVertexArray(0);

        glfwSwapBuffers(mWindow);
    }

    glDeleteVertexArrays(1, &mAttractorArrayObject);
    glDeleteBuffers(1, &mAttractorBufferObject);

    glDeleteVertexArrays(1, &mBackgroundArrayObject);
    glDeleteBuffers(1, &mBackgroundBufferObject);

    terminate();
}

void AttractorGLApp::processInput()
{
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);

    if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
        sCamera->processKeyboard(FORWARD, mFpsTimeDelta);
    if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
        sCamera->processKeyboard(BACKWARD, mFpsTimeDelta);
    if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
        sCamera->processKeyboard(LEFT, mFpsTimeDelta);
    if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
        sCamera->processKeyboard(RIGHT, mFpsTimeDelta);
    if (glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_PRESS)
        if (++mAttractorTime > MAX_TIME) mAttractorTime = MAX_TIME;
    if (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS)
        if (--mAttractorTime < MIN_TIME) mAttractorTime = MIN_TIME;

    /// Transparency adjusting.
    if (glfwGetKey(mWindow, GLFW_KEY_Z) == GLFW_PRESS)
    {
        mAttractorColor[3] += ALPHA_DELTA;
        if (mAttractorColor[3] > 1.0f) mAttractorColor[3] = 1.0f;
    }
    if (glfwGetKey(mWindow, GLFW_KEY_X) == GLFW_PRESS)
    {
        mAttractorColor[3] -= ALPHA_DELTA;
        if (mAttractorColor[3] < 0.0f) mAttractorColor[3] = 0.0f;
    }
}

void AttractorGLApp::setMVPViaVec(const glm::mat4& mvp) const
{
    /// Only for attractor.vs.
    mAttractorShader->setVec4("trans_0", mvp[0][0], mvp[0][1], mvp[0][2], mvp[0][3]);
    mAttractorShader->setVec4("trans_1", mvp[1][0], mvp[1][1], mvp[1][2], mvp[1][3]);
    mAttractorShader->setVec4("trans_2", mvp[2][0], mvp[2][1], mvp[2][2], mvp[2][3]);
    mAttractorShader->setVec4("trans_3", mvp[3][0], mvp[3][1], mvp[3][2], mvp[3][3]);
}

void AttractorGLApp::setFrameBufferSizeCallback(void (* func)(GLFWwindow*, GLint, GLint))
{
    glfwSetFramebufferSizeCallback(mWindow, func);
}

void AttractorGLApp::setCursorPosCallback(void (* func)(GLFWwindow*, GLdouble, GLdouble))
{
    glfwSetCursorPosCallback(mWindow, func);
}
