#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // linearly interpolate between both textures (80% container, 20% awesomeface)
    vec3 col = vec3(0.6 - TexCoord.x, 0.6 - TexCoord.y * TexCoord.x, sin(TexCoord.x));
    float alpha = 1.0 - distance(TexCoord, vec2(0.5, 0.5)) * 2.0;
    alpha = alpha * 2.0;
    FragColor = vec4(col.xyz, alpha);
}