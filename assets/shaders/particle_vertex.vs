#version 330 core
layout(location = 0)in vec3 aPos;
layout(location = 1)in vec2 aTexCoord;
layout(location = 2)in vec3 offset2;
out vec2 TexCoord;
out vec2 Offset;

uniform mat4 model; // emittor rotation
uniform mat4 view; // emittor position
uniform mat4 projection;
uniform vec3 offset;
uniform vec2 camera_angle;
uniform vec3 camera_pos;

#define PI 3.1415926538

// float atan2(float y, float x)
// {
    //     bool s = (abs(x) > abs(y));
    //     return mix(PI / 2.0 - atan(x, y), atan(y, x), s);
// }
float ATan2(vec2 dir)
{
    float angle = asin(dir.x) > 0 ? acos(dir.y) : -acos(dir.y);
    return angle;
}
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
    //camera_position = abs(camera_pos.xyz);
    //camera_position = camera_position - (11.0 * floor(camera_position / 11.0));
    // vec2 plane = vec2((camera_position.x / (1.0 - camera_position.z)), camera_position.y / (1.0 - camera_position.z));
    // Offset = plane;
    //vec3 camera_dir = camera_pos;
    // vec2 angle = vec2(atan(camera_dir.z, camera_dir.x), atan(camera_dir.z, camera_dir.y));
    //angle /= 3.14159265359;
    //angle += 1.0;
    vec3 dir = camera_pos - offset2;
    vec2 angle = camera_angle;
    dir = normalize(dir);
    
    vec4 S = (view * vec4(offset2, 1.0));
    S /= S.w;
    // source: https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.90.6202&rep=rep1&type=pdf
    angle.x -= 0.1 * ((S.x / (1.0 + S.z)) + 1.0);
    //  angle.y = -angle.y;
    angle.y += 0.05 * ((S.y / (1.0 + S.z)) + 1.0);
    angle.y = - angle.y;
    // angle.x = (atan(dir.z, dir.x) + PI) / (PI * 2.0);
    
    //angle.x = (atan((dir.z), (dir.x)) + PI) / (PI * 2.0);
    
    // if (angle.y < 0.25) {
        
    // }
    // else if (angle.y < 0.75) {
        //     angle.x = 0.5 + angle.x;
    // }
    
    // if (angle.y < 0.25) {
        //     angle.y = dir.y * 0.25 + 1.0;
    // }
    // else if (angle.y < 0.75)
    // {
        //     angle.y = 0.5 - (dir.y * 0.25);
    // }
    // else {
        //     angle.y = abs(dir.y * 0.25);
    // }
    
    // else {
        //     angle.y = 0.314  (dir.y / 3.14);
    // }
    
    // angle.x -= (dir.x + dir.z) * (0.25 * 0.5);
    if (angle.y < 0.0) {
        angle.y = 1.0 + angle.y;
    }
    else if (angle.y > 1.0) {
        angle.y = angle.y - 1.0;
    }
    
    if (angle.x < 0.0) {
        angle.x = 1.0 + angle.x;
    }
    else if (angle.x > 1.0) {
        angle.x = angle.x - 1.0;
    }
    angle.y = 1.0 - angle.y;
    Offset = angle.xy;
}