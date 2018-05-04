#include <glmodel.hpp>

GLModel::GLModel()
{
    mPosition           = glm::vec3(0.0f, 0.0f, 0.0f);
    mScaleFactors       = glm::vec3(1.0f, 1.0f, 1.0f);
    mRotationQuaternion = glm::quat(0.0f, 0.0f, 0.0f, 0.0f);

    updateModelMatrix();
}

glm::mat4 GLModel::getModelMatrix() const
{
    return mModelMatrix;
}

void GLModel::moveTo(const glm::vec3& pos)
{
    mPosition = pos;
    updateModelMatrix();
    return;
}

void GLModel::moveBy(const glm::vec3& pos)
{
    mPosition += pos;
    updateModelMatrix();
    return;
}

void GLModel::scaleTo(const glm::vec3& scalingFactors)
{
    mScaleFactors = scalingFactors;
    updateModelMatrix();
    return;
}

void GLModel::scaleBy(const glm::vec3& scalingFactors)
{
    mScaleFactors *= scalingFactors;
    updateModelMatrix();
    return;
}

void GLModel::rotateTo(const glm::vec3& rotationAngles)
{
    mRotationQuaternion = glm::quat(rotationAngles);
    updateModelMatrix();
    return;
}

void GLModel::rotateBy(const glm::vec3& rotationAngles)
{
    // TODO: Fix rotation by angle
    mRotationQuaternion *= glm::quat(rotationAngles);
    updateModelMatrix();
    return;
}

void GLModel::updateModelMatrix()
{
    glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), mPosition);
    glm::mat4 scalingMatrix     = glm::scale(glm::mat4(1.0f), mScaleFactors);
    glm::mat4 rotationMatrix    = glm::mat4_cast(mRotationQuaternion);

    mModelMatrix = translationMatrix * rotationMatrix * scalingMatrix;

    return;
}
