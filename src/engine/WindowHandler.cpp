#include "core/core.h"
#include <engine/WindowHandler.h>
#include <engine/InputHandler.h>

#include <cstdlib>

#include "external/imgui.h"
#include "external/imgui_impl_glfw.h"
#include "external/imgui_impl_opengl3.h"
#include "external/easylogging++.h"

namespace Echo2D {

/**
 * @brief GLFW callback function for handling window resizing.
 * 
 * @param window The GLFW window that was resized.
 * @param width The new width of the window.
 * @param height The new height of the window.
 */
static void FramebufferSizeCallback(GLFWwindow* window, int width, int height) {
   // Update the viewport whenever the window is resized
   glViewport(0, 0, width, height);
}

/**
 * @brief Destructor for WindowHandler. Cleans up GLFW and ImGui resources.
 */
WindowHandler::~WindowHandler() {
   // Destroy the GLFW window and terminate GLFW
   glfwDestroyWindow(Window);
   glfwTerminate();

   // Clean up ImGui resources
   ImGui_ImplOpenGL3_Shutdown();
   ImGui_ImplGlfw_Shutdown();
   ImGui::DestroyContext();

   LOG(INFO) << "[WindowHandler] GLFW window destroyed and resources cleaned up.";
}

/**
 * @brief Initializes the window, OpenGL, and ImGui.
 * 
 * This function initializes GLFW, creates a window, initializes GLAD
 * for OpenGL function loading, and sets up ImGui for rendering.
 */
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
   Window = glfwCreateWindow(Width, Height, Title, nullptr, nullptr);
   if (!Window) {
      LOG(ERROR) << "[WindowHandler] Failed to create window!";
      glfwTerminate();
      std::exit(EXIT_FAILURE);  ///< Exit if window creation fails
   }

   LOG(INFO) << "[WindowHandler] Window created with dimensions: " << Width << "x" << Height;

   // Make OpenGL context current for this window
   glfwMakeContextCurrent(Window);

   // Initialize input handling (keyboard, mouse, etc.)
   InputHandler::Init(Window);

   // Set the framebuffer resize callback function
   glfwSetFramebufferSizeCallback(Window, FramebufferSizeCallback);

   // Initialize GLAD to manage OpenGL function loading
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
      LOG(ERROR) << "[WindowHandler] Failed to initialize GLAD!";
      std::exit(EXIT_FAILURE);  ///< Exit if GLAD initialization fails
   }

   LOG(INFO) << "[WindowHandler] GLAD initialized successfully.";

   // Set the initial OpenGL viewport size
   glViewport(0, 0, Width, Height);

   // Initialize ImGui for GUI rendering
   IMGUI_CHECKVERSION();
   ImGui::CreateContext();
   ImGui::StyleColorsDark();

   ImGui_ImplGlfw_InitForOpenGL(Window, true);
   ImGui_ImplOpenGL3_Init();

   LOG(INFO) << "[WindowHandler] ImGui initialized successfully.";
}

/**
 * @brief Clears the window with a specified color.
 * 
 * This function clears the screen using the OpenGL color buffer.
 */
void WindowHandler::ClearColor() {
   glClear(GL_COLOR_BUFFER_BIT);
}

/**
 * @brief Polls for window events.
 * 
 * This function calls glfwPollEvents to handle input events, window events, etc.
 */
void WindowHandler::PollEvents() {
   glfwPollEvents();
}

/**
 * @brief Swaps the front and back buffers.
 * 
 * This function swaps the buffers, displaying the newly rendered content
 * on the window.
 */
void WindowHandler::SwapBuffers() {
   glfwSwapBuffers(Window);
}

/**
 * @brief Checks if the window should close.
 * 
 * @return true if the window should close, false otherwise.
 */
bool WindowHandler::ShouldWindowClose() {
   bool shouldClose = glfwWindowShouldClose(Window);
   return shouldClose;
}

} // namespace Echo2D

