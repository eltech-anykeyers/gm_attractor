#include <attractormodel.hpp>

AttractorModel::AttractorModel(std::vector<glm::vec3> vertices,
                               std::vector<glm::vec2> section)
    : GLModel()
{
    mTrajectoryVertices = vertices;
    mSectionVertices = section;
    mNSectionVertices = mSectionVertices.size();

    mRadius  = DFLT_RADIUS;
    mColor   = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

    configure();
}

AttractorModel::~AttractorModel()
{
    clearVertexData();
}

void AttractorModel::configure()
{

    mVerticesBuffer = std::make_unique<glm::vec3[]>(mNSectionVertices);
    mSegmentBuffer  = std::make_unique<glm::vec3[]>(2*mNSectionVertices+2);

    mShader = std::make_unique<Shader>("shaders/attractor/vert.glsl",
                                       "shaders/attractor/frag.glsl");

    glm::vec3 startPoint = mTrajectoryVertices[0];
    glm::vec3 normal = mTrajectoryVertices[1] - startPoint;

    // Calculate perpendiculars for normal vector
    glm::vec3 p1 = glm::cross(normal, glm::vec3(1.0f, 0.0f, 0.0f));
    if ( glm::dot(p1, p1) < 0.3f )
        p1 = glm::cross(normal, glm::vec3(0.0f, 1.0f, 0.0f));
    p1 = glm::normalize(p1);
    glm::vec3 p2 = glm::normalize(glm::cross(normal, p1));

    // Calculate vertices of segment
    glm::vec3 nextVertex;
    for ( GLsizei i = 0; i < mNSectionVertices; i++ ) {
        mVerticesBuffer[i] = startPoint +
            mRadius * mSectionVertices[i].x * p1 +
            mRadius * mSectionVertices[i].y * p2 ;
    }

    return;
}

void AttractorModel::draw(const glm::mat4& viewProjectionMatrix)
{
    draw(viewProjectionMatrix, 0, 2);
    return;
}

void AttractorModel::draw(
        const glm::mat4& viewProjectionMatrix,
        GLint from, GLsizei count)
{
    mShader->use();
    setModelMatrix(std::move(getModelMatrix()));
    setMvpMatrix(std::move(viewProjectionMatrix * getModelMatrix()));
    mShader->setVec4("color", mColor);
    mShader->setVec4("light_color", 1.0f, 1.0f, 1.0f, 1.0f);

    for ( GLint i = 0; i < count; i++ )
        drawSegment(from + i);

    return;
}

void AttractorModel::clearVertexData()
{
    for ( auto vao : mSegmentsVaos )
        glDeleteVertexArrays(1, &vao);
    mSegmentsVaos.clear();

    for ( auto vbo : mSegmentsVbos )
        glDeleteBuffers(1, &vbo);
    mSegmentsVbos.clear();
}

const std::vector<glm::vec3>& AttractorModel::getTrajectoryVertices() const
{
    return mTrajectoryVertices;
}

GLfloat AttractorModel::getNRadius() const
{
    return mRadius;
}

void AttractorModel::setRadius(GLfloat radius)
{
    mRadius = std::max(0.0f, radius);

    clearVertexData();
    configure();
}

glm::vec4 AttractorModel::getColor() const
{
    return mColor;
}

void AttractorModel::setColor(const glm::vec4& color)
{
    mColor = color;
}

void AttractorModel::setMvpMatrix(const glm::mat4& mvp)
{
    // Dirty trick to avoid hardware bug
    mShader->setVec4("trans_0", mvp[0][0], mvp[0][1], mvp[0][2], mvp[0][3]);
    mShader->setVec4("trans_1", mvp[1][0], mvp[1][1], mvp[1][2], mvp[1][3]);
    mShader->setVec4("trans_2", mvp[2][0], mvp[2][1], mvp[2][2], mvp[2][3]);
    mShader->setVec4("trans_3", mvp[3][0], mvp[3][1], mvp[3][2], mvp[3][3]);

    return;
}

void AttractorModel::setModelMatrix(const glm::mat4& model)
{
    // Dirty hack to avoid hardware bug
    mShader->setVec4("model_0", model[0][0], model[0][1], model[0][2], model[0][3]);
    mShader->setVec4("model_1", model[1][0], model[1][1], model[1][2], model[1][3]);
    mShader->setVec4("model_2", model[2][0], model[2][1], model[2][2], model[2][3]);
    mShader->setVec4("model_3", model[3][0], model[3][1], model[3][2], model[3][3]);

    return;
}

void AttractorModel::drawSegment(GLint segmentNo)
{
    if ( segmentNo >= mSegmentsVaos.size() ) {
        for ( int i = mSegmentsVaos.size(); i <= segmentNo; i++ ) {
            GLuint vao, vbo;
            computeSegment(i, vao, vbo);
            mSegmentsVaos.push_back(vao);
            mSegmentsVbos.push_back(vbo);
        }
    }

    glBindVertexArray(mSegmentsVaos[segmentNo]);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 2*mNSectionVertices+2);
    glBindVertexArray(0);

    return;
}

void AttractorModel::computeSegment(GLint segmentNo, GLuint& vao, GLuint& vbo)
{

    glm::vec3 topPoint = mTrajectoryVertices[segmentNo+1];
    glm::vec3 normal = topPoint - mTrajectoryVertices[segmentNo];

    // Calculate perpendiculars for normal vector
    glm::vec3 p1 = glm::cross(normal, glm::vec3(1.0f, 0.0f, 0.0f));
    if ( glm::dot(p1, p1) < 0.3f )
        p1 = glm::cross(normal, glm::vec3(0.0f, 1.0f, 0.0f));
    p1 = glm::normalize(p1);
    glm::vec3 p2 = glm::normalize(glm::cross(normal, p1));

    // Calculate vertices of segment
    glm::vec3 nextVertex;
    for ( GLsizei i = 0; i < mNSectionVertices; i++ ) {
        nextVertex = topPoint +
            mRadius * mSectionVertices[i].x * p1 +
            mRadius * mSectionVertices[i].y * p2 ;
        mSegmentBuffer[2*i] = mVerticesBuffer[i];
        mSegmentBuffer[2*i+1] = mVerticesBuffer[i] = nextVertex;
    }
    mSegmentBuffer[2*mNSectionVertices]   = mSegmentBuffer[0];
    mSegmentBuffer[2*mNSectionVertices+1] = mSegmentBuffer[1];

    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, (2*mNSectionVertices + 2) * sizeof(glm::vec3),
                 mSegmentBuffer.get(), GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3),
                          reinterpret_cast<GLvoid*>(0));
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
