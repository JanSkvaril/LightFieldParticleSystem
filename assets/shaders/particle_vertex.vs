#version 330 core
layout(location = 0)in vec3 aPos;
layout(location = 1)in vec2 aTexCoord;
layout(location = 2)in vec3 offset2;
layout(location = 3)in vec2 particle_uv;
out vec2 TexCoord;
out vec2 Offset;

uniform mat4 model; // emittor rotation
uniform mat4 view; // emittor position
uniform mat4 projection;
uniform vec3 offset;
uniform vec2 camera_angle;
uniform vec3 camera_pos;

#define PI 3.1415926538

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
    vec4 word_space = (view * model * (vec4(posi, 1.0)));
    word_space += (view * model * (vec4(offset2, 1.0)));
    vec4 screen_pos = projection * word_space;
    screen_pos = screen_pos / screen_pos.w;
    
    gl_Position = screen_pos;
    
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    
    //vec3 dir = +(camera_pos * 2.00) - offset2;
    vec2 angle = particle_uv;
    //dir = normalize(dir);
    
    // angle.x = 0.5 + atan(dir.z, dir.x) / (2.0 * PI);
    // angle.y = 0.5 + (asin(dir.y) / PI);
    
    Offset = angle.xy;
}