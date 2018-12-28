#version 330 core
out vec4 FragColor;
in vec2 texPos;
uniform vec3 color;
uniform sampler2D textureSampler;
uniform float useTexture;
void main(){
    if(useTexture == 1.0)
	FragColor = texture(textureSampler, texPos);
    else
        FragColor = vec4(color, 1.0f);
}
