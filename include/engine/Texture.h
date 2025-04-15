#include <core/core.h>
#include <external/stb_image.h>

#ifndef TEXTURE_H
#define TEXTURE_H

namespace Engine {
class Font {

};

class Texture {
public:
   Texture (const char* FilePath);
   ~Texture ();

   void Bind(GLuint slot = 0) const;
   void Unbind(GLuint slot = 0) const;

   GLuint GetID() const;
   int GetWidth() const;
   int GetHeight() const;

private:
   GLuint ID;
   int Width;
   int Height;
   int Bits;
};

}
#endif
