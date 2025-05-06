#include "core/core.h"
#include <engine/Application.h>
#include <engine/ApplicationInfo.h>
#include <engine/Renderer.h>
#include <engine/WindowHandler.h>

#include "external/imgui.h"
#include "external/imgui_impl_glfw.h"
#include "external/imgui_impl_opengl3.h"
#include "external/easylogging++.h"
INITIALIZE_EASYLOGGINGPP

#include <numeric>
#include <thread>

namespace Echo2D {

ApplicationInfo g_AppInfo;


void ImGuiNewFrame() {
   ImGui_ImplOpenGL3_NewFrame();
   ImGui_ImplGlfw_NewFrame();
   ImGui::NewFrame();
}


void ImGuiDraw() {
   ImGui::Render();
   ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}


Application::Application(const int Width, const int Height, const char *Title) {
   m_Window = new WindowHandler(Height, Width, Title);
   g_AppInfo.ScreenWidth = Width;
   g_AppInfo.ScreenHeight = Height;
   g_AppInfo.Title = Title;

   LOG(INFO) << "[Application] Created with dimensions (" << Width << "x" << Height << ") and title: " << Title;
}

Application::~Application() {
   LOG(INFO) << "[Application] Destroying application and window.";
   delete m_Window;
}

void Application::SetFPS(int FPS) {
   if (FPS < 0 ) return;
   m_TargetFrameTime = 1.0 / FPS;
   LOG(INFO) << "[Application] FPS set to " << FPS;
}

void Application::Debug() {
   m_ShowFPS = !m_ShowFPS;
}

void Application::Init() {}

void Application::Update(float dt) {}

void Application::Render() const {}

void Application::RenderImGui() {}

void Application::UpdateFpsCounter() {
   m_FrameCount++;
   m_FpsTimer += m_DeltaTime;

   m_FrameTimeHistory.push_back(m_DeltaTime);
   if (m_FrameTimeHistory.size() > MAX_SAMPLES) {
      m_FrameTimeHistory.erase(m_FrameTimeHistory.begin());
   }

   if (m_FpsTimer >= 1.0) {
      m_CurrentFPS = m_FrameCount;
      double avgFrameTime = std::accumulate(m_FrameTimeHistory.begin(), m_FrameTimeHistory.end(), 0.0)
         / m_FrameTimeHistory.size();

      m_RollingFPS = static_cast<int>(1.0 / avgFrameTime);

      m_FrameCount = 0;
      m_FpsTimer = 0;
   }
}

void Application::Run() {
   Init();
   m_LastFrameTime = glfwGetTime();

   while (!m_Window->ShouldWindowClose()) {
      double FrameStartTime = glfwGetTime();

      BeginFrame();

      Update(static_cast<float>(m_DeltaTime));
      Render();
      RenderImGui();

      EndFrame();
      CapFrameRate(FrameStartTime);
   }
}

void Application::BeginFrame() {
   g_BatchData.DrawCalls = 0;

   UpdateFpsCounter();
   ImGuiNewFrame();

   Renderer::InitDraw();
   m_Window->ClearColor();
}

void Application::EndFrame() {
   Renderer::EndDraw();
   Renderer::Flush();

   if (m_ShowFPS) {
      ImGui::BeginMainMenuBar();
      ImGui::Text("FPS: %.2lf, Rolling FPS: %d, Draw Calls: %d, Delta Time: %lf",
                  m_CurrentFPS, m_RollingFPS, g_BatchData.DrawCalls, m_DeltaTime);
      ImGui::EndMainMenuBar();
   }

   ImGuiDraw();

   m_Window->SwapBuffers();
   m_Window->PollEvents();

   double FrameEndTime = glfwGetTime();
   m_DeltaTime = FrameEndTime - m_LastFrameTime;
   m_LastFrameTime = FrameEndTime;
}

void Application::CapFrameRate(double FrameStartTime) {
   double Elapsed = glfwGetTime() - FrameStartTime;
   double SleepTime = m_TargetFrameTime - Elapsed;

   if (SleepTime > 0.001) {
      std::this_thread::sleep_for(std::chrono::duration<double>(SleepTime - 0.001));
   }
}

} // namespace Echo2D

