#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D u_texture;
uniform mat4 view; // emittor position
float bilinear(float x, float y, vec2 p1, vec2 p2, vec4 q) {
    float w1 = (p2.x - x) * (p2.y - y) / (p2.x - p1.x) * (p2.y - p1.y);
    float w2 = (p2.x - x) * (y - p1.y) / (p2.x - p1.x) * (p2.y - p1.y);
    float w3 = (x - p1.x) * (p2.y - y) / (p2.x - p1.x) * (p2.y - p1.y);
    float w4 = (x - p1.x) * (y - p1.y) / (p2.x - p1.x) * (p2.y - p1.y);
    return q.x * w1 + q.y * w2 + q.z * w3 + q.w * w4;
}
void main()
{
    vec4 col = vec4(1.0);
    float zoom = 5.0;
    vec2 pos = TexCoord;
    pos += 2.0;
    vec2 m = vec2(view[3][0] * 10.0, view[3][1] * 10.0);
    
    vec2 u1 = floor(m);
    
    pos /= zoom;
    vec2 first = vec2(pos.x - u1.x / zoom, pos.y - u1.y / zoom);
    
    vec2 u2 = u1 + 1.0;
    
    vec2 second = vec2(pos.x - u2.x / zoom, pos.y - u2.y / zoom);
    
    vec2 xy = m;
    vec4 q11 = texture(u_texture, first);
    vec4 q12 = texture(u_texture, vec2(first.x, second.y));
    vec4 q21 = texture(u_texture, vec2(second.x, first.y));
    vec4 q22 = texture(u_texture, second);
    
    col.r = bilinear(xy.x, xy.y, u1, u2, vec4(q11.r, q12.r, q21.r, q22.r));
    col.g = bilinear(xy.x, xy.y, u1, u2, vec4(q11.g, q12.g, q21.g, q22.g));
    col.b = bilinear(xy.x, xy.y, u1, u2, vec4(q11.b, q12.b, q21.b, q22.b));
    col.a = bilinear(xy.x, xy.y, u1, u2, vec4(q11.a, q12.a, q21.a, q22.a));
    
    FragColor = col; //vec4(col.xyz, alpha);
}