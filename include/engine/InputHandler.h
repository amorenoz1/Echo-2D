#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "core/core.h"
#include <utils/Utils.h>

namespace Engine {

/**
 * @class KeyListener
 * @brief Handles keyboard input via GLFW callbacks.
 *
 * This singleton class keeps track of key states and provides
 * an interface to check if a key is currently pressed.
 */
class KeyListener : public Utils::Singleton<KeyListener> {
    friend class Utils::Singleton<KeyListener>;

public:
    /**
     * @brief GLFW key callback function. Should be registered during window setup.
     */
    static void KeyCallback(GLFWwindow* Window, int Key, int Scancode, int Action, int Mods);

    /**
     * @brief Query if a specific key is currently pressed.
     * @param Key GLFW keycode.
     * @return true if key is pressed, false otherwise.
     */
    static bool isKeyPressed(int Key);

private:
    bool Keys[350] = { false }; ///< Array tracking key states.

    KeyListener() = default;
    ~KeyListener() = default;
};

/**
 * @class MouseListener
 * @brief Handles mouse input: position, scrolling, button presses.
 *
 * This singleton tracks mouse position, movement deltas, scroll values,
 * and dragging state.
 */
class MouseListener : public Utils::Singleton<MouseListener> {
    friend class Utils::Singleton<MouseListener>;

public:
    /**
     * @brief GLFW mouse position callback.
     */
    static void MousePosCallback(GLFWwindow* Window, double PosX, double PosY);

    /**
     * @brief GLFW scroll callback.
     */
    static void ScrollCallback(GLFWwindow* Window, double ScrollX, double ScrollY);

    /**
     * @brief GLFW mouse button callback.
     */
    static void MouseButtonCallback(GLFWwindow* Window, int Button, int Action, int Mods);

    /**
     * @brief Check if a mouse button is pressed.
     * @param MouseButton 0 = left, 1 = right, 2 = middle
     */
    static bool IsMouseButtonDown(int MouseButton);

    /**
     * @brief Is the mouse currently dragging?
     */
    static bool GetIsDragging();

    /// Get current X position of the mouse.
    static float GetX();

    /// Get current Y position of the mouse.
    static float GetY();

    /// Get delta X since last frame.
    static float GetDx();

    /// Get delta Y since last frame.
    static float GetDy();

    /// Get current horizontal scroll amount.
    static float GetScrollX();

    /// Get current vertical scroll amount.
    static float GetScrollY();

private:
    bool MouseButtons[3] = { false }; ///< Left, right, middle button states.
    bool IsDragging = false;          ///< True when any mouse button is held while moving.

    double PosX = 0.0, PosY = 0.0;       ///< Current position.
    double LastPosX = 0.0, LastPosY = 0.0; ///< Previous frame position.
    double ScrollX = 0.0, ScrollY = 0.0; ///< Scroll values.

    MouseListener() = default;
    ~MouseListener() = default;
};

/**
 * @class InputHandler
 * @brief Central static initializer for input callbacks.
 *
 * Use this class to register the input callbacks during application setup.
 */
class InputHandler {
public:
    /**
     * @brief Registers all input-related GLFW callbacks for the window.
     */
    static void Init(GLFWwindow* Window);
};

} // namespace Engine

#endif // INPUTHANDLER_H

