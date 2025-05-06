#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace Echo2D {

/**
 * @class Camera2D
 * @brief A 2D camera class for managing the viewport, zoom, rotation, and position.
 *
 * This class allows you to manipulate the camera's position, rotation, and zoom level,
 * providing an orthographic projection matrix for rendering. It supports camera movement
 * and transformation operations like pan, zoom, and rotate.
 */
class Camera2D {
public:
   /**
    * @brief Construct a new Camera2D object.
    * @param ViewportWidth Width of the viewport in pixels.
    * @param ViewportHeight Height of the viewport in pixels.
    *
    * Initializes the camera with the given viewport size and sets up an orthographic
    * projection matrix.
    */
   Camera2D(float ViewportWidth, float ViewportHeight);

   /// Destructor for Camera2D.
   ~Camera2D();

   /**
    * @brief Set the camera's position.
    * @param Position The new position of the camera.
    */
   void SetPosition(const glm::vec2& Position);

   /**
    * @brief Set the camera's rotation.
    * @param Degrees The rotation angle in degrees.
    */
   void SetRotation(float Degrees);

   /**
    * @brief Set the camera's zoom level.
    * @param Zoom The zoom factor (default is 1.0).
    */
   void SetZoom(float Zoom);

   /**
    * @brief Get the current position of the camera.
    * @return The camera's current position as a glm::vec2.
    */
   glm::vec2 GetPosition();

   /**
    * @brief Get the current zoom level of the camera.
    * @return The camera's current zoom level.
    */
   float GetZoom();

   /**
    * @brief Get the current rotation of the camera.
    * @return The camera's current rotation in degrees.
    */
   float GetRotation();

   /**
    * @brief Get the camera's projection matrix.
    * @return The orthographic projection matrix of the camera.
    */
   const glm::mat4& GetProjectionMatrix() const;

   /**
    * @brief Get the camera's view matrix.
    * @return The view matrix of the camera.
    */
   const glm::mat4& GetViewMatrix() const;

   /**
    * @brief Move the camera by a delta.
    * @param Delta The movement in x and y.
    */
   void Move(glm::vec2 Delta);

   /**
    * @brief Zoom the camera by a factor.
    * @param Factor The zoom factor (greater than 1 to zoom in, less than 1 to zoom out).
    */
   void Zoom(float Factor);

   /**
    * @brief Rotate the camera by a certain number of degrees.
    * @param Degrees The angle to rotate the camera.
    */
   void Rotate(float Degrees);

private:
   glm::vec2 m_Position; ///< The camera's position.
   glm::vec2 m_ViewportSize; ///< The size of the viewport.
   glm::mat4 m_ProjectionMatrix; ///< The camera's projection matrix.
   glm::mat4 m_ViewMatrix; ///< The camera's view matrix.
   float m_Zoom; ///< The camera's zoom factor.
   float m_Rotation; ///< The camera's rotation in degrees.

   /**
    * @brief Update the camera's view matrix based on the current position, zoom, and rotation.
    */
   void UpdateCameraState();
};

} // namespace Echo2D

#endif

