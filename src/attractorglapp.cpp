#include <attractorglapp.hpp>
#include <utils.hpp>

std::shared_ptr<Camera> AttractorGLApp::sCamera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 10.0f));
std::shared_ptr<FpsManager> AttractorGLApp::sFpsManager = std::make_shared<FpsManager>();

AttractorGLApp::AttractorGLApp(GLint width, GLint height, const std::string& title)
    : IGLApp(width, height, title)
    , mAttractorFilter(AttractorFilter::BOTH)
{
    configure();
}

void AttractorGLApp::configure()
{
    IGLApp::configure();

    /// Callbacks.
    setFrameBufferSizeCallback([](GLFWwindow*, GLint width, GLint height)
    {
        glViewport(0, 0, width, height);
    });

    setCursorPosCallback([](GLFWwindow*, GLdouble xPos, GLdouble yPos)
    {
        sCamera->processMouseMovement(xPos, yPos);
    });

    /// Shaders.
    mBackgroundShader = std::make_shared<Shader>("shaders/background/vert.glsl",
                                                 "shaders/background/frag.glsl");
    mAttractorShader = std::make_shared<Shader>("shaders/attractor/vert.glsl",
                                                "shaders/attractor/frag.glsl");

    /// Models configuration
    configureBackground();
    configureFirstAttractor();
    configureSecondAttractor();

    /// Transformation matrices.
    mProjectionMat = glm::perspective(glm::radians(mFieldOfView),
                                      static_cast<GLfloat>(mWindowWidth) /
                                      static_cast<GLfloat>(mWindowHeight),
                                      mNearDistance, mFarDistance);
    mViewMat = sCamera->getViewMatrix();
    mModelMat = glm::mat4(1.0f);
}

void AttractorGLApp::mainLoop()
{
    while (!glfwWindowShouldClose(mWindow))
    {
        mFpsTimeDelta = sFpsManager->enforceFPS();

        glfwPollEvents();
        processInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawBackgroundGradient(glm::vec3(0.4f, 0.4f, 0.4f),
                               glm::vec3(0.1f, 0.1f, 0.1f));

        /// Attractors drawing.
        mAttractorShader->use();
        mViewMat = sCamera->getViewMatrix();
        setAttractorMVP(std::move(mProjectionMat * mViewMat * mModelMat));
        glLineWidth(2.0f);
        /// First.
        mAttractorShader->setVec4("color", mFirstAttractorColor);
        glBindVertexArray(mFirstAttractorArrayObject);
        glDrawArrays(GL_LINE_STRIP, 0, mFirstAttractorTime);
        /// Second.
        mAttractorShader->setVec4("color", mSecondAttractorColor);
        glBindVertexArray(mSecondAttractorArrayObject);
        glDrawArrays(GL_LINE_STRIP, 0, mSecondAttractorTime);
        glBindVertexArray(0);

        glfwSwapBuffers(mWindow);
    }

    terminate();
}

void AttractorGLApp::terminate()
{
    glDeleteVertexArrays(1, &mSecondAttractorArrayObject);
    glDeleteBuffers(1, &mSecondAttractorBufferObject);
    delete[] mSecondAttractorVertices;

    glDeleteVertexArrays(1, &mFirstAttractorArrayObject);
    glDeleteBuffers(1, &mFirstAttractorBufferObject);
    delete[] mFirstAttractorVertices;

    glDeleteVertexArrays(1, &mBackgroundArrayObject);
    IGLApp::terminate();
}

void AttractorGLApp::processInput()
{
    if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(mWindow, true);

    /// View transformations.
    if (glfwGetKey(mWindow, GLFW_KEY_W) == GLFW_PRESS)
        sCamera->processKeyboard(FORWARD, mFpsTimeDelta);
    if (glfwGetKey(mWindow, GLFW_KEY_S) == GLFW_PRESS)
        sCamera->processKeyboard(BACKWARD, mFpsTimeDelta);
    if (glfwGetKey(mWindow, GLFW_KEY_A) == GLFW_PRESS)
        sCamera->processKeyboard(LEFT, mFpsTimeDelta);
    if (glfwGetKey(mWindow, GLFW_KEY_D) == GLFW_PRESS)
        sCamera->processKeyboard(RIGHT, mFpsTimeDelta);

    processInputForAttractors();
}

void AttractorGLApp::processInputForAttractors()
{
    /// Attractors selection.
    if (glfwGetKey(mWindow, GLFW_KEY_1) == GLFW_PRESS)
        mAttractorFilter = AttractorFilter::FIRST;
    if (glfwGetKey(mWindow, GLFW_KEY_2) == GLFW_PRESS)
        mAttractorFilter = AttractorFilter::SECOND;
    if (glfwGetKey(mWindow, GLFW_KEY_3) == GLFW_PRESS)
        mAttractorFilter = AttractorFilter::BOTH;

    /// Attractors time.
    if (glfwGetKey(mWindow, GLFW_KEY_Q) == GLFW_PRESS)
        adjustAttractorTime(true);
    if (glfwGetKey(mWindow, GLFW_KEY_E) == GLFW_PRESS)
        adjustAttractorTime(false);

    /// Color transformation.
    if (glfwGetKey(mWindow, GLFW_KEY_F1) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::RED, true);    /// INC RED.
    if (glfwGetKey(mWindow, GLFW_KEY_F2) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::RED, false);   /// DEC RED.
    if (glfwGetKey(mWindow, GLFW_KEY_F3) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::GREEN, true);  /// INC GREEN.
    if (glfwGetKey(mWindow, GLFW_KEY_F4) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::GREEN, false); /// DEC GREEN.
    if (glfwGetKey(mWindow, GLFW_KEY_F5) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::BLUE, true);   /// INC BLUE.
    if (glfwGetKey(mWindow, GLFW_KEY_F6) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::BLUE, false);  /// DEC BLUE.
    if (glfwGetKey(mWindow, GLFW_KEY_F7) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::ALPHA, true);  /// INC ALPHA.
    if (glfwGetKey(mWindow, GLFW_KEY_F8) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::ALPHA, false); /// DEC ALPHA.
}

void AttractorGLApp::configureBackground()
{
    /// Background.
    glGenVertexArrays(1, &mBackgroundArrayObject);
}

void AttractorGLApp::configureFirstAttractor()
{
    /// First attractor.
    mFirstAttractorTime = 1;
    mFirstAttractorColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    try
    {
        auto x = Utils::readPoints("res/first/x.txt");
        auto y = Utils::readPoints("res/first/y.txt");
        auto z = Utils::readPoints("res/first/z.txt");

        mFirstAttractorVerticesSize = 3 * x.size();
        mFirstAttractorVertices = new GLfloat[mFirstAttractorVerticesSize];
        for (GLsizei idx = 0, outerIdx = 0; idx < x.size(); idx += 3, ++outerIdx)
        {
            mFirstAttractorVertices[idx + 0] = x[outerIdx];
            mFirstAttractorVertices[idx + 1] = y[outerIdx];
            mFirstAttractorVertices[idx + 2] = z[outerIdx];
        }
    }
    catch (std::runtime_error& exc)
    {
        std::cerr << exc.what() << std::endl;
        exit(-ERR_FILE_EXIST);
    }

    glGenVertexArrays(1, &mFirstAttractorArrayObject);
    glGenBuffers(1, &mFirstAttractorBufferObject);
    glBindVertexArray(mFirstAttractorArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, mFirstAttractorBufferObject);
    glBufferData(GL_ARRAY_BUFFER, mFirstAttractorVerticesSize * sizeof(GLfloat),
                 mFirstAttractorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AttractorGLApp::configureSecondAttractor()
{
    /// Second attractor.
    mSecondAttractorTime = 1;
    mSecondAttractorColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    try
    {
        auto x = Utils::readPoints("res/second/x.txt");
        auto y = Utils::readPoints("res/second/y.txt");
        auto z = Utils::readPoints("res/second/z.txt");

        mSecondAttractorVerticesSize = 3 * x.size();
        mSecondAttractorVertices = new GLfloat[mSecondAttractorVerticesSize];
        for (GLsizei idx = 0, outerIdx = 0; idx < x.size(); idx += 3, ++outerIdx)
        {
            mSecondAttractorVertices[idx + 0] = x[outerIdx];
            mSecondAttractorVertices[idx + 1] = y[outerIdx];
            mSecondAttractorVertices[idx + 2] = z[outerIdx];
        }
    }
    catch (std::runtime_error& exc)
    {
        std::cerr << exc.what() << std::endl;
        exit(-ERR_FILE_EXIST);
    }

    glGenVertexArrays(1, &mSecondAttractorArrayObject);
    glGenBuffers(1, &mSecondAttractorBufferObject);
    glBindVertexArray(mSecondAttractorArrayObject);
    glBindBuffer(GL_ARRAY_BUFFER, mSecondAttractorBufferObject);
    glBufferData(GL_ARRAY_BUFFER, mSecondAttractorVerticesSize * sizeof(GLfloat),
                 mSecondAttractorVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat),
                          reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AttractorGLApp::setAttractorMVP(const glm::mat4& mvp) const
{
    /// Only for attractor.vs.
    mAttractorShader->setVec4("trans_0", mvp[0][0], mvp[0][1], mvp[0][2], mvp[0][3]);
    mAttractorShader->setVec4("trans_1", mvp[1][0], mvp[1][1], mvp[1][2], mvp[1][3]);
    mAttractorShader->setVec4("trans_2", mvp[2][0], mvp[2][1], mvp[2][2], mvp[2][3]);
    mAttractorShader->setVec4("trans_3", mvp[3][0], mvp[3][1], mvp[3][2], mvp[3][3]);
}

void AttractorGLApp::adjustAttractorTime(bool toIncrement)
{
    if (mAttractorFilter == AttractorFilter::FIRST)
    {
        if (toIncrement)
        {
            if (++mFirstAttractorTime > MAX_TIME) mFirstAttractorTime = MAX_TIME;
        }
        else
        {
            if (--mFirstAttractorTime < MIN_TIME) mFirstAttractorTime = MIN_TIME;
        }
    }
    else if (mAttractorFilter == AttractorFilter::SECOND)
    {
        if (toIncrement)
        {
            if (++mSecondAttractorTime > MAX_TIME) mSecondAttractorTime = MAX_TIME;
        }
        else
        {
            if (--mSecondAttractorTime < MIN_TIME) mSecondAttractorTime = MIN_TIME;
        }
    }
    else /// Both.
    {
        if (toIncrement)
        {
            if (++mFirstAttractorTime > MAX_TIME) mFirstAttractorTime = MAX_TIME;
            if (++mSecondAttractorTime > MAX_TIME) mSecondAttractorTime = MAX_TIME;
        }
        else
        {
            if (--mFirstAttractorTime < MIN_TIME) mFirstAttractorTime = MIN_TIME;
            if (--mSecondAttractorTime < MIN_TIME) mSecondAttractorTime = MIN_TIME;
        }
    }
}

void AttractorGLApp::adjustAttractorColor(const ColorComponent& component,
                                          bool toIncrement)
{
    static auto increment = [](glm::vec4& color, const ColorComponent& component)
    {
        color[static_cast<int>(component)] += COLOR_DELTA;
        if (color[static_cast<int>(component)] > 1.0f)
            color[static_cast<int>(component)] = 1.0f;
    };
    static auto decrement = [](glm::vec4& color, const ColorComponent& component)
    {
        color[static_cast<int>(component)] -= COLOR_DELTA;
        if (color[static_cast<int>(component)] < 0.0f)
            color[static_cast<int>(component)] = 0.0f;
    };

    if (mAttractorFilter == AttractorFilter::FIRST)
    {
        if (toIncrement)
            increment(mFirstAttractorColor, component);
        else
            decrement(mFirstAttractorColor, component);
    }
    else if (mAttractorFilter == AttractorFilter::SECOND)
    {
        if (toIncrement)
            increment(mSecondAttractorColor, component);
        else
            decrement(mSecondAttractorColor, component);
    }
    else /// Both
    {
        if (toIncrement)
        {
            increment(mFirstAttractorColor, component);
            increment(mSecondAttractorColor, component);
        }
        else
        {
            decrement(mFirstAttractorColor, component);
            decrement(mSecondAttractorColor, component);
        }
    }
}

void AttractorGLApp::drawBackgroundGradient(glm::vec3 topColor, glm::vec3 bottomColor)
{
    mBackgroundShader->use();
    mBackgroundShader->setVec3("top_color", topColor);
    mBackgroundShader->setVec3("bot_color", bottomColor);
    glDisable(GL_DEPTH_TEST);
    glBindVertexArray(mBackgroundArrayObject);
    glDrawArrays(GL_TRIANGLES, 0, 3);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
    return;
}

void AttractorGLApp::setFrameBufferSizeCallback(void (* func)(GLFWwindow*, GLint, GLint))
{
    glfwSetFramebufferSizeCallback(mWindow, func);
}

void AttractorGLApp::setCursorPosCallback(void (* func)(GLFWwindow*, GLdouble, GLdouble))
{
    glfwSetCursorPosCallback(mWindow, func);
}
