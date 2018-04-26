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
    virtual void configure() override;
    virtual void mainLoop() override;
    virtual void terminate() override;

    virtual void setFrameBufferSizeCallback(void (* func)(GLFWwindow*, GLint, GLint)) override;
    virtual void setCursorPosCallback(void (* func)(GLFWwindow*, GLdouble, GLdouble)) override;

private:
    constexpr static const GLuint ERR_FILE_EXIST = 4;

    constexpr static const GLsizei MIN_TIME = 1;
    constexpr static const GLsizei MAX_TIME = 10000;

    constexpr static const GLfloat COLOR_DELTA = 0.001f;

    static std::shared_ptr<Camera> sCamera;

    GLfloat mFpsTimeDelta;
    static std::shared_ptr<FpsManager> sFpsManager;

    /// Background.
    std::shared_ptr<Shader> mBackgroundShader;

    GLuint mBackgroundBufferObject;
    GLuint mBackgroundElementsBufferObject;
    GLuint mBackgroundArrayObject;

    GLfloat* mBackgroundVertices;
    GLsizei mBackgroundVerticesSize;
    GLint* mBackgroundVerticesOrder;
    GLsizei mBackgroundVerticesOrderSize;

    std::shared_ptr<Shader> mAttractorShader;

    /// First attractor.
    GLfloat mFirstAttractorTime;
    glm::vec4 mFirstAttractorColor;

    GLuint mFirstAttractorBufferObject;
    GLuint mFirstAttractorArrayObject;

    GLfloat* mFirstAttractorVertices;
    GLsizei mFirstAttractorVerticesSize;

    /// Second attractor.
    GLfloat mSecondAttractorTime;
    glm::vec4 mSecondAttractorColor;

    GLuint mSecondAttractorBufferObject;
    GLuint mSecondAttractorArrayObject;

    GLfloat* mSecondAttractorVertices;
    GLsizei mSecondAttractorVerticesSize;

    /// Transformations.
    glm::mat4 mProjectionMat;
    glm::mat4 mViewMat;
    glm::mat4 mModelMat;

    void configureBackground();
    void configureFirstAttractor();
    void configureSecondAttractor();

    void setMVPViaVec(const glm::mat4& mvp) const;

    void processInput();
};

#endif // ATTRACTORGLAPP_HPP
