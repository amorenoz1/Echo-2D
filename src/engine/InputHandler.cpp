#include "core/core.h"
#include <engine/InputHandler.h>
#include "external/easylogging++.h"

namespace Echo2D {

bool KeyListener::IsKeyPressed(int Key) {
   // Return current key state from the singleton instance
   return GetInstance().m_Keys[Key];
}

void KeyListener::KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods) {
   if (Action == GLFW_PRESS) {
      GetInstance().m_Keys[Key] = true;
   } else if (Action == GLFW_RELEASE) {
      GetInstance().m_Keys[Key] = false;
   }
}

bool MouseListener::IsMouseButtonDown(int MouseButton) {
   // Only check for valid mouse buttons (0–2)
   return (MouseButton < 3) ? GetInstance().m_MouseButtons[MouseButton] : false;
}

bool MouseListener::GetIsDragging() {
   return GetInstance().m_IsDragging;
}

void MouseListener::MousePosCallback(GLFWwindow* Window, double PosX, double PosY) {
   auto& instance = GetInstance();

   // Save previous position for delta calculation
   instance.m_LastPosX = instance.m_PosX;
   instance.m_LastPosY = instance.m_PosY;

   // Update to new position
   instance.m_PosX = PosX;
   instance.m_PosY = PosY;

   // Determine if dragging (any button held)
   instance.m_IsDragging =
      instance.m_MouseButtons[0] ||
      instance.m_MouseButtons[1] ||
      instance.m_MouseButtons[2];
}

void MouseListener::MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods) {
   auto& instance = GetInstance();

   if (Button < 3) {
      if (Action == GLFW_PRESS) {
         instance.m_MouseButtons[Button] = true;
      } else if (Action == GLFW_RELEASE) {
         instance.m_MouseButtons[Button] = false;
         instance.m_IsDragging = false;
      }
   }
}

void MouseListener::ScrollCallback(GLFWwindow* Window, double ScrollX, double ScrollY) {
   auto& instance = GetInstance();
   instance.m_ScrollX = ScrollX;
   instance.m_ScrollY = ScrollY;
}

float MouseListener::GetX() {
   return static_cast<float>(GetInstance().m_PosX);
}

float MouseListener::GetY() {
   return static_cast<float>(GetInstance().m_PosY);
}

float MouseListener::GetScrollX() {
   return static_cast<float>(GetInstance().m_ScrollX);
}

float MouseListener::GetScrollY() {
   return static_cast<float>(GetInstance().m_ScrollY);
}

float MouseListener::GetDx() {
   // Delta X = last frame X - current X
   return static_cast<float>(GetInstance().m_LastPosX - GetInstance().m_PosX);
}

float MouseListener::GetDy() {
   // Delta Y = last frame Y - current Y
   return static_cast<float>(GetInstance().m_LastPosY - GetInstance().m_PosY);
}

void InputHandler::Init(GLFWwindow* Window) {
   // Hook up all input-related callbacks
   glfwSetKeyCallback(Window, KeyListener::KeyCallback);
   glfwSetMouseButtonCallback(Window, MouseListener::MouseButtonCallback);
   glfwSetScrollCallback(Window, MouseListener::ScrollCallback);
   glfwSetCursorPosCallback(Window, MouseListener::MousePosCallback);

   LOG(INFO) << "[InputHandler] Initialized input callbacks for GLFW window.";
}

} // namespace Echo2D

