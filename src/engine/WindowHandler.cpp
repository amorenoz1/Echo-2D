#include "core/core.h"
#include <engine/WindowHandler.h>
#include <engine/InputHandler.h>

#include <cstdlib>

#include "external/imgui.h"
#include "external/imgui_impl_glfw.h"
#include "external/imgui_impl_opengl3.h"
#include "external/easylogging++.h"

namespace Echo2D {

static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
   // Update the viewport whenever the window is resized
   glViewport(0, 0, width, height);
}

WindowHandler::~WindowHandler() {
   // Destroy the GLFW window and terminate GLFW
   glfwDestroyWindow(m_Window);
   glfwTerminate();

   // Clean up ImGui resources
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();

   LOG(INFO) << "[WindowHandler] GLFW window destroyed and resources cleaned up.";
}

void WindowHandler::Init() {
   // Initialize GLFW
   if (!glfwInit()) {
      LOG(ERROR) << "[WindowHandler] Failed to initialize GLFW!";
      std::exit(EXIT_FAILURE);  ///< Exit if GLFW initialization fails
   }

   // GLFW configuration settings for OpenGL
   glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);  ///< Use OpenGL 4.1
   glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);  ///< Use OpenGL 4.1
   glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  ///< Use core profile
   glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);  ///< Ensure forward compatibility
   glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);  ///< Disable resizing of window
   glfwWindowHint(GLFW_POSITION_X, 0);  ///< Set window position
   glfwWindowHint(GLFW_POSITION_Y, 0);  ///< Set window position

   glfwSwapInterval(0);  ///< Disable V-Sync

   // Create the GLFW window
   m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);
   if (!m_Window) {
      LOG(ERROR) << "[WindowHandler] Failed to create window!";
      glfwTerminate();
      std::exit(EXIT_FAILURE);  ///< Exit if window creation fails
   }

   LOG(INFO) << "[WindowHandler] Window created with dimensions: " << m_Width << "x" << m_Height;

   // Make OpenGL context current for this window
   glfwMakeContextCurrent(m_Window);

   // Initialize input handling (keyboard, mouse, etc.)
   InputHandler::Init(m_Window);

   // Set the framebuffer resize callback function
   glfwSetFramebufferSizeCallback(m_Window, FramebufferSizeCallback);

   // Initialize GLAD to manage OpenGL function loading
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      LOG(ERROR) << "[WindowHandler] Failed to initialize GLAD!";
      std::exit(EXIT_FAILURE);  ///< Exit if GLAD initialization fails
   }

   LOG(INFO) << "[WindowHandler] GLAD initialized successfully.";

   // Set the initial OpenGL viewport size
   glViewport(0, 0, m_Width, m_Height);

   // Initialize ImGui for GUI rendering
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGui::StyleColorsDark();

   ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
   ImGui_ImplOpenGL3_Init();

   LOG(INFO) << "[WindowHandler] ImGui initialized successfully.";
}

void WindowHandler::ClearColor() {
   glClear(GL_COLOR_BUFFER_BIT);
}

void WindowHandler::PollEvents() {
   glfwPollEvents();
}

void WindowHandler::SwapBuffers() {
   glfwSwapBuffers(m_Window);
}

bool WindowHandler::ShouldWindowClose() {
   bool shouldClose = glfwWindowShouldClose(m_Window);
   return shouldClose;
}

} // namespace Echo2D

