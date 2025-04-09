#include "core/core.h"
#include <engine/ApplicationInfo.h>
#include <engine/WindowHandler.h>
#include <engine/Application.h>
#include <engine/Renderer.h>

#include <iostream>
#include <thread>

#include "external/imgui.h"
#include "external/imgui_impl_glfw.h"
#include "external/imgui_impl_opengl3.h"

namespace Engine{

static bool showFPS = false;

ApplicationInfo AppInfo;

Application::Application (const int Width, const int Height, const char* Title) : Width(Width), Height(Height){
   this->Window = new WindowHandler(Height, Width, Title);
   AppInfo.ScreenWidth = Width;
   AppInfo.ScreenHeight = Height;
   AppInfo.Title = Title;
}

Application::~Application() {
   delete this->Window;
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();
}

void Application::Run() {
   double LastTime = glfwGetTime();
   double FPS = 0;
   const double TargetFrameTime = 1.0 / 500.0; // 2 ms per frame

   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO();

   ImGui::StyleColorsDark();
   ImGui_ImplGlfw_InitForOpenGL(this->Window->Window, true);          
   ImGui_ImplOpenGL3_Init();

   this->Init();

   while(!this->Window->ShouldWindowClose()) {
      this->Window->PollEvents();

      double CurrentTime = glfwGetTime();
      double DeltaTime = CurrentTime - LastTime;
      LastTime = CurrentTime;
      FPS = 1.0/DeltaTime;

      BatchData.DrawCalls = 0;
      this->Window->ClearColor();
      this->Update(DeltaTime);

      ImGui_ImplOpenGL3_NewFrame();
      ImGui_ImplGlfw_NewFrame();

      Renderer::InitDraw();

      this->Render();

      Renderer::EndDraw();
      Renderer::Flush();

      ImGui::NewFrame();
      if (showFPS) {
         ImGui::BeginMainMenuBar();
         ImGui::Text("FPS: %.2f", (float)FPS);
         ImGui::EndMainMenuBar();
      }

      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

         this->Window->SwapBuffers();

      double FrameEndTime = glfwGetTime();
      double FrameDuration = FrameEndTime - CurrentTime;

      double SleepTime = TargetFrameTime - FrameDuration;
        if (SleepTime > 0.0) {
            std::this_thread::sleep_for(std::chrono::duration<double>(SleepTime));
        }
      // std::cout << "FPS: " << FPS << std::endl;
   }
   // std::cout << "Draw Calls From App: " << BatchData.DrawCalls << std::endl;
}

void Application::ShowFPS() {
   showFPS = !showFPS;
}

void Application::Update(float dt) {}

void Application::Render() const {}

void Application::Init() {}
}
