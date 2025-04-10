#ifndef APPLICATION_H
#define APPLICATION_H

#include "engine/WindowHandler.h"
#include <vector>

namespace Engine {

class Application {
public:
   Application(const int Width, const int Height, const char *Iitle);
   virtual ~Application();

   void Run();
   void Debug();
   void SetFPS(int FPS);
protected:
  
   virtual void Init();
   virtual void RenderImGui();
   virtual void Update(float dt);
   virtual void Render() const;

private:
   WindowHandler *Window = nullptr;
   double LastFrameTime = 0;
   double DeltaTime = 0;
   double FpsTimer = 0;
   double TargetFrameTime = 1.0/60.0;
   bool ShowFPS = false;
   int FrameCount = 0;
   int CurrentFPS = 0;
   int RollingFPS = 0;
   const int MAX_SAMPLES = 30;
   std::vector<double> FrameTimeHistory;

   void UpdateFpsCounter();
};

} // Engine


#endif // !APPLICATION_H

