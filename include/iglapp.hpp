#ifndef GLAPP_HPP
#define GLAPP_HPP

#include <memory>
#include <string>

#include <camera.hpp>
#include <fpsmanager.hpp>
#include <GL/gl.h>
#include <GLFW/glfw3.h>

class IGLApp
{
public:
    IGLApp(GLint width, GLint height, const std::string& title);

    void run();

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

    std::shared_ptr<Camera> mCamera;
    std::shared_ptr<FpsManager> mFpsManager;

    void init();
    void configure();
    void terminate();

    virtual void mainLoop() = 0;
};

#endif // GLAPP_HPP
