#ifndef ATTRACTORGLAPP_HPP
#define ATTRACTORGLAPP_HPP

#include <memory>

#include <iglapp.hpp>
#include <camera.hpp>
#include <fpsmanager.hpp>
#include <glm/glm.hpp>
#include <shader.hpp>
#include <attractormodel.hpp>

#include <utils.hpp>

enum AttractorFilter : int
{
    FIRST,
    SECOND,
    BOTH
};

enum ColorComponent : int
{
    RED,
    GREEN,
    BLUE,
    ALPHA
};

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
    GLuint mBackgroundArrayObject;

    /// Attractors.
    std::shared_ptr<Shader> mAttractorShader;
    AttractorFilter mAttractorFilter;

    /// First attractor.
    GLfloat mFirstAttractorTime;
    glm::vec4 mFirstAttractorColor;
    std::unique_ptr<AttractorModel> mFirstAttractor;

    /// Second attractor.
    GLfloat mSecondAttractorTime;
    glm::vec4 mSecondAttractorColor;
    std::unique_ptr<AttractorModel> mSecondAttractor;

    /// Transformations.
    glm::mat4 mProjectionMat;

    void configureBackground();

    std::vector<glm::vec2> readSectionVertices(std::string xFile,
                                               std::string yFile);
    std::vector<glm::vec3> readAttractorVertices(std::string xFile,
                                                 std::string yFile,
                                                 std::string zFile);

    void setAttractorMVP(const glm::mat4& mvp) const;
    void adjustAttractorTime(bool toIncrement);
    void adjustAttractorColor(const ColorComponent& component, bool toIncrement);

    void processInput();
    void processInputForAttractors();

    /// Drawing.
    void drawBackgroundGradient(const glm::vec3& topColor,
                                const glm::vec3& bottomColor
                               ) const;
};

#endif // ATTRACTORGLAPP_HPP
