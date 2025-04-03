#include "core/core.h"
#include <engine/WindowHandler.h>
#include <engine/InputHandler.h>
#include <cstdlib>
#include <iostream>

namespace Engine {
void WindowHandler::FramebufferSizeCallback (GLFWwindow *Window, int Width, int Height) {
   glViewport(0, 0, Width, Height);
}

WindowHandler::~WindowHandler() {
   glfwDestroyWindow(this->Window);
   glfwTerminate();
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

   this->Window = glfwCreateWindow(this->Width, this->Height, this->Title, nullptr, nullptr);

   if (!this->Window) {
      glfwTerminate();
      std::cerr << "Error: Failed to create window!" << std::endl;
      std::exit(1);
   }

   InputHandler::Init(this->Window);

   glfwSetFramebufferSizeCallback(this->Window, WindowHandler::FramebufferSizeCallback);

   glfwMakeContextCurrent(this->Window);

   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      std::cerr << "Error: failed to initialize glad!" << std::endl;
      std::exit(1);
   }

   glViewport(0, 0, this->Width, this->Height);

}

void WindowHandler::ClearColor() { glClear(GL_COLOR_BUFFER_BIT); }

void WindowHandler::PollEvents() { glfwPollEvents(); }

void WindowHandler::SwapBuffers() { glfwSwapBuffers(this->Window); }

bool WindowHandler::ShouldWindowClose() { return glfwWindowShouldClose(this->Window); }

};
