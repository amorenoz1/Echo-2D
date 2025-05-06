#ifndef SPRITESHEET_H
#define SPRITESHEET_H

#include <engine/Texture.h>
#include <glm/glm.hpp>
#include <string>

namespace Echo2D {

/**
 * @class Spritesheet
 * @brief Manages a texture containing a grid of sprites, and provides access to their texture coordinates.
 *
 * This class simplifies the process of retrieving UV coordinates for individual sprite tiles
 * within a larger spritesheet texture, based on a fixed tile size.
 */
class Spritesheet {
public:
   /**
     * @brief Constructs a Spritesheet from a texture file and individual sprite dimensions.
     * @param Filepath Path to the image file containing the spritesheet.
     * @param SpriteWidth Width of a single sprite in pixels.
     * @param SpriteHeight Height of a single sprite in pixels.
     */
   Spritesheet(std::string Filepath, int SpriteWidth, int SpriteHeight, float FrameInterval, int SpriteAmount);

   /**
     * @brief Destructor that releases texture resources.
     */
   ~Spritesheet();

   /**
     * @brief Retrieves the normalized texture coordinates for a sprite at grid position (i, j).
     * @param i Column index of the sprite (0-based).
     * @param j Row index of the sprite (0-based).
     * @return A glm::vec4 representing (u, v, width, height) in normalized texture space.
     */
   glm::vec4 GetTexCoords(int i, int j);

   /**
     * @brief Gets the texture used by this spritesheet.
     * @return A reference to the underlying Texture object.
     */
   Texture& GetTex();

   void SpriteUpdate(float dt);

   int GetCount();
private:
   Texture *m_TextureMap;        /**< Pointer to the texture representing the full spritesheet. */
   float m_SpriteWidthRatio;     /**< Width of a single sprite in normalized texture coordinates. */
   float m_SpriteHeightRatio;    /**< Height of a single sprite in normalized texture coordinates. */
   float m_TexCoordsOriginX;     /**< X origin of the current sprite in normalized coordinates (unused in final version). */
   float m_TexCoordsOriginY;     /**< Y origin of the current sprite in normalized coordinates (unused in final version). */
   float m_TexCoordsWidth;       /**< Width in normalized coordinates (unused in final version). */
   float m_TexCoordsHeight;      /**< Height in normalized coordinates (unused in final version). */
   float m_FrameInterval = 0.1f;
   float m_Timer = 0.0f;
   int m_Count = 0;
   int m_SpriteAmount = 0;
};

} // namespace Echo2D

#endif // SPRITESHEET_H

