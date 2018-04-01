#ifndef GLAPP_HPP
#define GLAPP_HPP

#include <memory>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <camera.hpp>
#include <fpsmanager.hpp>

class IGLApp
{
public:
    IGLApp(GLint width, GLint height, const std::string& title);

    void run();

    void setFrameBufferSizeCallback(void (* func)(GLFWwindow*, int, int));
    void setCursorPosCallback(void (* func)(GLFWwindow*, double, double));

    GLfloat getWindowWidth() const;
    void setWindowWidth(GLint value);

    GLfloat getWindowHeight() const;
    void setWindowHeight(GLint value);

    GLfloat getFieldOfView() const;
    void setFieldOfView(GLfloat value);

    GLfloat getNearDistance() const;
    void setNearDistance(GLfloat value);

    GLfloat getFarDistance() const;
    void setFarDistance(GLfloat value);

protected:
    GLFWwindow* mWindow;

    GLint mWindowWidth;
    GLint mWindowHeight;
    GLint mWindowCenterX;
    GLint mWindowCenterY;

    std::string mWindowTitle;

    GLfloat mFieldOfView;
    GLfloat mNearDistance;
    GLfloat mFarDistance;

    static std::shared_ptr<Camera> sCamera;
    static std::shared_ptr<FpsManager> mFpsManager;

    virtual void init();
    virtual void configure();
    virtual void terminate();

    virtual void mainLoop() = 0;
};

#endif // GLAPP_HPP
