#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D u_texture;

void main()
{
    vec4 color = texture(u_texture, TexCoord);
    FragColor = color;
}