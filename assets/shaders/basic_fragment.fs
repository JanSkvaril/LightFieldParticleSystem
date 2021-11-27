#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D u_texture;
uniform mat4 view; // emittor position
void main()
{
    float zoom = 5.0;
    vec2 pos = TexCoord;
    pos += 2.0;
    float m = view[3][0] * 10.0;
    
    float u = floor(m);
    
    pos /= zoom;
    vec2 first = vec2(pos.x, pos.y - u / zoom);
    float shift = 0.0;
    float frct = fract(abs(m));
    
    u += 1.0;
    
    vec2 second = vec2(pos.x, pos.y - u / zoom);
    
    vec4 col1 = texture(u_texture, first);
    vec4 col2 = texture(u_texture, second);
    
    vec4 col = mix(col1, col2, fract(m));
    
    FragColor = col; //vec4(col.xyz, alpha);
}