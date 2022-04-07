#version 330 core
layout(location = 0)in vec3 aPos;
layout(location = 1)in vec3 aNormal;
layout(location = 2)in vec2 aTexCoords;
layout(location = 3)in vec3 particle_position;
out vec2 TexCoord;
out vec3 Normals;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vec3 pos = aPos;
    
    vec4 word_space = view * model * vec4(pos, 1.0);
    word_space += view * vec4(particle_position.xyz, 1.0);
    vec4 screen_pos = projection * word_space;
    gl_Position = screen_pos;
    FragPos = vec3(model * vec4(pos, 1.0));
    TexCoord = vec2(aTexCoords);
    Normals = aNormal;
}