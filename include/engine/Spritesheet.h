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
   Spritesheet(std::string Filepath, int SpriteWidth, int SpriteHeight);

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

private:
   Texture *TextureMap;        /**< Pointer to the texture representing the full spritesheet. */
   float SpriteWidthRatio;     /**< Width of a single sprite in normalized texture coordinates. */
   float SpriteHeightRatio;    /**< Height of a single sprite in normalized texture coordinates. */
   float TexCoordsOriginX;     /**< X origin of the current sprite in normalized coordinates (unused in final version). */
   float TexCoordsOriginY;     /**< Y origin of the current sprite in normalized coordinates (unused in final version). */
   float TexCoordsWidth;       /**< Width in normalized coordinates (unused in final version). */
   float TexCoordsHeight;      /**< Height in normalized coordinates (unused in final version). */
};

} // namespace Echo2D

#endif // SPRITESHEET_H

