#ifndef ATTRACTORGLAPP_HPP
#define ATTRACTORGLAPP_HPP

#include <iglapp.hpp>

class AttractorGLApp : public IGLApp
{
public:
    AttractorGLApp(GLint width, GLint height, const std::string& title)
        : IGLApp(width, height, title)
    {}

protected:
    virtual void render() override;
};

#endif // ATTRACTORGLAPP_HPP
