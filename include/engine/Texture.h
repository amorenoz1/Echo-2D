#include <core/core.h>

#ifndef TEXTURE_H
#define TEXTURE_H

namespace Engine {

struct Texture {
   Texture(const char* FilePath);
   GLuint ID;
};

GLuint LoadTexture(const char* FilePath);

}
#endif
