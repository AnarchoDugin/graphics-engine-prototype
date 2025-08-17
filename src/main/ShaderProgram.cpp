#include <general/ShaderProgram.h>

#include <glad/glad.h>

#include <fstream>
#include <sstream>
#include <iostream>

ShaderProgram::ShaderProgram(const std::string& name, 
                           const std::string& vertexPath,
                           const std::string& fragmentPath,
                           const std::string& geometryPath)
    : id(0), name(name)
{
    std::string vertexCode = loadShaderFile(vertexPath);
    std::string fragmentCode = loadShaderFile(fragmentPath);
    std::string geometryCode;
    
    if (!geometryPath.empty())
    {
        geometryCode = loadShaderFile(geometryPath);
    }

    unsigned int vertex = compileShader(GL_VERTEX_SHADER, vertexCode);
    unsigned int fragment = compileShader(GL_FRAGMENT_SHADER, fragmentCode);
    unsigned int geometry = 0;
    
    if (!geometryPath.empty())
    {
        geometry = compileShader(GL_GEOMETRY_SHADER, geometryCode);
    }

    id = glCreateProgram();

    glAttachShader(id, vertex);
    glAttachShader(id, fragment);

    if (geometry)
    {
        glAttachShader(id, geometry);
    }

    glLinkProgram(id);
    checkCompileErrors(id, "PROGRAM");

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    if (geometry)
    {
        glDeleteShader(geometry);
    }
    
    std::cout << "Shader program '" << name << "' created successfully\n";
}

ShaderProgram::~ShaderProgram()
{
    glDeleteProgram(id);
}

void ShaderProgram::use() const 
{
    glUseProgram(id);
}

GLint ShaderProgram::getUniformLocation(const std::string& name)
{
    auto it = uniformCache.find(name);
    if (it != uniformCache.end())
    {
        return it->second;
    }
    
    int location = glGetUniformLocation(id, name.c_str());
    
    if (location == -1)
    {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader '" << this->name << "'\n";
    }

    uniformCache[name] = location;
    return location;
}

void ShaderProgram::setBool(const std::string& name, bool value)
{
    glUniform1i(getUniformLocation(name), static_cast<int>(value));
}

void ShaderProgram::setInt(const std::string& name, int value)
{
    glUniform1i(getUniformLocation(name), value);
}

void ShaderProgram::setFloat(const std::string& name, float value)
{
    glUniform1f(getUniformLocation(name), value);
}

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value)
{
    glUniform2fv(getUniformLocation(name), 1, &value[0]);
}

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value)
{
    glUniform3fv(getUniformLocation(name), 1, &value[0]);
}

void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value)
{
    glUniform4fv(getUniformLocation(name), 1, &value[0]);
}

void ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat)
{
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat)
{
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat)
{
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

std::string ShaderProgram::loadShaderFile(const std::string& filepath)
{
    std::ifstream file(filepath);

    if (!file)
    {
        std::cerr << "Failed to open shader file: " + filepath << std::endl;
    }
    
    std::stringstream buffer;

    buffer << file.rdbuf();
    return buffer.str();
}

unsigned int ShaderProgram::compileShader(unsigned int type, const std::string& source)
{
    unsigned int shader = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(shader, 1, &src, nullptr);
    glCompileShader(shader);

    checkCompileErrors(shader, type == GL_VERTEX_SHADER ? "VERTEX" : 
                       type == GL_FRAGMENT_SHADER ? "FRAGMENT" : "GEOMETRY");
    return shader;
}

void ShaderProgram::checkCompileErrors(unsigned int shader, const std::string& type)
{
    int success;
    char infoLog[1024];
    
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "Shader compilation error (" + type + "):\n" << infoLog << std::endl;
        }

    }

    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "Shader program linking error:\n" << infoLog << std::endl;
        }
    }
}