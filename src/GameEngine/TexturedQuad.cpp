#include "TexturedQuad.h"

namespace GameEngine{



	TexturedQuad::TexturedQuad(float *vertices, unsigned int *indices, size_t vSize, size_t iSize, glm::vec2 position, glm::vec3 color, float scale, std::string texturePath):  Quad (vertices, indices, vSize, iSize, scale), m_texture(new Texture(texturePath)){

}







}

