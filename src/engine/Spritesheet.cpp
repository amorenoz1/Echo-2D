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

/**
 * @brief Constructs a Spritesheet object.
 * @param filepath Path to the spritesheet image file.
 * @param spriteWidth Width of a single sprite in pixels.
 * @param spriteHeight Height of a single sprite in pixels.
 * @param FrameInterval Amount of time per frame of sprite animation
 */
Spritesheet::Spritesheet(const std::string filepath, int spriteWidth, int spriteHeight, float FrameInterval, int SpriteAmount)
   : TextureMap(new Texture(filepath.c_str())),
   SpriteWidthRatio(static_cast<float>(spriteWidth) / TextureMap->GetWidth()),
   SpriteHeightRatio(static_cast<float>(spriteHeight) / TextureMap->GetHeight()), FrameInterval(FrameInterval), SpriteAmount(SpriteAmount) {}

/**
 * @brief Destructor. Cleans up allocated texture.
 */
Spritesheet::~Spritesheet() {
   delete TextureMap;
}

/**
 * @brief Retrieves the texture coordinates for a sprite at grid position (i, j).
 * 
 * @param i Column index of the sprite in the spritesheet (0-based).
 * @param j Row index of the sprite in the spritesheet (0-based).
 * @return glm::vec4 (u, v, width, height) representing texture coordinates in normalized space [0, 1].
 */
glm::vec4 Spritesheet::GetTexCoords(int i, int j) {
   float u = i * SpriteWidthRatio;
   float v = j * SpriteHeightRatio;
   return {u, v, SpriteWidthRatio, SpriteHeightRatio}; // (u, v, width, height)
}

/**
 * @brief Gets the texture object associated with this spritesheet.
 * 
 * @return Reference to the Texture.
 */
Texture& Spritesheet::GetTex() {
   return *TextureMap;
}

/**
 * @brief Using dt argument makes timer an accumulator. 
 */
void Spritesheet::SpriteUpdate(float dt) {
   Timer += dt;
   if (Timer >= FrameInterval) {
      Count = (Count + 1) % SpriteAmount;
      Timer = 0.0f;
   }
}

int Spritesheet::GetCount() {
   return Count;
}

}; // namespace Echo2D

