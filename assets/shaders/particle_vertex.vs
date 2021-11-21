#version 330 core
layout(location = 0)in vec3 aPos;
layout(location = 1)in vec2 aTexCoord;

out vec2 TexCoord;

uniform mat4 model; // emittor rotation
uniform mat4 view; // emittor position
uniform mat4 projection;
uniform vec3 offset;
void main()
{
    vec3 camera_normal = vec3(view[0][2], view[1][2], view[2][2]);
    // camera_normal = - camera_normal;
    vec3 up = vec3(view[0][0], view[1][0], view[2][0]);
    vec3 r = cross(up, camera_normal);
    r = normalize(r);
    vec3 new_up = cross(camera_normal , r);
    mat3 rot = mat3(r, new_up, camera_normal);
    vec3 posi = rot * aPos;
    vec3 new_offset = rot * offset.xyz;
    vec4 word_space = (view * model * vec4(posi, 1.0) + vec4(new_offset, 1.0));
    vec4 screen_pos = projection * word_space;
    screen_pos = screen_pos / screen_pos.w;
    
    gl_Position = screen_pos;
    
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
}