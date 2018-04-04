#ifndef ATTRACTORGLAPP_HPP
#define ATTRACTORGLAPP_HPP

#include <memory>

#include <iglapp.hpp>
#include <camera.hpp>
#include <fpsmanager.hpp>
#include <glm/glm.hpp>
#include <shader.hpp>

class AttractorGLApp : public IGLApp
{
public:
    AttractorGLApp(GLint width, GLint height, const std::string& title);

protected:
    virtual void configure();
    virtual void mainLoop() override;

    virtual void setFrameBufferSizeCallback(void (* func)(GLFWwindow*, int, int)) override;
    virtual void setCursorPosCallback(void (* func)(GLFWwindow*, double, double)) override;

private:
    static std::shared_ptr<Camera> sCamera;

    static std::shared_ptr<FpsManager> sFpsManager;
    float mTimeDelta;

    std::shared_ptr<Shader> mMainShader;

    unsigned int mVertexBufferObject;
    unsigned int mVertexArrayObject;

    glm::mat4 mProjectionMat;
    glm::mat4 mViewMat;
    glm::mat4 mModelMat;

    void processInput();

    void setMVPViaVec(const glm::mat4& mvp) const;
};

#endif // ATTRACTORGLAPP_HPP
