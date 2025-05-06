#ifndef WINDOW_H
#define WINDOW_H

#include "core/core.h"

namespace Echo2D {

/**
 * @class WindowHandler
 * @brief Manages the creation and interaction with a GLFW window.
 *
 * Encapsulates GLFW window creation, input polling, and buffer swapping.
 * Should be created early in the application lifecycle.
 */
class WindowHandler {
public:
    /**
     * @brief Constructs a new WindowHandler and initializes the window.
     * @param H Height of the window in pixels.
     * @param W Width of the window in pixels.
     * @param T Title of the window.
     */
    WindowHandler(const int H, const int W, const char* T) 
        : m_Height(H), m_Width(W), m_Title(T) { Init(); }

    /// Cleans up and destroys the GLFW window.
    ~WindowHandler();

    /**
     * @brief Checks if the window should close.
     * @return true if the user requested to close the window.
     */
    bool ShouldWindowClose();

    /**
     * @brief Clears the window's color buffer.
     */
    void ClearColor();

    /**
     * @brief Swaps the window's front and back buffers.
     */
    void SwapBuffers();

    /**
     * @brief Polls for window/input events.
     */
    void PollEvents();

    /// @return Height of the window.
    int GetHeight();

    /// @return Width of the window.
    int GetWidth();

private:
    GLFWwindow* m_Window = nullptr; ///< GLFW window handle.
    int m_Height;                   ///< Window height in pixels.
    int m_Width;                    ///< Window width in pixels.
    const char* m_Title;            ///< Window title.

    /**
     * @brief Internal window creation and GLFW initialization.
     */
    void Init();
};

} // namespace Echo2D

#endif // WINDOW_H

