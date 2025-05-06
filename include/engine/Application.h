#ifndef APPLICATION_H
#define APPLICATION_H

#include "engine/WindowHandler.h"
#include <vector>

namespace Echo2D {

/**
 * @class Application
 * @brief Base class for a game or rendering application using the engine.
 *
 * This class is responsible for managing the main loop, timing, window
 * handling, and an optional debug overlay (FPS counter). To use, subclass this
 * class and override the Init, Update, Render, and optionally RenderImGui
 * methods.
 */
class Application {
public:
   /**
   * @brief Constructs an Application object.
   * @param Width The width of the window in pixels.
   * @param Height The height of the window in pixels.
   * @param Title The title of the window.
   */
   Application(int Width, int Height, const char *Title);

   /**
   * @brief Destructor for cleanup.
   */
   virtual ~Application();

   /**
   * @brief Starts the main application loop.
   */
   void Run();

   /**
   * @brief Toggles the FPS debug overlay on or off.
   */
   void Debug();

   /**
   * @brief Sets the target framerate (FPS) for the application.
   * @param FPS The target FPS to cap the game at.
   */
   void SetFPS(int FPS);

protected:
   /**
   * @brief Initialization hook that runs once before the main loop.
   */
   virtual void Init();

   /**
   * @brief Updates game logic every frame.
   * @param dt Time delta (in seconds) since the last frame.
   */
   virtual void Update(float dt);

   /**
   * @brief Renders the scene every frame.
   */
   virtual void Render() const;

   /**
   * @brief Optional hook for ImGui rendering (for debugging, etc.).
   */
   virtual void RenderImGui();

private:
   // Core Systems
   WindowHandler *m_Window = nullptr; ///< Manages window and input handling.

   // Timing and FPS Management
   double m_LastFrameTime = 0.0; ///< Timestamp of the last frame.
   double m_DeltaTime = 0.0;     ///< Time elapsed between frames.
   double m_FpsTimer = 0.0;      ///< Accumulates time for FPS measurement.
   double m_TargetFrameTime = 1.0 / 60.0;      ///< Target time per frame (based on 60 FPS).
   double m_CurrentFPS = 0.0; ///< Calculated FPS for the current second.

   // FPS Counter
   int m_FrameCount = 0;       ///< Frames counted in the current second.
   int m_RollingFPS = 0;       ///< Smoothed FPS value over a period of time.
   const int MAX_SAMPLES = 30; ///< Number of samples for FPS rolling average.
   std::vector<double> m_FrameTimeHistory; ///< History of frame times.

   // Debugging State
   bool m_ShowFPS = false; ///< Whether to display FPS overlay.

   // Internal Loop Helpers
   void UpdateFpsCounter();
   void BeginFrame();
   void EndFrame();

   /**
   * @brief Caps the framerate by sleeping if the frame completes early.
   * @param FrameStartTime The time when the current frame started.
   */
   void CapFrameRate(double FrameStartTime);
};

} // namespace Echo2D

#endif
