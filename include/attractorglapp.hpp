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

    std::string firstAttractorTrajectory();
    void setFirstAttractorTrajectory(std::string dir);

    std::string firstAttractorSection();
    void setFirstAttractorSection(std::string dir);

    std::string secondAttractorTrajectory();
    void setSecondAttractorTrajectory(std::string dir);

    std::string secondAttractorSection();
    void setSecondAttractorSection(std::string dir);

protected:
    virtual void configureApp() override;
    virtual void mainLoop() override;
    virtual void terminate() override;

    virtual void setFrameBufferSizeCallback(void (* func)(GLFWwindow*, GLint, GLint)) override;
    virtual void setCursorPosCallback(void (* func)(GLFWwindow*, GLdouble, GLdouble)) override;

private:
    constexpr static const GLuint ERR_FILE_EXIST = 4;

    constexpr static const GLsizei MIN_TIME = 1;
    constexpr static const GLsizei MAX_TIME = 10000;

    constexpr static const GLfloat DISTANCE_THRESHOLD = 0.025f;

    constexpr static const GLfloat COLOR_DELTA = 0.001f;

    static std::unique_ptr<Camera> sCamera;

    GLfloat mFpsTimeDelta;
    static std::unique_ptr<FpsManager> sFpsManager;

    /// Background.
    std::unique_ptr<Shader> mBackgroundShader;
    GLuint mBackgroundArrayObject;

    AttractorFilter mAttractorFilter;

    GLfloat mTimeDiff;

    /// First attractor.
    GLfloat mFirstAttractorTime;
    std::unique_ptr<AttractorModel> mFirstAttractor;
    std::string mFirstAttractorTrajectory;
    std::string mFirstAttractorSection;

    /// Second attractor.
    GLfloat mSecondAttractorTime;
    std::unique_ptr<AttractorModel> mSecondAttractor;
    std::string mSecondAttractorTrajectory;
    std::string mSecondAttractorSection;

    std::vector<bool> mPositionsToBeDrawnBoth;

    /// Transformations.
    glm::mat4 mProjectionMat;

    void configureBackground();

    std::vector<glm::vec2> readSectionVertices(std::string xFile,
                                               std::string yFile);
    std::vector<glm::vec3> readAttractorVertices(std::string xFile,
                                                 std::string yFile,
                                                 std::string zFile);

    void adjustAttractorTime(bool toIncrement);
    void adjustAttractorColor(const ColorComponent& component, bool toIncrement);

    void processInput();
    void processInputForAttractors();

    /// Drawing.
    void drawBackgroundGradient(const glm::vec3& topColor,
                                const glm::vec3& bottomColor
                               ) const;

    void calculatePositionsToBeDrawnBoth();
};

#endif // ATTRACTORGLAPP_HPP
