#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
uniform mat4 model;
uniform mat4 orthoMatrix;

out vec2 texPos;
void main()
{
    vec2 pos = vec2(aPos.x + 1, aPos.y + 1) / 2;
    texPos = aTex;
    gl_Position = orthoMatrix * model * vec4(pos.x, pos.y, 0.0, 1.0);
}
