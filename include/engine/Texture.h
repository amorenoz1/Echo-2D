#include "core/core.h"

#ifndef TEXTURE_H
#define TEXTURE_H

namespace Engine {

class Texture {
public:
   static GLuint LoadTexture(const char* FilePath);
};

}
#endif
