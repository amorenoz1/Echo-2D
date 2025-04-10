#ifndef WINDOW_H
#define WINDOW_H

#include "core/core.h"

namespace Engine {

class WindowHandler {
public:
   WindowHandler(const int H, const int W, const char *T) : Height(H), Width(W), Title(T){ Init(); }
   ~WindowHandler();
   bool ShouldWindowClose();
   void ClearColor();
   void SwapBuffers();
   void PollEvents();
   int GetHeight();
   int GetWidth();

private:
   GLFWwindow *Window;
   int Height;
   int Width;
   const char *Title;

   void Init();
};

} // namespace Engine

#endif // WINDOW_H
