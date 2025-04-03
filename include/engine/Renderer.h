#ifndef RENDERER_H
#define RENDERER_H

#include "core/core.h"
#include "utils/Utils.h"
#include "utils/ShaderUtils.h"
#include <vector>

namespace Engine {

class Renderer : public Utils::Singleton<Renderer> {
   friend class Utils::Singleton<Renderer>;

public:
   static void InitDraw(int ScreenWidth, int ScreenHeight);
   static void ClearScreenColor(glm::vec4 ScreenColor);
   static void DrawCircle(float Radius, glm::vec2 Center, glm::vec4 Color);
   static void DrawRect(glm::vec2 Dimensions, glm::vec2 Center, glm::vec4 Color);
   static void DrawTriangle(glm::vec2 V0, glm::vec2 V1, glm::vec2 V2, glm::vec4 Color);
   static void EndDraw();

private:
   int ScreenWidth;
   int ScreenHeight;
   GLuint VAO;
   GLuint VBO;
   GLuint EBO;
   std::vector<Utils::Vertex> VertexData;
   std::vector<GLuint> IndexData;
   Utils::Shader *Shader;

   static glm::vec2 ScreenToNDC(float ScreenX, float ScreenY);
   static void ExpandBuffers();

   Renderer();
   ~Renderer();
};

}// Engine

#endif
