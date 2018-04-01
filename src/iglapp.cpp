#include <cmath>
#include <stdexcept>

#include <iglapp.hpp>

const std::string GLAD_LOAD_FAILED_MSG = "GLAD can't be loaded.";

const std::string GLFW_INIT_FAILED_MSG = "GLFW can't be initialized.";
const std::string GLFW_CREATE_WINDOW_FAILED_MSG = "GLFW window can't be created.";

const std::string INCORRECT_VALUE_MSG = "Incorrect value";

const GLfloat DEFAULT_FIELD_OF_VIEW = 45.0f;
const GLfloat DEFAULT_NEAR_DISTANCE = 2.0f;
const GLfloat DEFAULT_FAR_DISTANCE = 1500.0f;

IGLApp::IGLApp(GLint width, GLint height, const std::string& title)
    : mWindowWidth(width)
    , mWindowHeight(height)
    , mWindowCenterX(mWindowWidth / 2)
    , mWindowCenterY(mWindowHeight / 2)
    , mWindowTitle(title)
    , mFieldOfView(DEFAULT_FIELD_OF_VIEW)
    , mNearDistance(DEFAULT_NEAR_DISTANCE)
    , mFarDistance(DEFAULT_FAR_DISTANCE)
{
    init();
    configure();
}

void IGLApp::init()
{
    if (!glfwInit())
    {
        throw std::runtime_error(GLFW_INIT_FAILED_MSG);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight,
                               mWindowTitle.c_str(), nullptr, nullptr);
    if (!mWindow)
    {
        throw std::runtime_error(GLFW_CREATE_WINDOW_FAILED_MSG);
    }
    glfwMakeContextCurrent(mWindow);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
    {
        throw std::runtime_error(GLFW_CREATE_WINDOW_FAILED_MSG);
    }

    /// Setup viewport to be the entire size of the window.
    glViewport(0, 0, mWindowWidth, mWindowHeight);
}

void IGLApp::configure()
{
    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(mWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    setFrameBufferSizeCallback([](GLFWwindow*, int width, int height)
    {
        glViewport(0, 0, width, height);
    });

    /// Camera.
    mCamera = std::make_shared<Camera>();

    /// FpsManager.
    mFpsManager = std::make_shared<FpsManager>(20);
}

void IGLApp::run()
{
    mainLoop();
}

void IGLApp::terminate()
{
    glfwTerminate();
}

GLfloat IGLApp::getWindowWidth() const
{
    return mWindowWidth;
}

void IGLApp::setWindowWidth(GLint value)
{
    if (value <= 0)
    {
        throw std::runtime_error(INCORRECT_VALUE_MSG + ": " +
                                 std::to_string(value));
    }
    mWindowWidth = value;
    mWindowCenterX = mWindowWidth / 2;
    /// TODO: Trigger window update or lock method after run() is called.
}

GLfloat IGLApp::getWindowHeight() const
{
    return mWindowHeight;
}

void IGLApp::setWindowHeight(GLint value)
{
    if (value <= 0)
    {
        throw std::runtime_error(INCORRECT_VALUE_MSG + ": " +
                                 std::to_string(value));
    }
    mWindowHeight = value;
    mWindowCenterY = mWindowHeight / 2;
    /// TODO: Trigger window update or lock method after run() is called.
}

GLfloat IGLApp::getFieldOfView() const
{
    return mFieldOfView;
}

void IGLApp::setFieldOfView(GLfloat value)
{
    if (value < 0.0 && value > 180.0)
    {
        throw std::runtime_error(INCORRECT_VALUE_MSG + ": " +
                                 std::to_string(value));
    }
    mFieldOfView = value;
}

GLfloat IGLApp::getNearDistance() const
{
    return mNearDistance;
}

void IGLApp::setNearDistance(GLfloat value)
{
    if (value < 0.0 && value > mFarDistance)
    {
        throw std::runtime_error(INCORRECT_VALUE_MSG + ": " +
                                 std::to_string(value));
    }
    mNearDistance = value;
}

GLfloat IGLApp::getFarDistance() const
{
    return mFarDistance;
}

void IGLApp::setFarDistance(GLfloat value)
{
    if (value < 0.0 && value < mNearDistance)
    {
        throw std::runtime_error(INCORRECT_VALUE_MSG + ": " +
                                 std::to_string(value));
    }
    mFarDistance = value;
}

