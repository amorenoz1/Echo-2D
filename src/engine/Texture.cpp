#include <core/core.h>
#include "external/stb_image.h"
#include <engine/Texture.h>
#include "external/easylogging++.h"

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
      LOG(ERROR) << "[Texture] File path for texture is null.";
      return;
   }

   // Load the image with stb_image. Force 4 channels (RGBA).
   unsigned char* Pixels = stbi_load(FilePath, &Width, &Height, &Bits, 4);

   if (!Pixels) {
      LOG(ERROR) << "[Texture] Could not load texture from file: " << FilePath;
      return;
   }

   LOG(INFO) << "[Texture] Loaded texture from " << FilePath << " with dimensions: "
             << Width << "x" << Height << " and " << Bits << " bits per channel.";

   // Generate an OpenGL texture object
   glGenTextures(1, &ID);
   LOG(INFO) << "[Texture] glCreateTextures returned ID: " << ID;

   // Bind the texture to the 2D texture target
   glBindTexture(GL_TEXTURE_2D, ID);
   LOG(INFO) << "[Texture] Bound texture ID: " << ID << " to GL_TEXTURE_2D.";

   // Set texture filtering and wrapping parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   LOG(INFO) << "[Texture] Texture parameters (MIN_FILTER, MAG_FILTER, WRAP_S, WRAP_T) set to GL_NEAREST and GL_CLAMP_TO_EDGE.";

   // Upload the image data to the GPU
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Pixels);
   LOG(INFO) << "[Texture] Texture image uploaded to GPU with dimensions: "
             << Width << "x" << Height;

   // Generate mipmaps for the texture
   glGenerateMipmap(GL_TEXTURE_2D);
   LOG(INFO) << "[Texture] Mipmaps generated.";

   // Free the image data after it's been uploaded to the GPU
   stbi_image_free(Pixels);
   LOG(INFO) << "[Texture] Image data freed from memory after upload.";
}

/**
 * @brief Destructor to delete the texture from GPU memory.
 */
Texture::~Texture() {
   LOG(INFO) << "[Texture] Deleting texture ID: " << ID;
   glDeleteTextures(1, &ID);
   LOG(INFO) << "[Texture] Texture ID: " << ID << " deleted from GPU memory.";
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
   } else {
      LOG(WARNING) << "[Texture] Texture ID: " << ID << " is not a valid OpenGL texture.";
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
   } else {
      LOG(WARNING) << "[Texture] Texture ID: " << ID << " is not a valid OpenGL texture.";
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
   LOG(TRACE) << "[Texture] GetHeight called, returning height: " << Height;
   return Height; 
}

/**
 * @brief Gets the width of the texture in pixels.
 * 
 * @return The texture width.
 */
int Texture::GetWidth() const { 
   LOG(TRACE) << "[Texture] GetWidth called, returning width: " << Width;
   return Width; 
}

} // namespace Engine

