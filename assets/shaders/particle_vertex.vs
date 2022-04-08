#version 330 core
layout(location = 0)in vec3 aPos;
layout(location = 1)in vec2 aTexCoord;
layout(location = 2)in vec3 offset2;
layout(location = 3)in vec2 particle_uv;
layout(location = 4)in int particle_texture;
out vec2 TexCoord;
out vec4 firstsecond;
out vec3 weights;
flat out int ParticleTexture;
out vec3 TexPosition;

uniform mat4 model; // emittor rotation
uniform mat4 view; // emittor position
uniform mat4 projection;
uniform vec2 camera_angle;
uniform vec3 camera_pos;
uniform float u_density;
#define PI 3.1415926538

void main()
{
    ParticleTexture = particle_texture;
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
    TexPosition = (word_space / word_space.w).xyz;
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    
    //vec3 dir = +(camera_pos * 2.00) - offset2;
    vec2 angle = particle_uv;
    //dir = normalize(dir);
    
    // angle.x = 0.5 + atan(dir.z, dir.x) / (2.0 * PI);
    // angle.y = 0.5 + (asin(dir.y) / PI);
    
    float zoom = u_density;
    vec2 pos = aTexCoord.xy;
    vec2 m = vec2(angle.x, angle.y) * (zoom);
    
    vec2 u1 = floor(m);
    pos /= zoom;
    vec2 first = vec2(pos.x + u1.x / zoom, pos.y + u1.y / zoom);
    
    vec2 u2 = u1 + 1.0;
    vec2 second = vec2(pos.x + u2.x / zoom, pos.y + u2.y / zoom);
    firstsecond = vec4(first.xy, second.xy);
    vec2 xy = m;
    float x = xy.x;
    float y = xy.y;
    // float w1 = (u2.x - x) * (u2.y - y) / (u2.x - u1.x) * (u2.y - u1.y);
    // float w2 = (u2.x - x) * (y - u1.y) / (u2.x - u1.x) * (u2.y - u1.y);
    // float w3 = (x - u1.x) * (u2.y - y) / (u2.x - u1.x) * (u2.y - u1.y);
    // float w4 = (x - u1.x) * (y - u1.y) / (u2.x - u1.x) * (u2.y - u1.y);
    
    vec2 A = vec2(u1);
    vec2 B = vec2(u2.x, u1.y);
    vec2 C = vec2(u1.x, u2.y);
    vec2 D = vec2(u2);
    
    vec2 Qab = vec2(x, A.y);
    vec2 Qcd = vec2(x, C.y);
    
    vec2 Q = vec2(x, y);
    
    float w1 = length(Qab - A) / length(B - A);
    float w2 = length(Qcd - C) / length(D - C);
    float w3 = length(Q - Qab) / length(Qcd - Qab);
    weights = vec3(w1, w2, w3);
}