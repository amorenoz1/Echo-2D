#pragma once

#include "engine/WindowHandler.h"
#include <vector>

namespace Engine {

/**
 * @class Application
 * @brief Base class for a game or rendering application using the engine.
 *
 * This class handles the main loop, timing, window management,
 * and optional debug overlay (FPS counter). To use, subclass it and override
 * Init, Update, Render, and optionally RenderImGui.
 */
class Application {
public:
    /**
     * @brief Construct a new Application object.
     * @param Width Window width in pixels.
     * @param Height Window height in pixels.
     * @param Title Window title.
     */
    Application(int Width, int Height, const char* Title);

    /// Destructor. Handles cleanup.
    virtual ~Application();

    /// Starts the main loop.
    void Run();

    /// Toggles the debug overlay (FPS display).
    void Debug();

    /**
     * @brief Sets a target framerate (in FPS).
     * @param FPS Frames per second to cap the game at.
     */
    void SetFPS(int FPS);

protected:
    /// Called once before the main loop. Use for initialization.
    virtual void Init();

    /**
     * @brief Called every frame to update game logic.
     * @param dt Time delta (in seconds) since last frame.
     */
    virtual void Update(float dt);

    /// Called every frame to render your scene.
    virtual void Render() const;

    /// Optional: ImGui rendering hook.
    virtual void RenderImGui();

private:
    // ===== Core Systems =====
    WindowHandler* Window = nullptr;  ///< Manages the window and input.

    // ===== Timing =====
    double LastFrameTime = 0.0;       ///< Time of the previous frame.
    double DeltaTime = 0.0;           ///< Time between current and previous frame.
    double FpsTimer = 0.0;            ///< Accumulates time for FPS measurement.
    double TargetFrameTime = 1.0 / 60.0; ///< Time per frame for target FPS.
    double CurrentFPS = 0.0;          ///< Calculated FPS this second.

    // ===== FPS Counter =====
    int FrameCount = 0;               ///< Frames counted in the current second.
    int RollingFPS = 0;               ///< Smoothed FPS value.
    const int MAX_SAMPLES = 30;       ///< Used to average FPS over N samples.
    std::vector<double> FrameTimeHistory; ///< History of frame times.

    // ===== State =====
    bool ShowFPS = false;             ///< Whether to display FPS overlay.

    // ===== Internal Loop Helpers =====
    /// Calculates FPS using a rolling average.
    void UpdateFpsCounter();

    /// Called at the start of each frame.
    void BeginFrame();

    /// Called at the end of each frame.
    void EndFrame();

    /**
     * @brief Caps the framerate by sleeping if frame finished early.
     * @param FrameStartTime Time when frame started.
     */
    void CapFrameRate(double FrameStartTime);
};

} // namespace Engine


