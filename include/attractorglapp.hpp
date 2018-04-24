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
    constexpr static const unsigned int ERR_FILE_EXIST = 4;

    constexpr static const size_t MIN_TIME = 1;
    constexpr static const size_t MAX_TIME = 10000;

    static std::shared_ptr<Camera> sCamera;

    static std::shared_ptr<FpsManager> sFpsManager;
    float mTimeDelta;

    float mTime;

    size_t mAttractorVerticesSize;
    float* mAttractorVertices;

    std::shared_ptr<Shader> mMainShader;

    unsigned int mMainBufferObject;
    unsigned int mMainArrayObject;

    glm::mat4 mProjectionMat;
    glm::mat4 mViewMat;
    glm::mat4 mModelMat;

    void processInput();

    void setMVPViaVec(const glm::mat4& mvp) const;
};

#endif // ATTRACTORGLAPP_HPP
