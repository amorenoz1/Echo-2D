#include <core/core.h>
#include "external/stb_image.h"
#include <engine/Texture.h>
#include "external/easylogging++.h"

namespace Echo2D {

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
   unsigned char* Pixels = stbi_load(FilePath, &m_Width, &m_Height, &m_Bits, 4);

   if (!Pixels) {
      LOG(ERROR) << "[Texture] Could not load texture from file: " << FilePath;
      return;
   }

   LOG(INFO) << "[Texture] Loaded texture from " << FilePath << " with dimensions: "
             << m_Width << "x" << m_Height << " and " << m_Bits << " bits per channel.";

   // Generate an OpenGL texture object
   glGenTextures(1, &m_ID);
   LOG(INFO) << "[Texture] glCreateTextures returned ID: " << m_ID;

   // Bind the texture to the 2D texture target
   glBindTexture(GL_TEXTURE_2D, m_ID);
   LOG(INFO) << "[Texture] Bound texture ID: " << m_ID << " to GL_TEXTURE_2D.";

   // Set texture filtering and wrapping parameters
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   LOG(INFO) << "[Texture] Texture parameters (MIN_FILTER, MAG_FILTER, WRAP_S, WRAP_T) set to GL_NEAREST and GL_CLAMP_TO_EDGE.";

   // Upload the image data to the GPU
   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Pixels);
   LOG(INFO) << "[Texture] Texture image uploaded to GPU with dimensions: "
             << m_Width << "x" << m_Height;

   // Generate mipmaps for the texture
   glGenerateMipmap(GL_TEXTURE_2D);
   LOG(INFO) << "[Texture] Mipmaps generated.";

   // Free the image data after it's been uploaded to the GPU
   stbi_image_free(Pixels);
   LOG(INFO) << "[Texture] Image data freed from memory after upload.";
}

Texture::Texture(int width, int height, unsigned char* data) 
    : m_Width(width), m_Height(height) {
    
   glGenTextures(1, &m_ID);
   glBindTexture(GL_TEXTURE_2D, m_ID);
   glTexImage2D(
      GL_TEXTURE_2D,
      0,
      GL_RGBA,
      width,
      height,
      0,
      GL_RED,
      GL_UNSIGNED_BYTE,
      data
   );
   // set texture options
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_R, GL_RED);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_G, GL_RED);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_B, GL_RED);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_SWIZZLE_A, GL_RED);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

Texture::~Texture() {
   LOG(INFO) << "[Texture] Deleting texture ID: " << m_ID;
   glDeleteTextures(1, &m_ID);
   LOG(INFO) << "[Texture] Texture ID: " << m_ID << " deleted from GPU memory.";
}

void Texture::Bind(GLuint slot) const {
   if (glIsTexture(m_ID)) {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, m_ID);
   } else {
      LOG(WARNING) << "[Texture] Texture ID: " << m_ID << " is not a valid OpenGL texture.";
   }
}

void Texture::Unbind(GLuint slot) const {
   if (glIsTexture(m_ID)) {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, 0);
   } else {
      LOG(WARNING) << "[Texture] Texture ID: " << m_ID << " is not a valid OpenGL texture.";
   }
}

GLuint Texture::GetID() const { 
   return m_ID; 
}

int Texture::GetHeight() const { 
   LOG(TRACE) << "[Texture] GetHeight called, returning height: " << m_Height;
   return m_Height; 
}

int Texture::GetWidth() const { 
   LOG(TRACE) << "[Texture] GetWidth called, returning width: " << m_Width;
   return m_Width; 
}

} // namespace Echo2D

