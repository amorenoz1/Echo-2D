#include "core/core.h"
#include <engine/WindowHandler.h>
#include <engine/Application.h>
#include <engine/Renderer.h>
#include <iostream>

namespace Engine{

Application::Application (const int Width, const int Height, const char* Title) : Width(Width), Height(Height){
   this->Window = new WindowHandler(Height, Width, Title);
}

Application::~Application() {
   delete this->Window;
}

void Application::Run() {
   double LastTime = glfwGetTime();

   while(!this->Window->ShouldWindowClose()) {

      double CurrentTime = glfwGetTime();
      double DeltaTime = CurrentTime - LastTime;
      LastTime = CurrentTime;

      this->Window->ClearColor();
      this->Update(DeltaTime);

      Renderer::InitDraw(this->Width, this->Height);
      this->Render();
      Renderer::EndDraw();

      this->Window->SwapBuffers();
      this->Window->PollEvents();

      double FrameEndTime = glfwGetTime();
      double FrameDuration = FrameEndTime - CurrentTime;
      double FPS = 1.0/DeltaTime;
      std::cout << "FPS: " << FPS << std::endl;
   }
}

void Application::Update(float dt) {}

void Application::Render() const {}

}
