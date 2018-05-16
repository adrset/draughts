#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in mat4 model;
layout (location = 5) in vec4 color;
out vec4 out_color;
uniform mat4 orthoMatrix;

void main()
{
    vec2 pos = vec2(aPos.x + 1, aPos.y + 1) / 2; 
    out_color = color;
    gl_Position = orthoMatrix * model * vec4(pos, 0.0, 1.0);
}
