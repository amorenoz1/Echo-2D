
#include <core/core.h>
#include "external/stb_image.h"
#include <engine/Texture.h>
#include <cstdio>

namespace Engine {

Texture::Texture(const char* fp) {
   printf("[Texture] Constructor called with path: %s\n", fp);
   this->ID = LoadTexture(fp);
   printf("[Texture] Constructor finished, texture ID: %u\n", this->ID);
}

GLuint LoadTexture(const char *FilePath) {
   printf("[TextureLoader] LoadTexture called\n");

   if (!FilePath) {
      printf("[TextureLoader] Invalid filepath (null)\n");
      return 404;
   }

   printf("[TextureLoader] Loading file: %s\n", FilePath);

   int Width, Height, Bits;
   unsigned int TextureID;

   unsigned char* Pixels = stbi_load(FilePath, &Width, &Height, &Bits, 4);

   if (!Pixels) {
      printf("[TextureLoader] Failed to load image at path: %s\n", FilePath);
      return 0;
   }

   printf("[TextureLoader] Image loaded: %dx%d, channels: %d\n", Width, Height, Bits);
   printf("[TextureLoader] Calling glCreateTextures...\n");

   glGenTextures(1, &TextureID);
   printf("[TextureLoader] glCreateTextures returned ID: %u\n", TextureID);

   glBindTexture(GL_TEXTURE_2D, TextureID);
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
   printf("[TextureLoader] Image data freed\n");

   return TextureID;
}

}

