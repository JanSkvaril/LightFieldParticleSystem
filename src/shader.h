#ifndef SHADER_CLASS_DEF
#define SHADER_CLASS_DEF

#include <string>
class Shader
{
public:
    Shader(std::string vertex_path, std::string fragment_path);
    unsigned int GetShaderProgram();

protected:
    enum class ShaderType
    {
        VERTEX,
        FRAGMENT
    };
    std::string LoadFile(std::string path);
    unsigned int CreateShader(std::string code, ShaderType type);
    unsigned int shader_program;
};

#endif