#ifndef RENDERER_H
#define RENDERER_H

#include "core/core.h"
#include "utils/ShaderUtils.h"
#include "utils/Utils.h"
#include "engine/Texture.h"
#include <vector>

namespace Engine {

struct BatchRendererData {
   uint32_t DrawCalls;
};

extern BatchRendererData BatchData;

class Renderer : public Utils::Singleton<Renderer> {
   friend class Utils::Singleton<Renderer>;

public:
   static void InitDraw();
   static void ClearScreenColor(glm::vec4 ScreenColor);
   static void DrawCircle(float Radius, glm::vec2 Center, glm::vec4 Color);
   static void DrawRect(glm::vec2 Dimensions, glm::vec2 Center, glm::vec4 Color);
   static void DrawTriangle(glm::vec2 V0, glm::vec2 V1, glm::vec2 V2, glm::vec4 Color);
   static void DrawRectTexture(glm::vec2 Dimensions, glm::vec2 Center, glm::vec4 Tint, Texture &Tex);
   static void DrawTriangleTexture(glm::vec2 V0, glm::vec2 V1, glm::vec2 V2, glm::vec4 Tint, Texture &Tex);
   static void DrawCircleTexture(float Radius, glm::vec2 Center, glm::vec4 Tint, Texture &Tex);
   static void EndDraw();
   static void Flush();

private:
   GLuint VAO;
   GLuint VBO;
   GLuint EBO;
   GLuint VBOMaxSize;
   GLuint EBOMaxSize;
   GLuint MaxTextureSlots;
   std::vector<Utils::Vertex> VertexData;
   std::vector<GLuint> IndexData;
   std::vector<Texture*> Textures;
   Utils::Shader *Shader;

   static void CheckAndFlush(const GLuint &VertexCount);
   static int FindTextureIndex(Texture &Texture);
   static void AddTexture(Texture &Texture);

   Renderer();
   ~Renderer();
};

} // namespace Engine

#endif
