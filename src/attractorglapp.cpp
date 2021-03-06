#include <attractorglapp.hpp>

std::unique_ptr<Camera> AttractorGLApp::sCamera = std::make_unique<Camera>(glm::vec3(0.0f, 0.0f, 10.0f));
std::unique_ptr<FpsManager> AttractorGLApp::sFpsManager = std::make_unique<FpsManager>();

AttractorGLApp::AttractorGLApp(
        GLint width, GLint height, const std::string& title)
    : IGLApp(width, height, title)
    , mAttractorFilter(AttractorFilter::BOTH)
    , mTimeDiff(1.0f)
    , mRadius(0.01f)
    , mRotation(0.0f, 0.0f, 0.0f)
    , mFirstAttractorTrajectory("coullet_1/")
    , mFirstAttractorSection("heart/")
    , mSecondAttractorTrajectory("coullet_2/")
    , mSecondAttractorSection("square/")
{

}

void AttractorGLApp::configureApp()
{
    IGLApp::configureApp();

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
    mBackgroundShader = std::make_unique<Shader>("shaders/background/vert.glsl",
                                                 "shaders/background/frag.glsl");

    std::string trajectoriesDir = "res/attractors_data/trajectories/";
    std::string sectionsDir     = "res/attractors_data/section_shapes/";

    /// First attractor.
    mFirstAttractorTime = 0;
    mFirstAttractor = std::make_unique<AttractorModel>(
            readAttractorVertices(trajectoriesDir +
                                      mFirstAttractorTrajectory + "x.txt",
                                  trajectoriesDir +
                                      mFirstAttractorTrajectory + "y.txt",
                                  trajectoriesDir +
                                      mFirstAttractorTrajectory + "z.txt"),
            readSectionVertices(sectionsDir +
                                    mFirstAttractorSection + "x.txt",
                                sectionsDir +
                                    mFirstAttractorSection + "y.txt"));
    mFirstAttractor->setColor(glm::vec4(1.0f, 0.0f, 0.0f, 0.67f));
    mFirstAttractor->setRadius(mRadius);

    /// Second attractor.
    mSecondAttractorTime = 0;
    mSecondAttractor = std::make_unique<AttractorModel>(
            readAttractorVertices(trajectoriesDir +
                                      mSecondAttractorTrajectory + "x.txt",
                                  trajectoriesDir +
                                      mSecondAttractorTrajectory + "y.txt",
                                  trajectoriesDir +
                                      mSecondAttractorTrajectory + "z.txt"),
            readSectionVertices(sectionsDir +
                                    mSecondAttractorSection + "x.txt",
                                sectionsDir +
                                    mSecondAttractorSection + "y.txt"));
    mSecondAttractor->setColor(glm::vec4(0.0f, 0.0f, 1.0f, 0.67f));
    mSecondAttractor->setRadius(mRadius);

    calculatePositionsToBeDrawnBoth();

    /// Background.
    configureBackground();

    /// Transformation matrices.
    mProjectionMat = glm::perspective(glm::radians(mFieldOfView),
                                      static_cast<GLfloat>(mWindowWidth) /
                                      static_cast<GLfloat>(mWindowHeight),
                                      mNearDistance, mFarDistance);
}

void AttractorGLApp::mainLoop()
{
    while (!glfwWindowShouldClose(mWindow))
    {
        mFpsTimeDelta = sFpsManager->enforceFPS();

        glfwPollEvents();
        processInput();

        /// Background.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        drawBackgroundGradient(glm::vec3(0.4f, 0.4f, 0.4f),
                               glm::vec3(0.1f, 0.1f, 0.1f));

        /// Attractors.
        glm::mat4 projViewMat = mProjectionMat * sCamera->getViewMatrix();
        for (GLsizei idx = 0; idx <= mFirstAttractorTime; ++idx)
        {
            auto draw = [&](GLsizei idx)
            {
                mFirstAttractor->draw(projViewMat, idx, 1);
                if (mPositionsToBeDrawnBoth[idx])
                {
                    mSecondAttractor->draw(projViewMat, idx, 1);
                }
            };

            /// Invert attractor's end color.
            if (idx >= END_TIME)
            {
                auto firstColor = mFirstAttractor->getColor();
                auto secondColor = mSecondAttractor->getColor();

                mFirstAttractor->setColor(glm::vec4(1.0f - firstColor.r,
                                                    1.0f - firstColor.g,
                                                    1.0f - firstColor.b,
                                                    firstColor.a));
                mSecondAttractor->setColor(glm::vec4(1.0f - secondColor.r,
                                                     1.0f - secondColor.g,
                                                     1.0f - secondColor.b,
                                                     secondColor.a));

                draw(idx);

                mFirstAttractor->setColor(firstColor);
                mSecondAttractor->setColor(secondColor);

                continue;
            }

            draw(idx);
        }

        glfwSwapBuffers(mWindow);
    }

    terminate();
}

void AttractorGLApp::terminate()
{
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
    if (glfwGetKey(mWindow, GLFW_KEY_Z) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::RED, true);    /// INC RED.
    if (glfwGetKey(mWindow, GLFW_KEY_X) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::RED, false);   /// DEC RED.
    if (glfwGetKey(mWindow, GLFW_KEY_C) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::GREEN, true);  /// INC GREEN.
    if (glfwGetKey(mWindow, GLFW_KEY_V) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::GREEN, false); /// DEC GREEN.
    if (glfwGetKey(mWindow, GLFW_KEY_B) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::BLUE, true);   /// INC BLUE.
    if (glfwGetKey(mWindow, GLFW_KEY_N) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::BLUE, false);  /// DEC BLUE.
    if (glfwGetKey(mWindow, GLFW_KEY_M) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::ALPHA, true);  /// INC ALPHA.
    if (glfwGetKey(mWindow, GLFW_KEY_COMMA) == GLFW_PRESS)
        adjustAttractorColor(ColorComponent::ALPHA, false); /// DEC ALPHA.

    /// Time diff adjusting.
    if (glfwGetKey(mWindow, GLFW_KEY_F1) == GLFW_PRESS)
        if (++mTimeDiff > 100.0f) mTimeDiff = 100.0f;
    if (glfwGetKey(mWindow, GLFW_KEY_F2) == GLFW_PRESS)
        if (--mTimeDiff < 1.0f) mTimeDiff = 1.0f;

    /// Radius adjusting.
    if (glfwGetKey(mWindow, GLFW_KEY_F3) == GLFW_PRESS)
    {
        mRadius += 0.005f;
        if (mRadius > 0.1f)
            mRadius = 0.1f;
        mFirstAttractor->setRadius(mRadius);
        mSecondAttractor->setRadius(mRadius);
    }
    if (glfwGetKey(mWindow, GLFW_KEY_F4) == GLFW_PRESS)
    {
        mRadius -= 0.005f;
        if (mRadius < 0.01f)
            mRadius = 0.01f;
        mFirstAttractor->setRadius(mRadius);
        mSecondAttractor->setRadius(mRadius);
    }

    /// Rotating.
    /// x-rotation.
    if (glfwGetKey(mWindow, GLFW_KEY_F5) == GLFW_PRESS)
    {
        mRotation.x += 0.025f;
        if (mRotation.x > PI_TWICE)
            mRotation.x = 0.0f;
        mFirstAttractor->rotateTo(mRotation);
        mSecondAttractor->rotateTo(mRotation);
    }
    if (glfwGetKey(mWindow, GLFW_KEY_F6) == GLFW_PRESS)
    {
        mRotation.x -= 0.025f;
        if (mRotation.x < 0.0f)
            mRotation.x = PI_TWICE;
        mFirstAttractor->rotateTo(mRotation);
        mSecondAttractor->rotateTo(mRotation);
    }
    /// y-rotation.
    if (glfwGetKey(mWindow, GLFW_KEY_F7) == GLFW_PRESS)
    {
        mRotation.y += 0.025f;
        if (mRotation.y > PI_TWICE)
            mRotation.y = 0.0f;
        mFirstAttractor->rotateTo(mRotation);
        mSecondAttractor->rotateTo(mRotation);
    }
    if (glfwGetKey(mWindow, GLFW_KEY_F8) == GLFW_PRESS)
    {
        mRotation.y -= 0.025f;
        if (mRotation.y < 0.0f)
            mRotation.y = PI_TWICE;
        mFirstAttractor->rotateTo(mRotation);
        mSecondAttractor->rotateTo(mRotation);
    }
    /// z-rotation.
    if (glfwGetKey(mWindow, GLFW_KEY_F9) == GLFW_PRESS)
    {
        mRotation.z += 0.025f;
        if (mRotation.z > PI_TWICE)
            mRotation.z = 0.0f;
        mFirstAttractor->rotateTo(mRotation);
        mSecondAttractor->rotateTo(mRotation);
    }
    if (glfwGetKey(mWindow, GLFW_KEY_F10) == GLFW_PRESS)
    {
        mRotation.z -= 0.025f;
        if (mRotation.z < 0.0f)
            mRotation.z = PI_TWICE;
        mFirstAttractor->rotateTo(mRotation);
        mSecondAttractor->rotateTo(mRotation);
    }
}

void AttractorGLApp::configureBackground()
{
    /// Background.
    glGenVertexArrays(1, &mBackgroundArrayObject);
}

std::vector<glm::vec3> AttractorGLApp::readAttractorVertices(
        std::string xFile, std::string yFile, std::string zFile)
{
    try
    {
        std::vector<glm::vec3> vertices;

        auto x = Utils::readPoints(xFile);
        auto y = Utils::readPoints(yFile);
        auto z = Utils::readPoints(zFile);

        for ( GLsizei i = 0; i < x.size(); i++ )
            vertices.emplace_back(x[i], y[i], z[i]);

        return vertices;
    }
    catch (std::runtime_error& exc)
    {
        std::cerr << exc.what() << std::endl;
        exit(-ERR_FILE_EXIST);
    }
}

std::vector<glm::vec2> AttractorGLApp::readSectionVertices(
        std::string xFile, std::string yFile)
{
    try
    {
        std::vector<glm::vec2> vertices;

        auto x = Utils::readPoints(xFile);
        auto y = Utils::readPoints(yFile);

        for ( GLsizei i = 0; i < x.size(); i++ )
            vertices.emplace_back(x[i], y[i]);

        return vertices;
    }
    catch (std::runtime_error& exc)
    {
        std::cerr << exc.what() << std::endl;
        exit(-ERR_FILE_EXIST);
    }
}

void AttractorGLApp::adjustAttractorTime(bool toIncrement)
{
/*  if (mAttractorFilter == AttractorFilter::FIRST)
    {
        if (toIncrement)
        {
            mFirstAttractorTime += mTimeDiff;
            if (mFirstAttractorTime > MAX_TIME)
                mFirstAttractorTime = MAX_TIME;
        }
        else
        {
            mFirstAttractorTime -= mTimeDiff;
            if (mFirstAttractorTime < MIN_TIME)
                mFirstAttractorTime = MIN_TIME;
        }
    }
    else if (mAttractorFilter == AttractorFilter::SECOND)
    {
        if (toIncrement)
        {
            mSecondAttractorTime += mTimeDiff;
            if (mSecondAttractorTime > MAX_TIME)
                mSecondAttractorTime = MAX_TIME;
        }
        else
        {
            mSecondAttractorTime -= mTimeDiff;
            if (mSecondAttractorTime < MIN_TIME)
                mSecondAttractorTime = MIN_TIME;
        }
    }
    else /// Both.
    { */
        if (toIncrement)
        {
            mFirstAttractorTime += mTimeDiff;
            mSecondAttractorTime += mTimeDiff;

            if (mFirstAttractorTime > MAX_TIME)
                mFirstAttractorTime = MAX_TIME;
            if (mSecondAttractorTime > MAX_TIME)
                mSecondAttractorTime = MAX_TIME;
        }
        else
        {
            mFirstAttractorTime -= mTimeDiff;
            mSecondAttractorTime -= mTimeDiff;

            if (mFirstAttractorTime < MIN_TIME)
                mFirstAttractorTime = MIN_TIME;
            if (mSecondAttractorTime < MIN_TIME)
                mSecondAttractorTime = MIN_TIME;
        }
/*  } */
}

void AttractorGLApp::adjustAttractorColor(const ColorComponent& component,
                                          bool toIncrement)
{
    static auto increment = [](AttractorModel& model, const ColorComponent& component)
    {
        auto color = model.getColor();

        color[static_cast<int>(component)] += COLOR_DELTA;
        if (color[static_cast<int>(component)] > 1.0f)
            color[static_cast<int>(component)] = 1.0f;

        model.setColor(color);
    };
    static auto decrement = [](AttractorModel& model, const ColorComponent& component)
    {
        auto color = model.getColor();

        color[static_cast<int>(component)] -= COLOR_DELTA;
        if (color[static_cast<int>(component)] < 0.0f)
            color[static_cast<int>(component)] = 0.0f;

        model.setColor(color);
    };

    if (mAttractorFilter == AttractorFilter::FIRST)
    {
        if (toIncrement)
            increment(*mFirstAttractor, component);
        else
            decrement(*mFirstAttractor, component);
    }
    else if (mAttractorFilter == AttractorFilter::SECOND)
    {
        if (toIncrement)
            increment(*mSecondAttractor, component);
        else
            decrement(*mSecondAttractor, component);
    }
    else /// Both
    {
        if (toIncrement)
        {
            increment(*mFirstAttractor, component);
            increment(*mSecondAttractor, component);
        }
        else
        {
            decrement(*mFirstAttractor, component);
            decrement(*mSecondAttractor, component);
        }
    }
}

void AttractorGLApp::drawBackgroundGradient(const glm::vec3& topColor, const glm::vec3& bottomColor) const
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

void AttractorGLApp::calculatePositionsToBeDrawnBoth()
{
    const auto& firstPoints = mFirstAttractor->getTrajectoryVertices();
    const auto& secondPoints = mSecondAttractor->getTrajectoryVertices();

    mPositionsToBeDrawnBoth.reserve(firstPoints.size());

    for (GLsizei idx = 0; idx < firstPoints.size(); ++idx)
    {
        GLfloat distance = glm::distance(firstPoints[idx], secondPoints[idx]);
        mPositionsToBeDrawnBoth.push_back(distance > DISTANCE_THRESHOLD);
    }

    return;
}

std::string AttractorGLApp::firstAttractorTrajectory()
{
    return mFirstAttractorTrajectory;
}

void AttractorGLApp::setFirstAttractorTrajectory(std::string dir)
{
    mFirstAttractorTrajectory = dir;
}

std::string AttractorGLApp::firstAttractorSection()
{
    return mFirstAttractorSection;
}

void AttractorGLApp::setFirstAttractorSection(std::string dir)
{
    mFirstAttractorSection = dir;
}

std::string AttractorGLApp::secondAttractorTrajectory()
{
    return mSecondAttractorTrajectory;
}

void AttractorGLApp::setSecondAttractorTrajectory(std::string dir)
{
    mSecondAttractorTrajectory = dir;
}

std::string AttractorGLApp::secondAttractorSection()
{
    return mSecondAttractorSection;
}

void AttractorGLApp::setSecondAttractorSection(std::string dir)
{
    mSecondAttractorSection = dir;
}

void AttractorGLApp::setFrameBufferSizeCallback(void (* func)(GLFWwindow*, GLint, GLint))
{
    glfwSetFramebufferSizeCallback(mWindow, func);
}

void AttractorGLApp::setCursorPosCallback(void (* func)(GLFWwindow*, GLdouble, GLdouble))
{
    glfwSetCursorPosCallback(mWindow, func);
}
