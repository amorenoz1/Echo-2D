#include <core/core.h>
#include "external/stb_image.h"
#include <engine/Texture.h>
#include <iostream>

namespace Engine {

Texture::Texture(const char* FilePath) {
   if (!FilePath) {
      std::cout << "File path for texture is null." << std::endl;
   }

   unsigned char* Pixels = stbi_load(FilePath, &Width, &Height, &Bits, 4);

   if (!Pixels) {
      std::cout << "Could not load texture." << std::endl;
   }

   printf("[TextureLoader] Calling glCreateTextures...\n");

   glGenTextures(1, &ID);
   printf("[TextureLoader] glCreateTextures returned ID: %u\n", ID);

   glBindTexture(GL_TEXTURE_2D, ID);
   printf("[TextureLoader] Bound texture\n");

   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   printf("[TextureLoader] Texture parameters set\n");

   glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Pixels);
   printf("[TextureLoader] Texture image uploaded\n");

   glGenerateMipmap(GL_TEXTURE_2D);
   printf("[TextureLoader] Mipmaps generated\n");

   stbi_image_free(Pixels);
}

Texture::~Texture() {}

void Texture::Bind(GLuint slot) const {
   if (glIsTexture(ID)) {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, ID);
   }
}

void Texture::Unbind(GLuint slot) const {
   if (glIsTexture(ID)) {
      glActiveTexture(GL_TEXTURE0 + slot);
      glBindTexture(GL_TEXTURE_2D, 0);
   }
}

GLuint Texture::GetID() const { return ID; }
 
int Texture::GetHeight() const { return Height; }

int Texture::GetWidth() const { return Width; }

}

