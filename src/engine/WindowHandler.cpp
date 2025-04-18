#include "core/core.h"
#include <engine/WindowHandler.h>
#include <engine/InputHandler.h>
#include <cstdlib>
#include <iostream>

#include "external/imgui.h"
#include "external/imgui_impl_glfw.h"
#include "external/imgui_impl_opengl3.h"

namespace Engine {
static void FramebufferSizeCallback (GLFWwindow *Window, int Width, int Height) {
   glViewport(0, 0, Width, Height);
}

WindowHandler::~WindowHandler() {
   glfwDestroyWindow(this->Window);
   glfwTerminate();
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();
}

void WindowHandler::Init() {
   if (!glfwInit()) {
      std::cerr << "Error: Failed to initialize GLFW!" << std::endl;
      std::exit(1);
   }

   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
   glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
   /*glfwWindowHint(GLFW_REFRESH_RATE, 60);*/
   glfwWindowHint(GLFW_POSITION_X, 0);
   glfwWindowHint(GLFW_POSITION_Y, 0);

// Setup Dear ImGui context
   this->Window = glfwCreateWindow(this->Width, this->Height, this->Title, nullptr, nullptr);

   if (!this->Window) {
      glfwTerminate();
      std::cerr << "Error: Failed to create window!" << std::endl;
      std::exit(1);
   }


   InputHandler::Init(this->Window);

   glfwSetFramebufferSizeCallback(this->Window, FramebufferSizeCallback);

   glfwMakeContextCurrent(this->Window);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Error: failed to initialize glad!" << std::endl;
      std::exit(1);
   }

   glViewport(0, 0, this->Width, this->Height);

   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGuiIO& io = ImGui::GetIO();

   ImGui::StyleColorsDark();
   ImGui_ImplGlfw_InitForOpenGL(Window, true);          
   ImGui_ImplOpenGL3_Init();

}

void WindowHandler::ClearColor() { glClear(GL_COLOR_BUFFER_BIT); }

void WindowHandler::PollEvents() { glfwPollEvents(); }

void WindowHandler::SwapBuffers() { glfwSwapBuffers(this->Window); }

bool WindowHandler::ShouldWindowClose() { return glfwWindowShouldClose(this->Window); }

};
