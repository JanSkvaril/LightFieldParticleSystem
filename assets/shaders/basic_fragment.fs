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
vec3 light_pos = vec3(0.0, 10.0, 0.0);
void main()
{
    if (use_light) {
        vec3 norm = normalize(Normals);
        vec3 lightDir = normalize(light_pos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * vec3(0.9255, 0.9255, 0.9255);
        vec3 result = (vec3(0.6588, 0.6549, 0.6549) + diffuse) * light_color;
        // linearly interpolate between both textures (80% container, 20% awesomeface)
        FragColor = vec4(result.xyz, 1.0);
    }
    else {
        FragColor = vec4(light_color, 1.0);
    }
}