#include "core/core.h"
#include <engine/InputHandler.h>
#include "external/easylogging++.h"

namespace Echo2D {

// =========================
// KeyListener Implementation
// =========================

/**
 * @brief Returns true if the specified key is currently pressed.
 * @param Key The GLFW key code.
 */
bool KeyListener::isKeyPressed(int Key) {
   bool isPressed = GetInstance().Keys[Key];
   // LOG(TRACE) << "[KeyListener] Checking if key " << Key << " is pressed: " << isPressed;
   return isPressed;
}

/**
 * @brief Callback for keyboard input events.
 * @param Window Pointer to the GLFW window.
 * @param Key Key code.
 * @param Scancode Hardware scan code.
 * @param Action Press/release action.
 * @param Mods Modifier keys (e.g., Shift).
 */
void KeyListener::KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods) {
   if (Action == GLFW_PRESS) {
      GetInstance().Keys[Key] = true;
      // LOG(INFO) << "[KeyListener] Key pressed: " << Key;
   } else if (Action == GLFW_RELEASE) {
      GetInstance().Keys[Key] = false;
      // LOG(INFO) << "[KeyListener] Key released: " << Key;
   }
}

// ==========================
// MouseListener Implementation
// ==========================

/**
 * @brief Returns true if the specified mouse button is currently held down.
 * @param MouseButton GLFW_MOUSE_BUTTON_X (0 = left, 1 = right, 2 = middle).
 */
bool MouseListener::IsMouseButtonDown(int MouseButton) {
   bool isPressed = (MouseButton < 3) ? GetInstance().MouseButtons[MouseButton] : false;
   // LOG(TRACE) << "[MouseListener] Checking if mouse button " << MouseButton << " is down: " << isPressed;
   return isPressed;
}

/**
 * @brief Returns true if any mouse button is being held down (used for drag detection).
 */
bool MouseListener::GetIsDragging() {
   bool isDragging = GetInstance().IsDragging;
   // LOG(TRACE) << "[MouseListener] Checking if dragging: " << isDragging;
   return isDragging;
}

/**
 * @brief Callback for mouse movement.
 * @param Window Pointer to the GLFW window.
 * @param PosX Current X position of the cursor.
 * @param PosY Current Y position of the cursor.
 */
void MouseListener::MousePosCallback(GLFWwindow* Window, double PosX, double PosY) {
   auto& instance = GetInstance();
   instance.LastPosX = instance.PosX;
   instance.LastPosY = instance.PosY;
   instance.PosX = PosX;
   instance.PosY = PosY;

   instance.IsDragging = instance.MouseButtons[0] || instance.MouseButtons[1] || instance.MouseButtons[2];
   
   // LOG(INFO) << "[MouseListener] Mouse moved to (" << PosX << ", " << PosY << ")";
   if (instance.IsDragging) {
     // LOG(INFO) << "[MouseListener] Mouse is being dragged.";
   }
}

/**
 * @brief Callback for mouse button press/release events.
 * @param Window Pointer to the GLFW window.
 * @param Button The mouse button index.
 * @param Action GLFW_PRESS or GLFW_RELEASE.
 * @param Mods Modifier keys (e.g., Shift).
 */
void MouseListener::MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods) {
   auto& instance = GetInstance();

   if (Button < 3) {
      if (Action == GLFW_PRESS) {
         instance.MouseButtons[Button] = true;
        // LOG(INFO) << "[MouseListener] Mouse button " << Button << " pressed.";
      } else if (Action == GLFW_RELEASE) {
         instance.MouseButtons[Button] = false;
         instance.IsDragging = false;
         // LOG(INFO) << "[MouseListener] Mouse button " << Button << " released.";
      }
   }
}

/**
 * @brief Callback for mouse scroll events.
 * @param Window Pointer to the GLFW window.
 * @param ScrollX Scroll delta on the X axis.
 * @param ScrollY Scroll delta on the Y axis.
 */
void MouseListener::ScrollCallback(GLFWwindow* Window, double ScrollX, double ScrollY) {
   auto& instance = GetInstance();
   instance.ScrollX = ScrollX;
   instance.ScrollY = ScrollY;

  // LOG(INFO) << "[MouseListener] Mouse scroll event: (" << ScrollX << ", " << ScrollY << ")";
}

// Position Getters

float MouseListener::GetX() {
   float posX = static_cast<float>(GetInstance().PosX);
   // LOG(TRACE) << "[MouseListener] GetX: " << posX;
   return posX;
}

float MouseListener::GetY() {
   float posY = static_cast<float>(GetInstance().PosY);
   // LOG(TRACE) << "[MouseListener] GetY: " << posY;
   return posY;
}

// Scroll Getters

float MouseListener::GetScrollX() {
   float scrollX = static_cast<float>(GetInstance().ScrollX);
   // LOG(TRACE) << "[MouseListener] GetScrollX: " << scrollX;
   return scrollX;
}

float MouseListener::GetScrollY() {
   float scrollY = static_cast<float>(GetInstance().ScrollY);
   // LOG(TRACE) << "[MouseListener] GetScrollY: " << scrollY;
   return scrollY;
}

// Delta Movement Getters

float MouseListener::GetDx() {
   float dx = static_cast<float>(GetInstance().LastPosX - GetInstance().PosX);
   // LOG(TRACE) << "[MouseListener] GetDx: " << dx;
   return dx;
}

float MouseListener::GetDy() {
   float dy = static_cast<float>(GetInstance().LastPosY - GetInstance().PosY);
   // LOG(TRACE) << "[MouseListener] GetDy: " << dy;
   return dy;
}

// ===========================
// InputHandler Initialization
// ===========================

/**
 * @brief Sets up all input callbacks for the given GLFW window.
 * @param Window The GLFW window to attach input callbacks to.
 */
void InputHandler::Init(GLFWwindow* Window) {
   glfwSetKeyCallback(Window, KeyListener::KeyCallback);
   glfwSetMouseButtonCallback(Window, MouseListener::MouseButtonCallback);
   glfwSetScrollCallback(Window, MouseListener::ScrollCallback);
   glfwSetCursorPosCallback(Window, MouseListener::MousePosCallback);

   LOG(INFO) << "[InputHandler] Initialized input callbacks for GLFW window.";
}

} // namespace Echo2D

