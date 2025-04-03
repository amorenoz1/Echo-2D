#include "core/core.h"
#include <engine/InputHandler.h>

namespace Engine {

bool KeyListener::isKeyPressed(int Key) { return GetInstance().Keys[Key]; }

void KeyListener::KeyCallback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods) {
   if (Action == GLFW_PRESS) {
      GetInstance().Keys[Key] = true;
   } else if (Action == GLFW_RELEASE) {
      GetInstance().Keys[Key] = false;
   }
}

bool MouseListener::IsMouseButtonDown(int MouseButton) {
   if (MouseButton < 3) {
      return GetInstance().MouseButtons[MouseButton];
   }

   return false;
}

bool MouseListener::GetIsDragging() { return GetInstance().IsDragging; }

void MouseListener::MousePosCallback(GLFWwindow *Window, double PosX, double PosY) {
   GetInstance().LastPosX = GetInstance().PosX;
   GetInstance().LastPosY = GetInstance().PosY;
   GetInstance().PosX = PosX;
   GetInstance().PosY = PosY;
   GetInstance().IsDragging = GetInstance().MouseButtons[0] ||
      GetInstance().MouseButtons[1] ||
      GetInstance().MouseButtons[2];
}

void MouseListener::MouseButtonCallback(GLFWwindow *Window, int Button, int Action, int Mods) {
   if (Action == GLFW_PRESS) {
      if (Button < 3)
         GetInstance().MouseButtons[Button] = true;
   } else if (Action == GLFW_RELEASE) {
      if (Button < 3)
         GetInstance().MouseButtons[Button] = false;
      GetInstance().IsDragging = false;
   }
}

void MouseListener::ScrollCallback(GLFWwindow *Window, double ScrollX, double ScrollY) {
   GetInstance().ScrollX = ScrollX;
   GetInstance().ScrollY = ScrollY;
}

float MouseListener::GetX() { return static_cast<float>(GetInstance().PosX); }
float MouseListener::GetY() { return static_cast<float>(GetInstance().PosY); }
float MouseListener::GetScrollX() { return static_cast<float>(GetInstance().ScrollX); }
float MouseListener::GetScrollY() { return static_cast<float>(GetInstance().ScrollY); }

float MouseListener::GetDx() {
   return static_cast<float>(GetInstance().LastPosX - GetInstance().PosX);
}
float MouseListener::GetDy() {
   return static_cast<float>(GetInstance().LastPosY - GetInstance().PosY);
}

void InputHandler::Init(GLFWwindow *Window){
   glfwSetKeyCallback(Window, KeyListener::KeyCallback);
   glfwSetMouseButtonCallback(Window, MouseListener::MouseButtonCallback);
   glfwSetScrollCallback(Window, MouseListener::ScrollCallback);
   glfwSetCursorPosCallback(Window, MouseListener::MousePosCallback);
}
} // namespace Engine
