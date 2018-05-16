#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 orthoMatrix;
void main()
{
      vec2 pos = vec2(aPos.x + 1, aPos.y + 1) / 2; 
    gl_Position = orthoMatrix * model * vec4(pos.x, pos.y, 0.0, 1.0);
}
