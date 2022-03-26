#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normals;
in vec3 FragPos;
// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform mat4 view; // emittor position
uniform vec3 light_color;
uniform bool use_light;
vec3 light_pos = vec3(0.0, 1.0, 0.0);
void main()
{
    vec3 norm = normalize(Normals);
    FragColor = vec4(norm.xyz, 1.0);
}