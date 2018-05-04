#ifndef GLMODEL_HPP
#define GLMODEL_HPP

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

class GLModel
{
public:
    virtual void configure() = 0;
    virtual void draw(const glm::mat4& viewProjectionMatrix) = 0;

    void moveTo(const glm::vec3& pos);
    void moveBy(const glm::vec3& vec);

    void scaleTo(const glm::vec3& scalingFactors);
    void scaleBy(const glm::vec3& scalingFactors);

    void rotateTo(const glm::vec3& rotationAngles);
    void rotateBy(const glm::vec3& rotationAngles);

    glm::mat4 getModelMatrix() const;

protected:
    GLModel();

    void updateModelMatrix();

private:
    glm::mat4 mModelMatrix;

    glm::vec3 mPosition;
    glm::vec3 mScaleFactors;
    glm::quat mRotationQuaternion;

};

#endif //GLMODEL_HPP
