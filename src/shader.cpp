#include "shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
Shader::Shader(std::string vertex_path, std::string fragment_path)
{
    auto vertex_code = LoadFile(vertex_path);
    auto fragment_code = LoadFile(fragment_path);
    unsigned int vertex_shader = CreateShader(vertex_code, ShaderType::VERTEX);
    unsigned int fragment_shader = CreateShader(fragment_code, ShaderType::FRAGMENT);
    unsigned int shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);
    int success;
    char infoLog[512];
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program, 512, NULL, infoLog);
        std::cout << "[ERROR LINKING SHADER PROGRAM]\n"
                  << infoLog << std::endl;
    }
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    this->shader_program = shader_program;
}
std::string Shader::LoadFile(std::string path)
{
    std::ifstream file;
    file.open(path);
    std::stringstream str_stream;
    str_stream << file.rdbuf();
    std::string str = str_stream.str();
    file.close();
    return str;
}

unsigned int Shader::CreateShader(std::string code, ShaderType type)
{
    auto source = code.c_str();
    unsigned int shader;
    if (type == ShaderType::FRAGMENT)
    {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
    }
    else
    {
        shader = glCreateShader(GL_VERTEX_SHADER);
    }
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);
    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "[SHADER COMPILATION ERROR]:\n"
                  << infoLog << std::endl;
        exit(1);
    }
    return shader;
}

unsigned int Shader::GetShaderProgram()
{
    return shader_program;
}
