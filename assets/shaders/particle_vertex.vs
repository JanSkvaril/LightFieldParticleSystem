#version 330 core
layout(location = 0)in vec3 aPos;
layout(location = 1)in vec2 aTexCoord;
layout(location = 2)in vec3 offset2;
out vec2 TexCoord;
out vec2 Offset;
uniform mat4 model; // emittor rotation
uniform mat4 view; // emittor position
uniform mat4 projection;
uniform vec3 offset;
void main()
{
    vec3 camera_normal = vec3(view[0][2], view[1][2], view[2][2]);
    camera_normal = - camera_normal;
    vec3 up = vec3(view[0][1], view[1][1], view[2][1]);
    vec3 r = cross(camera_normal, up);
    r = normalize(r);
    vec3 new_up = cross(camera_normal , r);
    mat3 rot = mat3(r, new_up, camera_normal);
    vec3 posi = rot * aPos;
    vec3 new_offset = rot * offset2.xyz;
    vec4 word_space = (view * model * vec4(posi, 1.0) + vec4(new_offset, 1.0));
    vec4 screen_pos = projection * word_space;
    screen_pos = screen_pos / screen_pos.w;
    
    gl_Position = screen_pos;
    
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    Offset = offset2.xy;
}