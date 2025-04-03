#include "core/core.h"
#include "external/stb_image.h"
#include <engine/Texture.h>

namespace Engine {

GLuint Texture::LoadTexture(const char *FilePath) {
   int Width, Height, Bits;
   unsigned int TextureID;
   unsigned char* Pixels = stbi_load(FilePath, &Width, &Height, &Bits, 0);

   glCreateTextures(GL_TEXTURE_2D, 1, &TextureID);
   glBindTexture(GL_TEXTURE_2D, TextureID);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   if (Pixels) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, Pixels);
      glGenerateMipmap(GL_TEXTURE_2D);
   }

   stbi_image_free(Pixels);

   return TextureID;
}

}
