#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "core/core.h"
#include <utils/Utils.h>

namespace Echo2D {

/**
 * @class KeyListener
 * @brief Handles keyboard input via GLFW callbacks.
 *
 * Singleton class that tracks the pressed state of keys using a key state array.
 * Intended for internal engine use to poll keyboard state.
 */
class KeyListener : public Utils::Singleton<KeyListener> {
    friend class Utils::Singleton<KeyListener>;

public:
    /**
     * @brief GLFW key callback function.
     * Registers press and release events for keys.
     * @param Window Pointer to the GLFW window.
     * @param Key The key code (GLFW).
     * @param Scancode The platform-specific scancode of the key.
     * @param Action GLFW_PRESS or GLFW_RELEASE.
     * @param Mods Modifier bits (e.g., Shift, Ctrl).
     */
    static void KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);

    /**
     * @brief Checks whether a key is currently being pressed.
     * @param Key GLFW keycode.
     * @return true if the key is pressed; false otherwise.
     */
    static bool IsKeyPressed(int Key);

private:
    bool m_Keys[350] = { false }; ///< State array for 350 possible GLFW keys.

    KeyListener() = default;
    ~KeyListener() = default;
};

/**
 * @class MouseListener
 * @brief Handles mouse input events like position, scroll, drag, and buttons.
 *
 * Singleton class that maintains mouse state, including position deltas
 * and button presses.
 */
class MouseListener : public Utils::Singleton<MouseListener> {
    friend class Utils::Singleton<MouseListener>;

public:
    /// Callback for mouse position movement.
    static void MousePosCallback(GLFWwindow* Window, double PosX, double PosY);

    /// Callback for mouse scroll events.
    static void ScrollCallback(GLFWwindow* Window, double ScrollX, double ScrollY);

    /// Callback for mouse button press/release.
    static void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods);

    /**
     * @brief Checks whether a mouse button is held down.
     * @param MouseButton 0 = left, 1 = right, 2 = middle.
     * @return true if held; false otherwise.
     */
    static bool IsMouseButtonDown(int MouseButton);

    /**
     * @brief Determines if the mouse is being dragged.
     * @return true if dragging (any button down while moving).
     */
    static bool GetIsDragging();

    /// Current X position of the mouse.
    static float GetX();

    /// Current Y position of the mouse.
    static float GetY();

    /// Mouse delta in X direction since last frame.
    static float GetDx();

    /// Mouse delta in Y direction since last frame.
    static float GetDy();

    /// Mouse scroll in X direction.
    static float GetScrollX();

    /// Mouse scroll in Y direction.
    static float GetScrollY();

private:
    bool m_MouseButtons[3] = { false }; ///< Button state array for left, right, middle.
    bool m_IsDragging = false;          ///< Drag state flag.

    double m_PosX = 0.0, m_PosY = 0.0;             ///< Current position.
    double m_LastPosX = 0.0, m_LastPosY = 0.0;     ///< Previous position (for delta).
    double m_ScrollX = 0.0, m_ScrollY = 0.0;       ///< Scroll delta.

    MouseListener() = default;
    ~MouseListener() = default;
};

/**
 * @class InputHandler
 * @brief Static class responsible for initializing input callbacks.
 *
 * Use during application or window setup to hook GLFW input events
 * to the engine's listeners.
 */
class InputHandler {
public:
    /**
     * @brief Sets GLFW input callbacks to engine's handlers.
     * @param Window Pointer to the GLFW window.
     */
    static void Init(GLFWwindow* Window);
};

} // namespace Echo2D

#endif // INPUTHANDLER_H

