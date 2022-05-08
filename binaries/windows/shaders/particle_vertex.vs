#version 330 core

// this shader is used for light field particles
// particles are billboards, so vertecies must but rotated by given rotation matrix M
// shader also handles first part of the interpolation, vit. thesis

// vertex position
layout(location = 0)in vec3 VPosition;
// rectangle texture position,- interpolated for fragment shader
layout(location = 1)in vec2 aTexCoord;
// particle world position vector
layout(location = 2)in vec3 ParticlePosition;
// calculated (u,v) coordinates for particle
layout(location = 3)in vec2 ParticleUV;
// index of texture handle
layout(location = 4)in int ParticleTextureID;

out vec2 TexCoord;
out vec4 InterpolationPoints;
out vec3 InterpolationWeights;
flat out int ParticleTexture;
out vec3 TexPosition;

uniform mat4 viewmodel; // view and model tranformation matrix
uniform mat4 projection; // projection matrix
uniform float u_density; // texture density
uniform mat3 rot; // rotation matrix M

// constants
#define PI 3.1415926538

void main()
{
    ParticleTexture = ParticleTextureID;
    
    // === billboard rotatin and position ===
    
    // calculate vertex position
    vec3 vertex_position = rot * VPosition;
    vec4 word_space = (viewmodel * (vec4(vertex_position, 1.0)));
    // particle world position
    word_space += (viewmodel * (vec4(ParticlePosition, 1.0)));
    // screen position
    vec4 screen_pos = projection * word_space;
    screen_pos = screen_pos / screen_pos.w;
    gl_Position = screen_pos;
    // interpolate positiion
    TexPosition = (word_space / word_space.w).xyz;
    // interpolate texture position
    TexCoord = vec2(aTexCoord.x, aTexCoord.y);
    
    // === interpolation ===
    float zoom = u_density;
    vec2 pos = aTexCoord.xy;
    vec2 uv_zoomed = vec2(ParticleUV.x, ParticleUV.y) * (zoom);
    
    // get x, y and points, viz. thesis
    vec2 u1 = floor(uv_zoomed);
    pos /= zoom;
    vec2 first_point = vec2(pos.x + u1.x / zoom, pos.y + u1.y / zoom);
    vec2 u2 = u1 + 1.0;
    vec2 second_point = vec2(pos.x + u2.x / zoom, pos.y + u2.y / zoom);
    InterpolationPoints = vec4(first_point.xy, second_point.xy);
    vec2 xy = uv_zoomed;
    float x = xy.x;
    float y = xy.y;
    
    // interpolation points
    vec2 A = vec2(u1);
    vec2 B = vec2(u2.x, u1.y);
    vec2 C = vec2(u1.x, u2.y);
    vec2 D = vec2(u2);
    vec2 Qab = vec2(x, A.y);
    vec2 Qcd = vec2(x, C.y);
    vec2 Q = vec2(x, y);
    
    // calculate weights
    float w1 = length(Qab - A) / length(B - A);
    float w2 = length(Qcd - C) / length(D - C);
    float w3 = length(Q - Qab) / length(Qcd - Qab);
    // weights are interpolated for fragment shader
    InterpolationWeights = vec3(w1, w2, w3);
}