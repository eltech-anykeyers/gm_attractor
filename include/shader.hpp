#ifndef SHADER_HPP
#define SHADER_HPP

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <glad/glad.h>

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath) throw(std::ifstream::failure)
    {
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        /// Set exceptions.
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        /// Open files.
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;

        /// Read from files. Can throw an exception.
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();

        /// Close files.
        vShaderFile.close();
        fShaderFile.close();

        /// Convert to strings.
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();

        /// Convert to raw strings.
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();

        unsigned int vertex, fragment;

        /// Vertex shader.
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, nullptr);
        glCompileShader(vertex);
        checkCompileErrors(vertex, SHADER_TYPE::VERTEX);

        /// Fragment shader.
        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, nullptr);
        glCompileShader(fragment);
        checkCompileErrors(fragment, SHADER_TYPE::FRAGMENT);

        /// Shader program.
        mID = glCreateProgram();
        glAttachShader(mID, vertex);
        glAttachShader(mID, fragment);
        glLinkProgram(mID);
        checkCompileErrors(mID, SHADER_TYPE::PROGRAM);

        /**
         * Delete the shaders as they're linked into our program now
         * and no longer necessary
         */
        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    void use()
    {
        glUseProgram(mID);
    }

    unsigned int getID()
    {
        return mID;
    }

    void setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(mID, name.c_str()),
                    static_cast<int>(value));
    }

    void setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(mID, name.c_str()),
                    value);
    }

    void setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(mID, name.c_str()),
                    value);
    }

private:
    constexpr static const int BUFFER_SIZE = 1024;

    enum SHADER_TYPE
    {
        VERTEX,
        FRAGMENT,
        PROGRAM
    };

    unsigned int mID;

    void checkCompileErrors(unsigned int shader, SHADER_TYPE type)
    {
        int success;
        char infoLog[BUFFER_SIZE];

        if (type != SHADER_TYPE::PROGRAM)
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
            {
                glGetShaderInfoLog(shader, BUFFER_SIZE, nullptr, infoLog);
                /// TODO: Show or throw something.
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
            {
                glGetProgramInfoLog(shader, BUFFER_SIZE, nullptr, infoLog);
                /// TODO: Show or throw something.
            }
        }
    }
};

#endif // SHADER_HPP
