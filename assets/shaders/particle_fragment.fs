#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec2 Offset;

uniform sampler2D u_texture;
uniform vec3 offset;
uniform mat4 view; // emittor position
uniform float u_density;
float bilinear(vec4 w, vec4 q) {
    return q.r * w.r + q.g * w.g + q.b * w.b + q.a * w.a;
}
vec2 my_floor(vec2 vec) {
    if (vec.x < 0.0)vec.x = -floor(abs(vec.x));
    else vec.x = floor(vec.x);
    if (vec.y < 0.0)vec.y = -floor(abs(vec.y));
    else vec.y = floor(vec.y);
    return vec;
}
void main()
{
    vec4 col = vec4(1.0, 1.0, 1.0, 1.0);
    float zoom = u_density;
    vec2 pos = TexCoord;
    // pos += 5.0;
    float rot_speed = 1.0;
    vec2 m = vec2(Offset.x, Offset.y) * (zoom);
    
    vec2 u1 = floor(m);
    pos /= zoom;
    vec2 first = vec2(pos.x + u1.x / zoom, pos.y + u1.y / zoom);
    
    vec2 u2 = u1 + 1.0;
    if (u2.x > zoom)u2.x = 0.0;
    if (u2.y > zoom)u2.y = 0.0;
    vec2 second = vec2(pos.x + u2.x / zoom, pos.y + u2.y / zoom);
    vec2 xy = m;
    vec4 q11 = texture(u_texture, first);
    vec4 q12 = texture(u_texture, vec2(first.x, second.y));
    vec4 q21 = texture(u_texture, vec2(second.x, first.y));
    vec4 q22 = texture(u_texture, second);
    
    float x = xy.x;
    float y = xy.y;
    float w1 = (u2.x - x) * (u2.y - y) / (u2.x - u1.x) * (u2.y - u1.y);
    float w2 = (u2.x - x) * (y - u1.y) / (u2.x - u1.x) * (u2.y - u1.y);
    float w3 = (x - u1.x) * (u2.y - y) / (u2.x - u1.x) * (u2.y - u1.y);
    float w4 = (x - u1.x) * (y - u1.y) / (u2.x - u1.x) * (u2.y - u1.y);
    vec4 weights = vec4(w1, w2, w3, w4);
    col.r = bilinear(weights, vec4(q11.r, q12.r, q21.r, q22.r));
    col.g = bilinear(weights, vec4(q11.g, q12.g, q21.g, q22.g));
    col.b = bilinear(weights, vec4(q11.b, q12.b, q21.b, q22.b));
    col.a = bilinear(weights, vec4(q11.a, q12.a, q21.a, q22.a));
    FragColor = col; //vec4(col.xyz, alpha);
    return;
    //  col.r = col.g;
}