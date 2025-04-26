/** 
 * @file Renderer.cpp
 * @brief Implementation of 2D batch rendering system using OpenGL
 * @details Handles rendering of geometric primitives and textured shapes with dynamic buffer management.
 */

#include "external/glad.h"
#include <cmath>
#include <core/core.h>
#include <engine/ApplicationInfo.h>
#include <engine/Renderer.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <ostream>
#include <utils/ShaderUtils.h>

namespace Engine {

BatchRendererData BatchData = {0}; ///< Global batch rendering statistics tracker

/**
 * @brief Converts screen coordinates to Normalized Device Coordinates (NDC)
 * @param ScreenX X coordinate in screen space
 * @param ScreenY Y coordinate in screen space
 * @return glm::vec2 Coordinates in OpenGL's NDC space (-1 to 1)
 */
glm::vec2 ScreenToNDC(float ScreenX, float ScreenY) {
   float ndcX = (2.0f * ScreenX) / AppInfo.ScreenWidth - 1.0f;
   float ndcY = 1.0f - (2.0f * ScreenY) / AppInfo.ScreenHeight;
   return glm::vec2(ndcX, ndcY);
}

/**
 * @brief Constructs a Renderer and initializes OpenGL resources
 * @details Creates VAO/VBO/EBO, sets up vertex attributes, and initializes shader
 */
Renderer::Renderer() {
   Shader = new Utils::Shader();
   VBOMaxSize = sizeof(Utils::Vertex) * 1024;
   EBOMaxSize = sizeof(GLuint) * 1024;

   glGenVertexArrays(1, &VAO);
   glGenBuffers(1, &VBO);
   glGenBuffers(1, &EBO);

   glBindVertexArray(VAO);
   glBindBuffer(GL_ARRAY_BUFFER, VBO);
   glBufferData(GL_ARRAY_BUFFER, VBOMaxSize, nullptr, GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, EBOMaxSize, nullptr, GL_DYNAMIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Utils::Vertex),
                         (void *)offsetof(Utils::Vertex, Position));
   glEnableVertexAttribArray(0);

   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Utils::Vertex),
                         (void *)offsetof(Utils::Vertex, Color));
   glEnableVertexAttribArray(1);

   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Utils::Vertex),
                         (void *)offsetof(Utils::Vertex, TexCoords));
   glEnableVertexAttribArray(2);

   glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Utils::Vertex),
                         (void *)offsetof(Utils::Vertex, TextureIndex));
   glEnableVertexAttribArray(3);

   Shader->Use();
   int MaxSamplers;
   glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxSamplers);
   int Samplers[MaxSamplers];
   for (int i = 0; i < MaxSamplers; i++)
      Samplers[i] = i;
   Shader->SetIntV("Textures", MaxSamplers, Samplers);

   MaxTextureSlots = (GLuint)MaxSamplers;
}

/**
 * @brief Prepares for a new drawing batch
 * @details Clears vertex/index buffers and texture references
 */
void Renderer::InitDraw() {
   GetInstance().VertexData.clear();
   GetInstance().IndexData.clear();
   GetInstance().Textures.clear();
}

/**
 * @brief Checks if buffers need flushing before adding new vertices
 * @param VertexCount Number of vertices about to be added
 * @note Automatically flushes if buffers are full
 */
void Renderer::CheckAndFlush(const GLuint &VertexCount) {
   if ((GetInstance().IndexData.size() + (VertexCount - 2) * 3) * sizeof(GLuint) >= GetInstance().EBOMaxSize ||
       (GetInstance().VertexData.size() + VertexCount) * sizeof(Utils::Vertex) >= GetInstance().VBOMaxSize ||
       GetInstance().Textures.size() >= GetInstance().MaxTextureSlots) {
      EndDraw();
      Flush();
      InitDraw();
   }
}

/**
 * @brief Registers a texture for use in current batch
 * @param Texture Texture to add to the batch
 * @note Skips duplicates to maintain texture uniqueness
 */
void Renderer::AddTexture(Texture &Texture) {
   for (int i = 0; i < GetInstance().Textures.size(); i++) {
      if (Texture.GetID() == GetInstance().Textures.at(i)->GetID()) {
         return;
      }
   }
   GetInstance().Textures.push_back(&Texture);
}

/**
 * @brief Finds texture index in current batch
 * @param Texture Texture to search for
 * @return int Index of texture if found, -1 otherwise
 */
int Renderer::FindTextureIndex(Texture &Texture) {
   for (int i = 0; i < GetInstance().Textures.size(); i++) {
      if (Texture.GetID() == GetInstance().Textures.at(i)->GetID()) {
         return i;
      }
   }
   return -1;
}

/**
 * @brief Sets screen clear color
 * @param ScreenColor RGBA color in 0-255 range
 * @note Automatically converts to 0-1 range for OpenGL
 */
void Renderer::ClearScreenColor(glm::vec4 ScreenColor) {
   glm::vec4 pcColor = (1.0f / 255.0f) * ScreenColor;
   glClearColor(pcColor.r, pcColor.g, pcColor.b, pcColor.a);
}

/**
 * @brief Draws a colored circle
 * @param Radius Circle radius in screen pixels
 * @param Center Center position in screen coordinates
 * @param Color RGBA color (0-255 range, auto-normalized)
 * @todo Replace hardcoded 49 vertices with configurable resolution
 */
void Renderer::DrawCircle(float Radius, glm::vec2 Center, glm::vec4 Color) {
   const int VertexCount = 49;
   CheckAndFlush(VertexCount);
   float Angle = 360.0f / (float)VertexCount;

   Utils::Vertex CenterVertex;
   CenterVertex.Position.x = ScreenToNDC(Center.x, 0.0f).x;
   CenterVertex.Position.y = ScreenToNDC(0.0f, Center.y).y;
   CenterVertex.Position.z = 0.0f;
   CenterVertex.Color = Color;
   CenterVertex.TexCoords = {0.0f, 0.0f};
   CenterVertex.TextureIndex = -1.0f;
   GetInstance().VertexData.push_back(CenterVertex);

   for (int i = 0; i < VertexCount; i++) {
      float CurrAngle = Angle * i;
      Utils::Vertex TempVert;
      TempVert.Position.x = ScreenToNDC((Radius * std::cos(glm::radians(CurrAngle))) + Center.x, 0.0f).x;
      TempVert.Position.y = ScreenToNDC(0.0f, (Radius * std::sin(glm::radians(CurrAngle))) + Center.y).y;
      TempVert.Position.z = 0.0f;
      TempVert.Color = (1.0f / 255.0f) * Color;
      TempVert.TexCoords = {0.0f, 0.0f};
      TempVert.TextureIndex = -1.0f;
      GetInstance().VertexData.push_back(TempVert);
   }

   GLuint StartingIndex = GetInstance().VertexData.size() - (GLuint)VertexCount;
   for (GLuint i = 0; i < VertexCount - 2; i++) {
      GetInstance().IndexData.push_back(StartingIndex);
      GetInstance().IndexData.push_back(StartingIndex + i + 1);
      GetInstance().IndexData.push_back(StartingIndex + i + 2);
   }
}

/**
 * @brief Draws a colored rectangle
 * @param Dimensions Width/Height in screen pixels
 * @param Center Center position in screen coordinates
 * @param Color RGBA color (0-255 range, auto-normalized)
 */
void Renderer::DrawRect(glm::vec2 Dimensions, glm::vec2 Center, glm::vec4 Color) {
   const GLuint VertexCount = 4;
   CheckAndFlush(VertexCount);

   Utils::Vertex vertices[4];
   glm::vec2 positions[4] = {
      {Center.x, Center.y},
      {Center.x + Dimensions.x, Center.y},
      {Center.x + Dimensions.x, Center.y + Dimensions.y},
      {Center.x, Center.y + Dimensions.y}
   };

   for (int i = 0; i < 4; i++) {
      glm::vec2 ndc = ScreenToNDC(positions[i].x, positions[i].y);
      vertices[i].Position = {ndc.x, ndc.y, 0.0f};
      vertices[i].Color = (1.0f / 255.f) * Color;
      vertices[i].TexCoords = {0.0f, 0.0f};
      vertices[i].TextureIndex = -1.0f;
      GetInstance().VertexData.push_back(vertices[i]);
   }

   GLuint StartingIndex = GetInstance().VertexData.size() - VertexCount;
   GLuint indices[] = {StartingIndex, StartingIndex+1, StartingIndex+2,
                       StartingIndex, StartingIndex+3, StartingIndex+2};
   for (GLuint index : indices) GetInstance().IndexData.push_back(index);
}

/**
 * @brief Draws a colored triangle
 * @param V0 First vertex position in screen coordinates
 * @param V1 Second vertex position in screen coordinates
 * @param V2 Third vertex position in screen coordinates
 * @param Color RGBA color (0-255 range, auto-normalized)
 * @note Creates a solid-color triangle without texture
 */
void Renderer::DrawTriangle(glm::vec2 V0, glm::vec2 V1, glm::vec2 V2, glm::vec4 Color) {
   const GLuint VertexCount = 3;
   CheckAndFlush(VertexCount);

   Utils::Vertex vertices[3];
   glm::vec2 positions[3] = {V0, V1, V2};

   for (int i = 0; i < 3; i++) {
      glm::vec2 ndc = ScreenToNDC(positions[i].x, positions[i].y);
      vertices[i].Position = {ndc.x, ndc.y, 0.0f};
      vertices[i].Color = (1.0f / 255.f) * Color;
      vertices[i].TexCoords = {0.0f, 0.0f};
      vertices[i].TextureIndex = -1.0f;
      GetInstance().VertexData.push_back(vertices[i]);
   }

   GLuint StartingIndex = GetInstance().VertexData.size() - VertexCount;
   GLuint indices[] = {StartingIndex, StartingIndex+1, StartingIndex+2};
   for (GLuint index : indices) GetInstance().IndexData.push_back(index);
}

/**
 * @brief Draws a textured rectangle
 * @param Dimensions Width/Height in screen pixels
 * @param Center Center position in screen coordinates
 * @param Tint Color multiplier (0-255 range, auto-normalized)
 * @param Tex Texture to apply
 * @note Texture coordinates are set to full UV range (0,0 to 1,1)
 */
void Renderer::DrawRectTexture(glm::vec2 Dimensions, glm::vec2 Center, glm::vec4 Tint, Texture &Tex) {
   const GLuint VertexCount = 4;
   CheckAndFlush(VertexCount);
   AddTexture(Tex);
   int Index = FindTextureIndex(Tex);

   Utils::Vertex vertices[4];
   glm::vec2 positions[4] = {
      {Center.x, Center.y},
      {Center.x + Dimensions.x, Center.y},
      {Center.x + Dimensions.x, Center.y + Dimensions.y},
      {Center.x, Center.y + Dimensions.y}
   };

   glm::vec2 uvs[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

   for (int i = 0; i < 4; i++) {
      glm::vec2 ndc = ScreenToNDC(positions[i].x, positions[i].y);
      vertices[i].Position = {ndc.x, ndc.y, 0.0f};
      vertices[i].Color = (1.0f / 255.f) * Tint;
      vertices[i].TexCoords = uvs[i];
      vertices[i].TextureIndex = (float)Index;
      GetInstance().VertexData.push_back(vertices[i]);
   }

   GLuint StartingIndex = GetInstance().VertexData.size() - VertexCount;
   GLuint indices[] = {StartingIndex, StartingIndex+1, StartingIndex+2,
                       StartingIndex, StartingIndex+3, StartingIndex+2};
   for (GLuint index : indices) GetInstance().IndexData.push_back(index);
}

/**
 * @brief Draws a textured triangle
 * @param V0 First vertex position in screen coordinates
 * @param V1 Second vertex position in screen coordinates
 * @param V2 Third vertex position in screen coordinates
 * @param Tint Color multiplier (0-255 range, auto-normalized)
 * @param Tex Texture to apply
 * @note Uses predefined UV coordinates (bottom-left, top-center, bottom-right)
 */
void Renderer::DrawTriangleTexture(glm::vec2 V0, glm::vec2 V1, glm::vec2 V2, glm::vec4 Tint, Texture &Tex) {
   const GLuint VertexCount = 3;
   CheckAndFlush(VertexCount);
   AddTexture(Tex);
   int Index = FindTextureIndex(Tex);

   Utils::Vertex vertices[3];
   glm::vec2 positions[3] = {V0, V1, V2};
   glm::vec2 uvs[3] = {{0.0f, 0.0f}, {0.5f, 1.0f}, {1.0f, 0.0f}};

   for (int i = 0; i < 3; i++) {
      glm::vec2 ndc = ScreenToNDC(positions[i].x, positions[i].y);
      vertices[i].Position = {ndc.x, ndc.y, 0.0f};
      vertices[i].Color = (1.0f / 255.f) * Tint;
      vertices[i].TexCoords = uvs[i];
      vertices[i].TextureIndex = (float)Index;
      GetInstance().VertexData.push_back(vertices[i]);
   }

   GLuint StartingIndex = GetInstance().VertexData.size() - VertexCount;
   GLuint indices[] = {StartingIndex, StartingIndex+1, StartingIndex+2};
   for (GLuint index : indices) GetInstance().IndexData.push_back(index);
}

/**
 * @brief Draws a textured circle
 * @param Radius Circle radius in screen pixels
 * @param Center Center position in screen coordinates
 * @param Tint Color multiplier (0-255 range, auto-normalized)
 * @param Tex Texture to apply
 * @note Texture coordinates are radially mapped from center
 * @todo Make vertex count configurable instead of hardcoded 49
 */
void Renderer::DrawCircleTexture(float Radius, glm::vec2 Center, glm::vec4 Tint, Texture &Tex) {
   const int VertexCount = 49;
   CheckAndFlush(VertexCount);
   AddTexture(Tex);
   int Index = FindTextureIndex(Tex);
   float Angle = 360.0f / (float)VertexCount;

   // Center vertex
   Utils::Vertex CenterVertex;
   CenterVertex.Position.x = ScreenToNDC(Center.x, 0.0f).x;
   CenterVertex.Position.y = ScreenToNDC(0.0f, Center.y).y;
   CenterVertex.Position.z = 0.0f;
   CenterVertex.Color = (1.0f / 255.0f) * Tint;
   CenterVertex.TexCoords = {0.5f, 0.5f};
   CenterVertex.TextureIndex = (float)Index;
   GetInstance().VertexData.push_back(CenterVertex);

   // Perimeter vertices
   for (int i = 0; i < VertexCount; i++) {
      float CurrAngle = Angle * i;
      Utils::Vertex TempVert;
      TempVert.Position.x = ScreenToNDC((Radius * std::cos(glm::radians(CurrAngle))) + Center.x, 0.0f).x;
      TempVert.Position.y = ScreenToNDC(0.0f, (Radius * std::sin(glm::radians(CurrAngle))) + Center.y).y;
      TempVert.Position.z = 0.0f;
      TempVert.Color = (1.0f / 255.0f) * Tint;
      TempVert.TexCoords = {
         0.5f * std::cos(glm::radians(CurrAngle)) + 0.5f,
         0.5f * std::sin(glm::radians(CurrAngle)) + 0.5f
      };
      TempVert.TextureIndex = (float)Index;
      GetInstance().VertexData.push_back(TempVert);
   }

   GLuint StartingIndex = GetInstance().VertexData.size() - (GLuint)VertexCount;
   for (GLuint i = 0; i < VertexCount - 2; i++) {
      GetInstance().IndexData.push_back(StartingIndex);
      GetInstance().IndexData.push_back(StartingIndex + i + 1);
      GetInstance().IndexData.push_back(StartingIndex + i + 2);
   }
}

/**
 * @brief Finalizes batch data and uploads to GPU
 * @note Debug vertex printing can be enabled via 'printVertex' flag
 */
void Renderer::EndDraw() {
   glBindBuffer(GL_ARRAY_BUFFER, GetInstance().VBO);
   glBufferSubData(GL_ARRAY_BUFFER, 0,
                   sizeof(Utils::Vertex) * GetInstance().VertexData.size(),
                   GetInstance().VertexData.data());
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetInstance().EBO);
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                   sizeof(GLuint) * GetInstance().IndexData.size(),
                   GetInstance().IndexData.data());

   GetInstance().Shader->Use();
   GetInstance().Shader->SetMat4("projection", glm::mat4(1.0f));
   GetInstance().Shader->SetMat4("model", glm::mat4(1.0f));
   GetInstance().Shader->SetMat4("view", glm::mat4(1.0f));
   GetInstance().Shader->SetVec4("Tint", glm::vec4(1.0f));

}

/**
 * @brief Submits current batch to GPU for rendering
 * @details Binds textures, issues draw call, and updates statistics
 */
void Renderer::Flush() {
   for (uint32_t i = 0; i < GetInstance().Textures.size(); i++) {
      GetInstance().Textures.at(i)->Bind(i);
   }

   glBindVertexArray(GetInstance().VAO);
   glDrawElements(GL_TRIANGLES, GetInstance().IndexData.size(), GL_UNSIGNED_INT, 0);
   BatchData.DrawCalls++;

   for (uint32_t i = 0; i < GetInstance().Textures.size(); i++) {
      GetInstance().Textures.at(i)->Unbind(i);
   }
}

/**
 * @brief Destructor - cleans up OpenGL resources
 */
Renderer::~Renderer() {
   delete Shader;
   glDeleteVertexArrays(1, &VAO);
   glDeleteBuffers(1, &VBO);
   glDeleteBuffers(1, &EBO);
}

} // namespace Engine
