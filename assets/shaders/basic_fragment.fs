#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform mat4 view; // emittor position
void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    FragColor = vec4(TexCoord.x, 1.0, 1.0, 1.0);
}