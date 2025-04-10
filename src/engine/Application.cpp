#include "core/core.h"
#include <engine/Application.h>
#include <engine/ApplicationInfo.h>
#include <engine/Renderer.h>
#include <engine/WindowHandler.h>

#include <numeric>
#include <thread>

#include "external/imgui.h"
#include "external/imgui_impl_glfw.h"
#include "external/imgui_impl_opengl3.h"

namespace Engine {

ApplicationInfo AppInfo;

Application::Application(const int Width, const int Height, const char *Title) {
   Window = new WindowHandler(Height, Width, Title);
   AppInfo.ScreenWidth = Width;
   AppInfo.ScreenHeight = Height;
   AppInfo.Title = Title;
}

Application::~Application() { delete Window; }

void ImGuiNewFrame() {
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame();
}

void ImGuiDraw() {
   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void Application::UpdateFpsCounter() {
   FrameCount++;
   FpsTimer += DeltaTime;

   FrameTimeHistory.push_back(DeltaTime);
   if (FrameTimeHistory.size() > MAX_SAMPLES) {
      FrameTimeHistory.erase(FrameTimeHistory.begin());
   }

   if (FpsTimer >= 1.0) {
      CurrentFPS = FrameCount;

      double AverageFrameTime =
         std::accumulate(FrameTimeHistory.begin(), FrameTimeHistory.end(), 0.0) /
         FrameTimeHistory.size();

      RollingFPS = static_cast<int>(1.0 / AverageFrameTime);

      FrameCount = 0;
      FpsTimer = 0;
   }
}

void Application::Run() {
   Init();
   LastFrameTime = glfwGetTime();

   while (!Window->ShouldWindowClose()) {
      // delta time calculation
      double CurrentTime = glfwGetTime();
      DeltaTime = CurrentTime - LastFrameTime;
      LastFrameTime = CurrentTime;

      BatchData.DrawCalls = 0;

      UpdateFpsCounter();

      ImGuiNewFrame();

      Renderer::InitDraw();
      {
         Window->ClearColor();
         Update(static_cast<float>(DeltaTime));
         Render();
         RenderImGui();
      }
      Renderer::EndDraw();
      Renderer::Flush();
 
      if (ShowFPS) {
         ImGui::BeginMainMenuBar();
         ImGui::Text("FPS: %d, Rolling FPS: %d, Draw Calls: %d, Delta Time: %lf", CurrentFPS,
                     RollingFPS, BatchData.DrawCalls, DeltaTime);
         ImGui::EndMainMenuBar();
      }     

      ImGuiDraw();

      Window->SwapBuffers();
      Window->PollEvents();

      double FrameEndTime = glfwGetTime() - CurrentTime;
      double SleepTime = TargetFrameTime - FrameEndTime;
      if (SleepTime > 0.0) {
         std::this_thread::sleep_for(std::chrono::duration<double>(SleepTime));
      }
   }
}
void Application::SetFPS(int FPS) {
   TargetFrameTime = 1.0/FPS;
}
void Application::Debug() { ShowFPS = !ShowFPS; }

void Application::RenderImGui() {}

void Application::Update(float dt) {}

void Application::Render() const {}

void Application::Init() {}
} // namespace Engine
