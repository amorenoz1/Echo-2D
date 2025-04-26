#include <core/core.h>
#include "external/stb_image.h"
#include <engine/Texture.h>
#include <iostream>

namespace Engine {

/**
 * @brief Constructs a texture object from a file.
 * 
 * This constructor loads the texture from the specified file using stb_image,
 * generates an OpenGL texture, and uploads the texture data to the GPU.
 * The texture is configured with basic parameters (e.g., wrapping, filtering).
 * 
 * @param FilePath The path to the texture image file.
 */
Texture::Texture(const char* FilePath) {
   if (!FilePath) {
      std::cout << "File path for texture is null." << std::endl;
   }

   // Load the image with stb_image. Force 4 channels (RGBA).
   unsigned char* Pixels = stbi_load(FilePath, &Width, &Height, &Bits, 4);

   if (!Pixels) {
      std::cout << "Could not load texture." << std::endl;
   }

   std::cout << "[Texture] Calling glCreateTextures..." << std::endl;

   // Generate an OpenGL texture object
   glGenTextures(1, &ID);
   std::cout << "[Texture] glCreateTextures returned ID: " << ID << std::endl;

   // Bind the texture to the 2D texture target
   glBindTexture(GL_TEXTURE_2D, ID);
   std::cout << "[Texture] Bound texture" << std::endl;

   // Set texture filtering and wrapping parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   std::cout << "[Texture] Texture parameters set" << std::endl;

   // Upload the image data to the GPU
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Pixels);
   std::cout << "[Texture] Texture image uploaded" << std::endl;

   // Generate mipmaps for the texture
   glGenerateMipmap(GL_TEXTURE_2D);
   std::cout << "[Texture] Mipmaps generated" << std::endl;

   // Free the image data after it's been uploaded to the GPU
   stbi_image_free(Pixels);
}

/**
 * @brief Destructor to delete the texture from GPU memory.
 */
Texture::~Texture() {
   std::cout << "[Texture] Deleting texture: " << ID << std::endl;
   glDeleteTextures(1, &ID);
}

/**
 * @brief Binds the texture to a specified texture unit.
 * 
 * This method activates a specific texture unit (using `glActiveTexture`)
 * and binds the texture to that unit.
 * 
 * @param slot The texture unit index to bind to (default is 0).
 */
void Texture::Bind(GLuint slot) const {
   if (glIsTexture(ID)) {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, ID);
   }
}

/**
 * @brief Unbinds the texture from the specified texture unit.
 * 
 * This method unbinds the texture from the specified texture unit.
 * 
 * @param slot The texture unit index to unbind from (default is 0).
 */
void Texture::Unbind(GLuint slot) const {
   if (glIsTexture(ID)) {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, 0);
   }
}

/**
 * @brief Gets the texture's OpenGL ID.
 * 
 * @return The OpenGL texture ID.
 */
GLuint Texture::GetID() const { 
    return ID; 
}

/**
 * @brief Gets the height of the texture in pixels.
 * 
 * @return The texture height.
 */
int Texture::GetHeight() const { 
    return Height; 
}

/**
 * @brief Gets the width of the texture in pixels.
 * 
 * @return The texture width.
 */
int Texture::GetWidth() const { 
    return Width; 
}

} // namespace Engine

