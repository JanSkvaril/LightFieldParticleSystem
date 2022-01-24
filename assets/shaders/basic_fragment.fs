#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normals;
in vec3 FragPos;
// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform mat4 view; // emittor position
vec3 light_pos = vec3(0.0, 1.0, 0.0);
void main()
{
    vec3 norm = normalize(Normals);
    vec3 lightDir = normalize(light_pos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * vec3(1.0);
    vec3 result = (vec3(1.0) + diffuse) * vec3(0.451, 0.7255, 0.7725);
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    FragColor = vec4(result.xyz, 1.0);
}