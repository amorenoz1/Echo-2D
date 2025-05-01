#include "engine/Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <external/easylogging++.h>

namespace Echo2D {

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

Camera2D::~Camera2D() {
   LOG(INFO) << "Camera2D destroyed.";
}

void Camera2D::SetPosition(const glm::vec2& Position) {
   m_Position = Position;
   LOG(INFO) << "Camera2D position set to: " << m_Position.x << ", " << m_Position.y;
   UpdateCameraState();
}

void Camera2D::SetRotation(float Degrees) {
   m_Rotation = Degrees;
   LOG(INFO) << "Camera2D rotation set to: " << m_Rotation << " degrees";
   UpdateCameraState();
}

void Camera2D::SetZoom(float Zoom) {
   m_Zoom = Zoom;
   LOG(INFO) << "Camera2D zoom set to: " << m_Zoom;
   UpdateCameraState();
}

glm::vec2 Camera2D::GetPosition() {
   return m_Position;
}

float Camera2D::GetRotation() {
   return m_Rotation;
}

float Camera2D::GetZoom() {
   return m_Zoom;
}

const glm::mat4& Camera2D::GetProjectionMatrix() const {
   return m_ProjectionMatrix;
}

const glm::mat4& Camera2D::GetViewMatrix() const {
   return m_ViewMatrix;
}

void Camera2D::Move(glm::vec2 Delta) {
   m_Position += Delta;
   LOG(INFO) << "Camera2D moved by delta: " << Delta.x << ", " << Delta.y;
   UpdateCameraState();
}

void Camera2D::Zoom(float Factor) {
   m_Zoom *= Factor;
   LOG(INFO) << "Camera2D zoomed by factor: " << Factor << ", new zoom: " << m_Zoom;
   UpdateCameraState();
}

void Camera2D::Rotate(float Degrees) {
   m_Rotation += Degrees;
   LOG(INFO) << "Camera2D rotated by: " << Degrees << " degrees, new rotation: " << m_Rotation;
   UpdateCameraState();
}

void Camera2D::UpdateCameraState() {
   m_ViewMatrix = glm::mat4(1.0f);

   // Correct order: scale → rotate → translate (inverse order of model transform)
   m_ViewMatrix = glm::scale(m_ViewMatrix, glm::vec3(1.0f / m_Zoom));
   m_ViewMatrix = glm::rotate(m_ViewMatrix, glm::radians(-m_Rotation), glm::vec3(0.0f, 0.0f, 1.0f));
   m_ViewMatrix = glm::translate(m_ViewMatrix, glm::vec3(-m_Position, 0.0f));

   LOG(DEBUG) << "Camera2D view matrix updated.";
}

} // namespace Echo2D

