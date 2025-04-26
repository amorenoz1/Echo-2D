#include "core/core.h"
#include <engine/Application.h>
#include <engine/ApplicationInfo.h>
#include <engine/Renderer.h>
#include <engine/WindowHandler.h>

#include "external/imgui.h"
#include "external/imgui_impl_glfw.h"
#include "external/imgui_impl_opengl3.h"

#include <numeric>
#include <semaphore>
#include <thread>

namespace Engine {

// ─────────────────────────────────────────────────────────────
// Globals
// ─────────────────────────────────────────────────────────────
ApplicationInfo AppInfo;

// ─────────────────────────────────────────────────────────────
// Utility Functions
// ─────────────────────────────────────────────────────────────
void ImGuiNewFrame() {
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame();
}

void ImGuiDraw() {
   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// ─────────────────────────────────────────────────────────────
// Application Implementation
// ─────────────────────────────────────────────────────────────
Application::Application(const int Width, const int Height, const char *Title) {
   Window = new WindowHandler(Height, Width, Title);
   AppInfo.ScreenWidth = Width;
   AppInfo.ScreenHeight = Height;
   AppInfo.Title = Title;
}

Application::~Application() {
   delete Window;
}

void Application::SetFPS(int FPS) {
   if (FPS < 0 ) return;
   TargetFrameTime = 1.0 / FPS;
}

void Application::Debug() {
   ShowFPS = !ShowFPS;
}

void Application::Init() {}
void Application::Update(float dt) {}
void Application::Render() const {}
void Application::RenderImGui() {}

// ─────────────────────────────────────────────────────────────
// FPS Counter
// ─────────────────────────────────────────────────────────────
void Application::UpdateFpsCounter() {
   FrameCount++;
   FpsTimer += DeltaTime;

   FrameTimeHistory.push_back(DeltaTime);
   if (FrameTimeHistory.size() > MAX_SAMPLES) {
      FrameTimeHistory.erase(FrameTimeHistory.begin());
   }

   if (FpsTimer >= 1.0) {
      CurrentFPS = FrameCount;
      double avgFrameTime = std::accumulate(FrameTimeHistory.begin(), FrameTimeHistory.end(), 0.0)
         / FrameTimeHistory.size();

      RollingFPS = static_cast<int>(1.0 / avgFrameTime);

      FrameCount = 0;
      FpsTimer = 0;
   }
}

// ─────────────────────────────────────────────────────────────
// Application Main Loop
// ─────────────────────────────────────────────────────────────
void Application::Run() {
   Init();
   LastFrameTime = glfwGetTime();

   while (!Window->ShouldWindowClose()) {
      double FrameStartTime = glfwGetTime();

      BeginFrame();

      Update(static_cast<float>(DeltaTime));
      Render();
      RenderImGui();

      EndFrame();
      CapFrameRate(FrameStartTime);
   }
}

// ─────────────────────────────────────────────────────────────
// Frame Loop Helpers
// ─────────────────────────────────────────────────────────────
void Application::BeginFrame() {
   BatchData.DrawCalls = 0;

   UpdateFpsCounter();
   ImGuiNewFrame();

   Renderer::InitDraw();
   Window->ClearColor();
}

void Application::EndFrame() {
   Renderer::EndDraw();
   Renderer::Flush();

   if (ShowFPS) {
      ImGui::BeginMainMenuBar();
      ImGui::Text("FPS: %.2lf, Rolling FPS: %d, Draw Calls: %d, Delta Time: %lf",
                  CurrentFPS, RollingFPS, BatchData.DrawCalls, DeltaTime);
      ImGui::EndMainMenuBar();
   }

   ImGuiDraw();

   Window->SwapBuffers();
   Window->PollEvents();

   double FrameEndTime = glfwGetTime();
   DeltaTime = FrameEndTime - LastFrameTime;
   LastFrameTime = FrameEndTime;
}

void Application::CapFrameRate(double FrameStartTime) {
   double Elapsed = glfwGetTime() - FrameStartTime;
   double SleepTime = TargetFrameTime - Elapsed;

   if (SleepTime > 0.001) {
      std::this_thread::sleep_for(std::chrono::duration<double>(SleepTime - 0.001));
   }
}

} // namespace Engine

