#include <engine/Spritesheet.h>
#include <engine/Texture.h>

namespace Echo2D {

/**
 * @class Spritesheet
 * @brief Represents a 2D spritesheet texture composed of uniformly sized sprite tiles.
 *
 * Provides utilities to retrieve texture coordinates for individual sprites based
 * on their grid position.
 */

Spritesheet::Spritesheet(const std::string filepath, int spriteWidth, int spriteHeight, float FrameInterval, int SpriteAmount)
   : m_TextureMap(new Texture(filepath.c_str())),
   m_SpriteWidthRatio(static_cast<float>(spriteWidth) / m_TextureMap->GetWidth()),
   m_SpriteHeightRatio(static_cast<float>(spriteHeight) / m_TextureMap->GetHeight()), m_FrameInterval(FrameInterval), m_SpriteAmount(SpriteAmount) {}

Spritesheet::~Spritesheet() {
   delete m_TextureMap;
}

glm::vec4 Spritesheet::GetTexCoords(int i, int j) {
   float u = i * m_SpriteWidthRatio;
   float v = j * m_SpriteHeightRatio;
   return {u, v, m_SpriteWidthRatio, m_SpriteHeightRatio}; // (u, v, width, height)
}

Texture& Spritesheet::GetTex() {
   return *m_TextureMap;
}

void Spritesheet::SpriteUpdate(float dt) {
   m_Timer += dt;
   if (m_Timer >= m_FrameInterval) {
      m_Count = (m_Count + 1) % m_SpriteAmount;
      m_Timer = 0.0f;
   }
}

int Spritesheet::GetCount() {
   return m_Count;
}

}; // namespace Echo2D

