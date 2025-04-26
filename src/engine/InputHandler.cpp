#include "core/core.h"
#include <engine/InputHandler.h>

namespace Engine {

// =========================
// KeyListener Implementation
// =========================

/**
 * @brief Returns true if the specified key is currently pressed.
 * @param Key The GLFW key code.
 */
bool KeyListener::isKeyPressed(int Key) {
    return GetInstance().Keys[Key];
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
    } else if (Action == GLFW_RELEASE) {
        GetInstance().Keys[Key] = false;
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
    if (MouseButton < 3) {
        return GetInstance().MouseButtons[MouseButton];
    }
    return false;
}

/**
 * @brief Returns true if any mouse button is being held down (used for drag detection).
 */
bool MouseListener::GetIsDragging() {
    return GetInstance().IsDragging;
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
        } else if (Action == GLFW_RELEASE) {
            instance.MouseButtons[Button] = false;
            instance.IsDragging = false;
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
}

// Position Getters

float MouseListener::GetX() { return static_cast<float>(GetInstance().PosX); }
float MouseListener::GetY() { return static_cast<float>(GetInstance().PosY); }

// Scroll Getters

float MouseListener::GetScrollX() { return static_cast<float>(GetInstance().ScrollX); }
float MouseListener::GetScrollY() { return static_cast<float>(GetInstance().ScrollY); }

// Delta Movement Getters

float MouseListener::GetDx() {
    return static_cast<float>(GetInstance().LastPosX - GetInstance().PosX);
}

float MouseListener::GetDy() {
    return static_cast<float>(GetInstance().LastPosY - GetInstance().PosY);
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
}

} // namespace Engine

