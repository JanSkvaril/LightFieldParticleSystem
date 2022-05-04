#version 330 core
// basic vertex shader used for rendering model object

layout(location = 0)in vec3 aPos;
layout(location = 1)in vec3 aNormal;
layout(location = 2)in vec2 aTexCoords;

out vec2 TexCoord;
out vec3 Normals;
out vec3 FragPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   gl_Position = projection * view * model * vec4(aPos, 1.0);
   vec4 pos = view * model * vec4(aPos, 1.0);
   FragPos = vec3(0.0, 0.0, 0.0);
   TexCoord = vec2(aTexCoords);
   vec4 norm = model * vec4(aNormal, 1.0);
   Normals = norm.xyz / norm.w;
}