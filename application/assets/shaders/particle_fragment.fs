#version 450
#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require

// this is light field particle shader
// it interpolates final color from given samples

// output color
out vec4 FragColor;

// tex coordinates
in vec2 TexCoord;
// interpolation points (A and C). B and D are derived from these points, viz. thesis
in vec4 InterpolationPoints;
// calculated and interpolated weights for interplation
in vec3 InterpolationWeights;
// particle texture ID
flat in int ParticleTexture;

// if true, border around particle will be shown
uniform int show_border;

// bindless texture handles
// maximum amount is 5, but can be easly increased
uniform uint64_t texture_handles[5];

// calculate final part of the interpolation
float bilinear(vec3 w, vec4 q) {
    float first = q.x + (q.y - q.x) * w.x;
    float second = q.z + (q.w - q.z) * w.y;
    return first + (second - first) * w.z;
}

void main()
{
    // create texture sampler
    sampler2D u_texture = sampler2D(texture_handles[ParticleTexture]);
    vec4 col = vec4(1.0, 1.0, 1.0, 1.0);
    
    // fetch textures from given points
    vec4 q11 = texture(u_texture, InterpolationPoints.xy);
    vec4 q12 = texture(u_texture, vec2(InterpolationPoints.z, InterpolationPoints.y));
    vec4 q21 = texture(u_texture, vec2(InterpolationPoints.x, InterpolationPoints.a));
    vec4 q22 = texture(u_texture, InterpolationPoints.za);
    
    // interpolate
    col.r = bilinear(InterpolationWeights, vec4(q11.r, q12.r, q21.r, q22.r));
    col.g = bilinear(InterpolationWeights, vec4(q11.g, q12.g, q21.g, q22.g));
    col.b = bilinear(InterpolationWeights, vec4(q11.b, q12.b, q21.b, q22.b));
    col.a = bilinear(InterpolationWeights, vec4(q11.a, q12.a, q21.a, q22.a));
    
    // show border if specified
    // remove this to increase performance!
    if (show_border == 1) {
        if (TexCoord.x < 0.01 || TexCoord.y < 0.01 || TexCoord.x > 0.99 || TexCoord.y > 0.99) {
            col = vec4(1.0, 0.0, 0.0, 1.0);
        }
    }
    
    FragColor = col;
    
}