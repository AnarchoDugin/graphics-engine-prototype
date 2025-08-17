#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <unordered_map>

#include <glm/glm.hpp>

class ShaderProgram 
{
private:
    unsigned int id;

    std::string name;
    std::unordered_map<std::string, int> uniformCache;
    
    int getUniformLocation(const std::string& name);
    
public:
    ShaderProgram(const std::string& name, 
                 const std::string& vertexShaderFile,
                 const std::string& fragmentShaderFile,
                 const std::string& geometryShaderFile = "");
    
    ~ShaderProgram();
    
    void use() const;
    
    void setBool(const std::string& name, bool value);
    void setInt(const std::string& name, int value);
    void setFloat(const std::string& name, float value);

    void setVec2(const std::string& name, const glm::vec2& value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setVec4(const std::string& name, const glm::vec4& value);

    void setMat2(const std::string& name, const glm::mat2& mat);
    void setMat3(const std::string& name, const glm::mat3& mat);
    void setMat4(const std::string& name, const glm::mat4& mat);
    
private:
    static std::string loadShaderFile(const std::string& fileName);
    static unsigned int compileShader(unsigned int type, const std::string& source);
    static void checkCompileErrors(unsigned int shader, const std::string& type);
};

#endif