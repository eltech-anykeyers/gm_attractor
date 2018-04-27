#include <shader.hpp>

Shader::Shader(const char* vertexPath, const char* fragmentPath) throw(std::ifstream::failure)
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
    glDetachShader(mID, vertex);
    glDetachShader(mID, fragment);

    /**
     * Delete the shaders as they're linked into our program now
     * and no longer necessary
     */
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::use()
{
    glUseProgram(mID);
}

unsigned int Shader::getID()
{
    return mID;
}

void Shader::setBool(const std::string& name, bool value) const
{
    glUniform1i(glGetUniformLocation(mID, name.c_str()),
                static_cast<int>(value));
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(mID, name.c_str()),
                value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(mID, name.c_str()),
                value);
}

void Shader::setVec2(const std::string& name, const glm::vec2& value) const
{
    glUniform2fv(glGetUniformLocation(mID, name.c_str()),
                 1, &value[0]);
}

void Shader::setVec2(const std::string& name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(mID, name.c_str()),
                x, y);
}

void Shader::setVec3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(mID, name.c_str()),
                 1, &value[0]);
}

void Shader::setVec3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(mID, name.c_str()),
                x, y, z);
}

void Shader::setVec4(const std::string& name, const glm::vec4& value) const
{
    glUniform4fv(glGetUniformLocation(mID, name.c_str()),
                 1, &value[0]);
}

void Shader::setVec4(const std::string& name, float x, float y, float z, float w) const
{
    glUniform4f(glGetUniformLocation(mID, name.c_str()),
                x, y, z, w);
}

void Shader::setMat2(const std::string& name, const glm::mat2& mat) const
{
    glUniformMatrix2fv(glGetUniformLocation(mID, name.c_str()),
                       1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat3(const std::string& name, const glm::mat3& mat) const
{
    glUniformMatrix3fv(glGetUniformLocation(mID, name.c_str()),
                       1, GL_FALSE, &mat[0][0]);
}

void Shader::setMat4(const std::string& name, const glm::mat4& mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(mID, name.c_str()),
                       1, GL_FALSE, &mat[0][0]);
}

void Shader::checkCompileErrors(unsigned int shader, Shader::SHADER_TYPE type)
{
    int success;
    char infoLog[BUFFER_SIZE];

    if (type != SHADER_TYPE::PROGRAM)
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, BUFFER_SIZE, nullptr, infoLog);
            throw std::runtime_error(infoLog);
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, BUFFER_SIZE, nullptr, infoLog);
            throw std::runtime_error(infoLog);
        }
    }
}
