#ifndef APPLICATION_H
#define APPLICATION_H

#include "engine/WindowHandler.h"

namespace Engine {

class Application {
public:
   Application(const int Width, const int Height, const char *Iitle);
   virtual ~Application();

   void Run();

protected:
   WindowHandler *Window;
   const int Width;
   const int Height;

   virtual void Init();
   virtual void Update(float dt);
   virtual void Render() const;
};

} // Engine


#endif // !APPLICATION_H

