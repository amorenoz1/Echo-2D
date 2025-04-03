#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "core/core.h"
#include <utils/Utils.h>

namespace Engine {

class KeyListener : public Utils::Singleton<KeyListener> {
   friend class Utils::Singleton<KeyListener>;
public:
   static void KeyCallback(GLFWwindow *Window, int Key, int Scancode, int Action, int Mods);
   static bool isKeyPressed(int Key);

private:
   bool Keys[350];

   KeyListener() = default;
   ~KeyListener() = default;
};

class MouseListener : public Utils::Singleton<MouseListener> {
   friend class Utils::Singleton<MouseListener>;
public:
   static void MousePosCallback(GLFWwindow *Window, double PosX, double PosY);
   static void ScrollCallback(GLFWwindow *Window, double ScrollX, double ScrollY);
   static void MouseButtonCallback(GLFWwindow *Window, int Button, int Action, int Mods);
   static bool IsMouseButtonDown(int MouseButton);
   static bool GetIsDragging();
   static float GetX();
   static float GetY();
   static float GetDx();
   static float GetDy();
   static float GetScrollX();
   static float GetScrollY();


private:
   bool MouseButtons[3];
   bool IsDragging;
   double PosX, PosY, LastPosX, LastPosY;
   double ScrollX, ScrollY;

   MouseListener() = default;
   ~MouseListener() = default;
};

class InputHandler {
public:
   static void Init(GLFWwindow *Window);
};

} // namespace Engine

#endif
