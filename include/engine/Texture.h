#ifndef TEXTURE_H
#define TEXTURE_H

#include <core/core.h>
#include <external/stb_image.h>

namespace Echo2D {

/**
 * @class Texture
 * @brief Manages 2D textures loaded from image files.
 *
 * Loads image data using stb_image and uploads it to the GPU as an OpenGL texture.
 */
class Texture {
public:
   /**
     * @brief Constructs a texture from a file path.
     * @param FilePath Path to the image file (PNG, JPEG, etc.).
     */
   Texture(const char* FilePath);

   Texture(int width, int height, unsigned char* data); // For glyph textures
   /// Destructor: cleans up GPU resources.
   ~Texture();

   /**
     * @brief Binds the texture to a texture unit.
     * @param slot Texture unit index (default: 0).
     */
   void Bind(GLuint slot = 0) const;

   /**
     * @brief Unbinds the texture from a texture unit.
     * @param slot Texture unit index (default: 0).
     */
   void Unbind(GLuint slot = 0) const;

   /// @return OpenGL texture ID.
   GLuint GetID() const;

   /// @return Width in pixels.
   int GetWidth() const;

   /// @return Height in pixels.
   int GetHeight() const;

private:
   GLuint m_ID = 0;      ///< OpenGL texture object ID.
   int m_Width = 0;      ///< Texture width.
   int m_Height = 0;     ///< Texture height.
   int m_Bits = 0;       ///< Number of channels (RGB = 3, RGBA = 4).
};

} // namespace Echo2D

#endif // TEXTURE_H

