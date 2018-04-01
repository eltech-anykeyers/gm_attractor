#ifndef ATTRACTORGLAPP_HPP
#define ATTRACTORGLAPP_HPP

#include <memory>

#include <iglapp.hpp>
#include <camera.hpp>
#include <fpsmanager.hpp>

class AttractorGLApp : public IGLApp
{
public:
    AttractorGLApp(GLint width, GLint height, const std::string& title)
        : IGLApp(width, height, title)
    {}

protected:
    virtual void configure();
    virtual void mainLoop() override;

    virtual void setFrameBufferSizeCallback(void (* func)(GLFWwindow*, int, int)) override;
    virtual void setCursorPosCallback(void (* func)(GLFWwindow*, double, double)) override;

private:
    static std::shared_ptr<Camera> sCamera;
    static std::shared_ptr<FpsManager> sFpsManager;
};

#endif // ATTRACTORGLAPP_HPP
