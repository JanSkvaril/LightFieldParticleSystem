#version 450
#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require
out vec4 FragColor;

in vec2 TexCoord;
in vec4 firstsecond;
in vec3 weights;
flat in int ParticleTexture;

uniform vec3 offset;
uniform mat4 view; // emittor position
uniform float u_density;
uniform int show_border;
uniform uint64_t allTheSamplers[5];
float bilinear(vec3 w, vec4 q) {
    float first = q.x + (q.y - q.x) * w.x;
    float second = q.z + (q.w - q.z) * w.y;
    return first + (second - first) * w.z;
}

void main()
{
    sampler2D u_texture = sampler2D(allTheSamplers[ParticleTexture]);
    vec4 col = vec4(1.0, 1.0, 1.0, 1.0);
    
    vec4 q11 = texture(u_texture, firstsecond.xy);
    vec4 q12 = texture(u_texture, vec2(firstsecond.z, firstsecond.y));
    vec4 q21 = texture(u_texture, vec2(firstsecond.x, firstsecond.a));
    vec4 q22 = texture(u_texture, firstsecond.za);
    
    col.r = bilinear(weights, vec4(q11.r, q12.r, q21.r, q22.r));
    col.g = bilinear(weights, vec4(q11.g, q12.g, q21.g, q22.g));
    col.b = bilinear(weights, vec4(q11.b, q12.b, q21.b, q22.b));
    col.a = bilinear(weights, vec4(q11.a, q12.a, q21.a, q22.a));
    
    if (show_border == 1) {
        if (TexCoord.x < 0.01 || TexCoord.y < 0.01 || TexCoord.x > 0.99 || TexCoord.y > 0.99) {
            col = vec4(1.0, 0.0, 0.0, 1.0);
        }
    }
    
    FragColor = col; //vec4(col.xyz, alpha);
    
    return;
    //  col.r = col.g;
}