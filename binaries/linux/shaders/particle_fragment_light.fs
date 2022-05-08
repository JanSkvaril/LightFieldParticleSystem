#version 450
#extension GL_NV_bindless_texture : require
#extension GL_NV_gpu_shader5 : require

// particle shader, using real time dynamic light
// more documentation in particle_fragment.fs file
// this shader is quite slow, because it has to fetch texture 8 times
// try to mipmaping to increase performance

out vec4 FragColor;
in vec2 TexCoord;
in vec4 InterpolationPoints;
in vec3 InterpolationWeights;
flat in int ParticleTexture;
in vec3 TexPosition;

uniform int show_border;
uniform uint64_t texture_handles[5];

// calculte final part of the interpolation
float bilinear(vec3 w, vec4 q) {
    float first = q.x + (q.y - q.x) * w.x;
    float second = q.z + (q.w - q.z) * w.y;
    return first + (second - first) * w.z;
}

void main()
{
    // create sampler
    sampler2D u_texture = sampler2D(texture_handles[0]);
    vec4 col = vec4(1.0, 1.0, 1.0, 1.0);
    
    // fetch color textures
    vec4 q11 = texture(u_texture, InterpolationPoints.xy);
    vec4 q12 = texture(u_texture, vec2(InterpolationPoints.z, InterpolationPoints.y));
    vec4 q21 = texture(u_texture, vec2(InterpolationPoints.x, InterpolationPoints.a));
    vec4 q22 = texture(u_texture, InterpolationPoints.za);
    
    // interpolate color
    col.r = bilinear(InterpolationWeights, vec4(q11.r, q12.r, q21.r, q22.r));
    col.g = bilinear(InterpolationWeights, vec4(q11.g, q12.g, q21.g, q22.g));
    col.b = bilinear(InterpolationWeights, vec4(q11.b, q12.b, q21.b, q22.b));
    col.a = bilinear(InterpolationWeights, vec4(q11.a, q12.a, q21.a, q22.a));
    
    // new sampler for normals
    u_texture = sampler2D(texture_handles[1]);
    vec4 normal = vec4(1.0, 1.0, 1.0, 1.0);
    
    // fetch normals
    q11 = texture(u_texture, InterpolationPoints.xy);
    q12 = texture(u_texture, vec2(InterpolationPoints.z, InterpolationPoints.y));
    q21 = texture(u_texture, vec2(InterpolationPoints.x, InterpolationPoints.a));
    q22 = texture(u_texture, InterpolationPoints.za);
    
    // interpolate normals
    normal.r = bilinear(InterpolationWeights, vec4(q11.r, q12.r, q21.r, q22.r));
    normal.g = bilinear(InterpolationWeights, vec4(q11.g, q12.g, q21.g, q22.g));
    normal.b = bilinear(InterpolationWeights, vec4(q11.b, q12.b, q21.b, q22.b));
    normal.a = bilinear(InterpolationWeights, vec4(q11.a, q12.a, q21.a, q22.a));
    
    // calculate light
    vec3 light_color = vec3(1.0, 1.0, 1.0);
    vec3 light_pos = vec3(4.0, 0, 0);
    vec3 lightDir = normalize(light_pos - TexPosition);
    float diff = max(dot(normalize(normal.rgb), lightDir), 0.0);
    vec3 diffuse = diff * light_color;
    vec3 result = col.rgb * diffuse;
    
    FragColor = vec4(result.rgb, col.a);
    
}