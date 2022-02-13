#ifndef TEXTURE_CLASS_DEF
#define TEXTURE_CLASS_DEF

#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// #include <stb_image/stb_image.h>
class Texture
{
public:
    Texture(std::string path);
    void Bind();

protected:
    unsigned char *data;
    unsigned int texture;
    int width, height = 0;
};

#endif