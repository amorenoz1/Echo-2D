#include "engine/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <external/easylogging++.h>

namespace Echo2D {

/**
 * @brief Construct a new Camera2D object with a given viewport size.
 * 
 * Initializes the orthographic projection matrix, sets the initial position to (0, 0),
 * rotation to 0 degrees, and zoom to 1.0. Logs the creation of the camera.
 * 
 * @param ViewportWidth The width of the viewport in pixels.
 * @param ViewportHeight The height of the viewport in pixels.
 */
Camera2D::Camera2D(float ViewportWidth, float ViewportHeight) {
   m_ViewportSize = {ViewportWidth, ViewportHeight};

   // Center-origin orthographic projection
   float halfW = ViewportWidth / 2.0f;
   float halfH = ViewportHeight / 2.0f;
   m_ProjectionMatrix = glm::ortho(-halfW, halfW, halfH, -halfH);

   m_Zoom = 1.0f;
   m_Position = glm::vec2(0.0f);
   m_Rotation = 0.0f;

   LOG(INFO) << "Camera2D created with viewport: "
      << ViewportWidth << "x" << ViewportHeight;

   UpdateCameraState();
}

/**
 * @brief Destroy the Camera2D object.
 *
 * Logs the destruction of the camera.
 */
Camera2D::~Camera2D() {
   LOG(INFO) << "Camera2D destroyed.";
}

/**
 * @brief Set the position of the camera.
 * 
 * Updates the camera's position and recalculates the view matrix.
 * Logs the new position.
 * 
 * @param Position The new position of the camera.
 */
void Camera2D::SetPosition(const glm::vec2& Position) {
   m_Position = Position;
   LOG(INFO) << "Camera2D position set to: " << m_Position.x << ", " << m_Position.y;
   UpdateCameraState();
}

/**
 * @brief Set the rotation of the camera.
 * 
 * Updates the camera's rotation and recalculates the view matrix.
 * Logs the new rotation.
 * 
 * @param Degrees The new rotation in degrees.
 */
void Camera2D::SetRotation(float Degrees) {
   m_Rotation = Degrees;
   LOG(INFO) << "Camera2D rotation set to: " << m_Rotation << " degrees";
   UpdateCameraState();
}

/**
 * @brief Set the zoom of the camera.
 * 
 * Updates the camera's zoom and recalculates the view matrix.
 * Logs the new zoom level.
 * 
 * @param Zoom The new zoom level.
 */
void Camera2D::SetZoom(float Zoom) {
   m_Zoom = Zoom;
   LOG(INFO) << "Camera2D zoom set to: " << m_Zoom;
   UpdateCameraState();
}

/**
 * @brief Get the current position of the camera.
 * 
 * @return The camera's position.
 */
glm::vec2 Camera2D::GetPosition() {
   return m_Position;
}

/**
 * @brief Get the current zoom level of the camera.
 * 
 * @return The camera's zoom level.
 */
float Camera2D::GetZoom() {
   return m_Zoom;
}

/**
 * @brief Get the current rotation of the camera.
 * 
 * @return The camera's rotation in degrees.
 */
float Camera2D::GetRotation() {
   return m_Rotation;
}

/**
 * @brief Get the camera's projection matrix.
 * 
 * @return The projection matrix.
 */
const glm::mat4& Camera2D::GetProjectionMatrix() const {
   return m_ProjectionMatrix;
}

/**
 * @brief Get the camera's view matrix.
 * 
 * @return The view matrix.
 */
const glm::mat4& Camera2D::GetViewMatrix() const {
   return m_ViewMatrix;
}

/**
 * @brief Move the camera by a given delta.
 * 
 * Moves the camera by the specified delta in the x and y directions.
 * Recalculates the view matrix and logs the change.
 * 
 * @param Delta The change in position.
 */
void Camera2D::Move(glm::vec2 Delta) {
   m_Position += Delta;
   LOG(INFO) << "Camera2D moved by delta: " << Delta.x << ", " << Delta.y;
   UpdateCameraState();
}

/**
 * @brief Zoom the camera by a factor.
 * 
 * Multiplies the camera's zoom by the specified factor.
 * Recalculates the view matrix and logs the change.
 * 
 * @param Factor The zoom factor.
 */
void Camera2D::Zoom(float Factor) {
   m_Zoom *= Factor;
   LOG(INFO) << "Camera2D zoomed by factor: " << Factor << ", new zoom: " << m_Zoom;
   UpdateCameraState();
}

/**
 * @brief Rotate the camera by a specified number of degrees.
 * 
 * Increases or decreases the camera's rotation by the specified angle.
 * Recalculates the view matrix and logs the change.
 * 
 * @param Degrees The angle to rotate the camera.
 */
void Camera2D::Rotate(float Degrees) {
   m_Rotation += Degrees;
   LOG(INFO) << "Camera2D rotated by: " << Degrees << " degrees, new rotation: " << m_Rotation;
   UpdateCameraState();
}

/**
 * @brief Update the camera's view matrix.
 * 
 * Recalculates the view matrix based on the camera's position, zoom, and rotation.
 */
void Camera2D::UpdateCameraState() {
   m_ViewMatrix = glm::mat4(1.0f);

   // Correct order: scale → rotate → translate (inverse order of model transform)
   m_ViewMatrix = glm::scale(m_ViewMatrix, glm::vec3(1.0f / m_Zoom));
   m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(-m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
   m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(-m_Position, 0.0f));

   LOG(DEBUG) << "Camera2D view matrix updated.";
}

} // namespace Echo2D

