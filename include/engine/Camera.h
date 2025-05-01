#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

namespace Echo2D {

class Camera2D {
public:
   Camera2D(float ViewportWidth, float ViewportHeight);
   ~Camera2D();

   void SetPosition(const glm::vec2& Position);
   void SetRotation(float Degrees);
   void SetZoom(float Zoom);

   glm::vec2 GetPosition();
   float GetZoom();
   float GetRotation();

   const glm::mat4& GetProjectionMatrix() const;
   const glm::mat4& GetViewMatrix() const;

   void Move(glm::vec2 Delta);
   void Zoom(float Factor);
   void Rotate(float Degrees);

private:
   glm::vec2 m_Position;
   glm::vec2 m_ViewportSize;
   glm::mat4 m_ProjectionMatrix;
   glm::mat4 m_ViewMatrix;
   float m_Zoom;
   float m_Rotation;

   void UpdateCameraState();
};

}

#endif
