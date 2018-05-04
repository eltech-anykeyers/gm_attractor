#ifndef ATTRACTORMODEL_HPP
#define ATTRACTORMODEL_HPP

#include <memory>
#include <vector>
#include <string>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <glmodel.hpp>
#include <shader.hpp>
#include <utils.hpp>

class AttractorModel : public GLModel
{
public:
    AttractorModel(std::vector<glm::vec3> vertices,
                   std::vector<glm::vec2> section);
    ~AttractorModel();

    virtual void configure() override;
    virtual void draw(const glm::mat4& viewProjectionMatrix) override;
    virtual void draw(const glm::mat4& viewProjectionMatrix,
                      GLint from, GLsizei count);
    virtual void clearVertexData();

    GLfloat getNRadius() const;
    void setRadius(GLfloat radius);

    glm::vec4 getColor() const;
    void setColor(const glm::vec4& color);

protected:

private:
    constexpr static const GLfloat   DFLT_RADIUS   = 1.0f;

    std::unique_ptr<Shader> mShader;

    GLsizei mNSectionVertices;
    GLfloat mRadius;
    glm::vec4 mColor;

    std::vector<glm::vec3> mTrajectoryVertices;
    std::vector<glm::vec2> mSectionVertices;

    std::vector<GLuint> mSegmentsVaos;
    std::vector<GLuint> mSegmentsVbos;

    std::unique_ptr<glm::vec3[]> mVerticesBuffer;
    std::unique_ptr<glm::vec3[]> mSegmentBuffer;

    void setMvpMatrix(const glm::mat4& mvp);
    void drawSegment(GLint segmentNo);
    void computeSegment(GLint segmentNo, GLuint& vao, GLuint& vbo);
};

#endif // ATTRACTORMODEL_HPP
