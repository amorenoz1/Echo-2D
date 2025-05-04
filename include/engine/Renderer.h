#ifndef RENDERER_H
#define RENDERER_H

#include "core/core.h"
#include "engine/Camera.h"
#include "engine/Spritesheet.h"
#include "utils/ShaderUtils.h"
#include "engine/Font.h"
#include "utils/Utils.h"
#include "engine/Texture.h"
#include <vector>

namespace Echo2D {

/**
 * @struct BatchRendererData
 * @brief Tracks high-level renderer stats like draw call count.
 */
struct BatchRendererData {
   uint32_t DrawCalls = 0; ///< Number of draw calls issued this frame.
};

extern BatchRendererData BatchData;

/**
 * @class Renderer
 * @brief A 2D batch renderer with support for shapes, textures, and text.
 *
 * Singleton class responsible for buffering and drawing primitives
 * using OpenGL and batching to minimize draw calls.
 */
class Renderer : public Utils::Singleton<Renderer> {
   friend class Utils::Singleton<Renderer>;

public:
   // === Initialization & Finalization ===

   /// Initializes a 2D camera
   static void AddCamera2D(Camera2D& Camera);

   /// Initializes buffers and rendering state.
   static void InitDraw();

   /// Called once per frame before any drawing.
   static void ClearScreenColor(glm::vec4 ScreenColor);

   /// Called once per frame after all drawing is submitted.
   static void EndDraw();

   /// Sends all buffered draw calls to the GPU.
   static void Flush();

   // === Primitive Drawing ===

   /// Draws a filled circle.
   static void DrawCircle(float Radius, glm::vec2 Center, glm::vec4 Color);

   /// Draws a filled rectangle.
   static void DrawRect(glm::vec2 Dimensions, glm::vec2 Center, glm::vec4 Color);

   /// Draws a filled triangle.
   static void DrawTriangle(glm::vec2 V0, glm::vec2 V1, glm::vec2 V2, glm::vec4 Color);

   // === Textured Primitives ===

   /// Draws a textured rectangle.
   static void DrawRectTexture(glm::vec2 Dimensions, glm::vec2 Center, glm::vec4 Tint, Texture& Tex);

   /// Draws a textured triangle.
   static void DrawTriangleTexture(glm::vec2 V0, glm::vec2 V1, glm::vec2 V2, glm::vec4 Tint, Texture& Tex);

   /// Draws a textured circle.
   static void DrawCircleTexture(float Radius, glm::vec2 Center, glm::vec4 Tint, Texture& Tex);

   // === Textured Advanced ===

   /// Draw a rect from a spritesheet
   static void DrawRectSprite(glm::vec2 Dimensions, glm::vec2 Center, glm::vec4 Tint, Spritesheet &Sprites, int i, int j);

   // === Text Rendering ===

   /// Renders a string of text at the specified position.
   static void DrawText(const std::string& text, glm::vec2 position, Font& font, glm::vec4 color, float scale = 1.0f);

private:
   // === OpenGL Buffer Objects ===
   GLuint VAO = 0;
   GLuint VBO = 0;
   GLuint EBO = 0;

   GLuint VBOMaxSize = 0;         ///< Max size in bytes for vertex buffer.
   GLuint EBOMaxSize = 0;         ///< Max size in bytes for index buffer.
   GLuint MaxTextureSlots = 0;    ///< Max number of simultaneously bound textures.

   // === Internal Draw Buffers ===
   std::vector<Utils::Vertex> VertexData; ///< Accumulated vertex data.
   std::vector<GLuint> IndexData;         ///< Accumulated index data.
   std::vector<Texture*> Textures;        ///< Currently bound textures.

   Utils::Shader* Shader = nullptr;       ///< Active rendering shader.


   // === Matrices ===
   glm::mat4 Projection = glm::mat4(1.0f); 
   glm::mat4 View = glm::mat4(1.0f);
   glm::mat4 Model = glm::mat4(1.0f);

   // === Camera ===
   Camera2D *Camera = nullptr;

   // === Internal Helpers ===

   /**
     * @brief Checks if the buffer has space for more vertices; flushes if needed.
     * @param VertexCount Number of vertices to add.
     */
   static void CheckAndFlush(const GLuint& VertexCount);

   /**
     * @brief Finds the texture slot index of a texture, or -1 if not bound.
     */
   static int FindTextureIndex(Texture& Texture);

   /**
     * @brief Adds a texture to the batch if there's room.
     */
   static void AddTexture(Texture& Texture);

   Renderer();
   ~Renderer();
};

} // namespace Echo2D

#endif // RENDERER_H

