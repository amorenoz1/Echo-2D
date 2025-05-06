/**
 * @file Renderer.cpp
 * @brief Implementation of 2D batch rendering system using OpenGL
 * @details Handles rendering of geometric primitives and textured shapes with
 * dynamic buffer management.
 */

#include "external/glad.h"
#include <cmath>
#include <core/core.h>
#include <engine/ApplicationInfo.h>
#include <engine/Renderer.h>
#include <glm/gtc/matrix_transform.hpp>
#include <utils/ShaderUtils.h>

namespace Echo2D {

BatchRendererData g_BatchData = {0}; 


Renderer::Renderer() {
   m_Shader = new Utils::Shader();
   m_VBOMaxSize = sizeof(Utils::Vertex) * 1024;
   m_EBOMaxSize = sizeof(GLuint) * 1024;
   m_Projection = glm::ortho(20.0f, (float)g_AppInfo.ScreenWidth,
                           (float)g_AppInfo.ScreenHeight, 0.0f);

   glGenVertexArrays(1, &m_VAO);
   glGenBuffers(1, &m_VBO);
   glGenBuffers(1, &m_EBO);

   glBindVertexArray(m_VAO);
   glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
   glBufferData(GL_ARRAY_BUFFER, m_VBOMaxSize, nullptr, GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_EBOMaxSize, nullptr, GL_DYNAMIC_DRAW);

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

   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   m_Shader->Use();
   int MaxSamplers;
   glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &MaxSamplers);
   int Samplers[MaxSamplers];
   for (int i = 0; i < MaxSamplers; i++)
      Samplers[i] = i;
   m_Shader->SetIntV("Textures", MaxSamplers, Samplers);

   m_MaxTextureSlots = (GLuint)MaxSamplers;
}


void Renderer::AddCamera2D(Camera2D &Camera) { GetInstance().m_Camera = &Camera; }


void Renderer::InitDraw() {
   GetInstance().m_VertexData.clear();
   GetInstance().m_IndexData.clear();
   GetInstance().m_Textures.clear();
}


void Renderer::CheckAndFlush(const GLuint &VertexCount) {
   if ((GetInstance().m_IndexData.size() + (VertexCount - 2) * 3) *
      sizeof(GLuint) >=
      GetInstance().m_EBOMaxSize ||
      (GetInstance().m_VertexData.size() + VertexCount) * sizeof(Utils::Vertex) >=
      GetInstance().m_VBOMaxSize ||
      GetInstance().m_Textures.size() >= GetInstance().m_MaxTextureSlots) {
      EndDraw();
      Flush();
      InitDraw();
   }
}


void Renderer::AddTexture(Texture &Texture) {
   for (int i = 0; i < GetInstance().m_Textures.size(); i++) {
      if (Texture.GetID() == GetInstance().m_Textures.at(i)->GetID()) {
         return;
      }
   }
   GetInstance().m_Textures.push_back(&Texture);
}


int Renderer::FindTextureIndex(Texture &Texture) {
   for (int i = 0; i < GetInstance().m_Textures.size(); i++) {
      if (Texture.GetID() == GetInstance().m_Textures.at(i)->GetID()) {
         return i;
      }
   }
   return -1;
}


void Renderer::ClearScreenColor(glm::vec4 ScreenColor) {
   glm::vec4 pcColor = (1.0f / 255.0f) * ScreenColor;
   glClearColor(pcColor.r, pcColor.g, pcColor.b, pcColor.a);
}


void Renderer::DrawCircle(float Radius, glm::vec2 Center, glm::vec4 Color) {
   const int VertexCount = 49;
   CheckAndFlush(VertexCount);
   float Angle = 360.0f / (float)VertexCount;

   Utils::Vertex CenterVertex;
   CenterVertex.Position.x = Center.x;
   CenterVertex.Position.y = Center.y;
   CenterVertex.Position.z = 0.0f;
   CenterVertex.Color = Color;
   CenterVertex.TexCoords = {0.0f, 0.0f};
   CenterVertex.TextureIndex = -1.0f;
   GetInstance().m_VertexData.push_back(CenterVertex);

   for (int i = 0; i < VertexCount; i++) {
      float CurrAngle = Angle * i;
      Utils::Vertex TempVert;
      TempVert.Position.x = Radius * std::cos(glm::radians(CurrAngle)) + Center.x;
      TempVert.Position.y = Radius * std::sin(glm::radians(CurrAngle)) + Center.y;
      TempVert.Position.z = 0.0f;
      TempVert.Color = (1.0f / 255.0f) * Color;
      TempVert.TexCoords = {0.0f, 0.0f};
      TempVert.TextureIndex = -1.0f;
      GetInstance().m_VertexData.push_back(TempVert);
   }

   GLuint StartingIndex = GetInstance().m_VertexData.size() - (GLuint)VertexCount;
   for (GLuint i = 0; i < VertexCount - 2; i++) {
      GetInstance().m_IndexData.push_back(StartingIndex);
      GetInstance().m_IndexData.push_back(StartingIndex + i + 1);
      GetInstance().m_IndexData.push_back(StartingIndex + i + 2);
   }
}


void Renderer::DrawRect(glm::vec2 Dimensions, glm::vec2 Center,
                        glm::vec4 Color) {
   const GLuint VertexCount = 4;
   CheckAndFlush(VertexCount);

   Utils::Vertex vertices[4];
   glm::vec2 positions[4] = {{Center.x, Center.y},
      {Center.x + Dimensions.x, Center.y},
      {Center.x + Dimensions.x, Center.y + Dimensions.y},
      {Center.x, Center.y + Dimensions.y}};

   for (int i = 0; i < 4; i++) {
      vertices[i].Position = {Center.x, Center.y, 0.0f};
      vertices[i].Color = (1.0f / 255.f) * Color;
      vertices[i].TexCoords = {0.0f, 0.0f};
      vertices[i].TextureIndex = -1.0f;
      GetInstance().m_VertexData.push_back(vertices[i]);
   }

   GLuint StartingIndex = GetInstance().m_VertexData.size() - VertexCount;
   GLuint indices[] = {StartingIndex, StartingIndex + 1, StartingIndex + 2,
      StartingIndex, StartingIndex + 3, StartingIndex + 2};
   for (GLuint index : indices)
   GetInstance().m_IndexData.push_back(index);
}


void Renderer::DrawTriangle(glm::vec2 V0, glm::vec2 V1, glm::vec2 V2,
                            glm::vec4 Color) {
   const GLuint VertexCount = 3;
   CheckAndFlush(VertexCount);

   Utils::Vertex vertices[3];
   glm::vec2 positions[3] = {V0, V1, V2};

   for (int i = 0; i < 3; i++) {
      vertices[i].Position = {positions[i].x, positions[i].y, 0.0f};
      vertices[i].Color = (1.0f / 255.f) * Color;
      vertices[i].TexCoords = {0.0f, 0.0f};
      vertices[i].TextureIndex = -1.0f;
      GetInstance().m_VertexData.push_back(vertices[i]);
   }

   GLuint StartingIndex = GetInstance().m_VertexData.size() - VertexCount;
   GLuint indices[] = {StartingIndex, StartingIndex + 1, StartingIndex + 2};
   for (GLuint index : indices)
   GetInstance().m_IndexData.push_back(index);
}


void Renderer::DrawRectTexture(glm::vec2 Dimensions, glm::vec2 Position,
                                Texture &Tex, glm::vec4 Tint) {
   const GLuint VertexCount = 4;
   CheckAndFlush(VertexCount);
   AddTexture(Tex);
   int Index = FindTextureIndex(Tex);

   Utils::Vertex vertices[4];
   glm::vec2 positions[4] = {{Position.x, Position.y},
      {Position.x + Dimensions.x, Position.y},
      {Position.x + Dimensions.x, Position.y + Dimensions.y},
      {Position.x, Position.y + Dimensions.y}};

   glm::vec2 uvs[4] = {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}};

   for (int i = 0; i < 4; i++) {
      vertices[i].Position = {positions[i].x, positions[i].y, 0.0f};
      vertices[i].Color = (1.0f / 255.f) * Tint;
      vertices[i].TexCoords = uvs[i];
      vertices[i].TextureIndex = (float)Index;
      GetInstance().m_VertexData.push_back(vertices[i]);
   }

   GLuint StartingIndex = GetInstance().m_VertexData.size() - VertexCount;
   GLuint indices[] = {StartingIndex, StartingIndex + 1, StartingIndex + 2,
      StartingIndex, StartingIndex + 3, StartingIndex + 2};
   for (GLuint index : indices)
   GetInstance().m_IndexData.push_back(index);
}

void Renderer::DrawTriangleTexture(glm::vec2 V0, glm::vec2 V1, glm::vec2 V2,
                                   Texture &Tex, glm::vec4 Tint) {
   const GLuint VertexCount = 3;
   CheckAndFlush(VertexCount);
   AddTexture(Tex);
   int Index = FindTextureIndex(Tex);

   Utils::Vertex vertices[3];
   glm::vec2 positions[3] = {V0, V1, V2};
   glm::vec2 uvs[3] = {{0.0f, 0.0f}, {0.5f, 1.0f}, {1.0f, 0.0f}};

   for (int i = 0; i < 3; i++) {
      vertices[i].Position = {positions[i].x, positions[i].y, 0.0f};
      vertices[i].Color = (1.0f / 255.f) * Tint;
      vertices[i].TexCoords = uvs[i];
      vertices[i].TextureIndex = (float)Index;
      GetInstance().m_VertexData.push_back(vertices[i]);
   }

   GLuint StartingIndex = GetInstance().m_VertexData.size() - VertexCount;
   GLuint indices[] = {StartingIndex, StartingIndex + 1, StartingIndex + 2};
   for (GLuint index : indices)
   GetInstance().m_IndexData.push_back(index);
}

void Renderer::DrawCircleTexture(float Radius, glm::vec2 Center,
                                 Texture &Tex, glm::vec4 Tint) {
   const int VertexCount = 49;
   CheckAndFlush(VertexCount);
   AddTexture(Tex);
   int Index = FindTextureIndex(Tex);
   float Angle = 360.0f / (float)VertexCount;

   // Center vertex
   Utils::Vertex CenterVertex;
   CenterVertex.Position.x = Center.x;
   CenterVertex.Position.y = Center.y;
   CenterVertex.Position.z = 0.0f;
   CenterVertex.Color = (1.0f / 255.0f) * Tint;
   CenterVertex.TexCoords = {0.5f, 0.5f};
   CenterVertex.TextureIndex = (float)Index;
   GetInstance().m_VertexData.push_back(CenterVertex);

   // Perimeter vertices
   for (int i = 0; i < VertexCount; i++) {
      float CurrAngle = Angle * i;
      Utils::Vertex TempVert;
      TempVert.Position.x = Radius * std::cos(glm::radians(CurrAngle)) + Center.x;
      TempVert.Position.y = Radius * std::sin(glm::radians(CurrAngle)) + Center.y;
      TempVert.Position.z = 0.0f;
      TempVert.Color = (1.0f / 255.0f) * Tint;
      TempVert.TexCoords = {0.5f * std::cos(glm::radians(CurrAngle)) + 0.5f,
         0.5f * std::sin(glm::radians(CurrAngle)) + 0.5f};
      TempVert.TextureIndex = (float)Index;
      GetInstance().m_VertexData.push_back(TempVert);
   }

   GLuint StartingIndex = GetInstance().m_VertexData.size() - (GLuint)VertexCount;
   for (GLuint i = 0; i < VertexCount - 2; i++) {
      GetInstance().m_IndexData.push_back(StartingIndex);
      GetInstance().m_IndexData.push_back(StartingIndex + i + 1);
      GetInstance().m_IndexData.push_back(StartingIndex + i + 2);
   }
}

void Renderer::DrawText(const std::string &text, glm::vec2 position, Font &font,
                        glm::vec4 color, float scale) {
   // Starting X position (we'll advance this per character)
   float x = position.x;
   float y = position.y;

   for (char c : text) {
      Character &ch = font.GetCharacter(c);

      float xpos = x + ch.m_Bearing.x * scale;
      float ypos = y - ch.m_Bearing.y * scale;

      float w = ch.m_Size.x * scale;
      float h = ch.m_Size.y * scale;

      DrawRectTexture({w, h}, {xpos, ypos}, *ch.m_Texture, color);

      x += (ch.m_Advance >> 6) * scale;
   }
}

void Renderer::DrawRectSprite(glm::vec2 Dimensions, glm::vec2 Position,
                              Spritesheet &Sprites, int i,
                              int j, glm::vec4 Tint) {
    const GLuint VertexCount = 4;
    CheckAndFlush(VertexCount);
    AddTexture(Sprites.GetTex());
    int Index = FindTextureIndex(Sprites.GetTex());

    Utils::Vertex vertices[4];
    glm::vec2 positions[4] = {
        {Position.x, Position.y},
        {Position.x + Dimensions.x, Position.y},
        {Position.x + Dimensions.x, Position.y + Dimensions.y},
        {Position.x, Position.y + Dimensions.y}
    };

    // GetTexCoords returns (u, v, width, height)
    glm::vec4 texCoords = Sprites.GetTexCoords(i, j);
    float u = texCoords.x;
    float v = texCoords.y;
    float w = texCoords.z;
    float h = texCoords.w;

    glm::vec2 uvs[4] = {
        {u, v},
        {u + w, v},
        {u + w, v + h},
        {u, v + h}
    };

    glm::vec4 normalizedTint = Tint * (1.0f / 255.0f);

    for (int k = 0; k < 4; k++) {
        vertices[k].Position = {positions[k].x, positions[k].y, 0.0f};
        vertices[k].Color = normalizedTint;
        vertices[k].TexCoords = uvs[k];
        vertices[k].TextureIndex = static_cast<float>(Index);
        GetInstance().m_VertexData.push_back(vertices[k]);
    }

    GLuint StartingIndex = static_cast<GLuint>(GetInstance().m_VertexData.size() - VertexCount);
    GLuint indices[] = {
        StartingIndex, StartingIndex + 1, StartingIndex + 2,
        StartingIndex, StartingIndex + 3, StartingIndex + 2
    };

    for (GLuint index : indices)
        GetInstance().m_IndexData.push_back(index);
}

void Renderer::EndDraw() {
   glBindBuffer(GL_ARRAY_BUFFER, GetInstance().m_VBO);
   glBufferSubData(GL_ARRAY_BUFFER, 0,
                   sizeof(Utils::Vertex) * GetInstance().m_VertexData.size(),
                   GetInstance().m_VertexData.data());
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetInstance().m_EBO);
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                   sizeof(GLuint) * GetInstance().m_IndexData.size(),
                   GetInstance().m_IndexData.data());

   if (GetInstance().m_Camera != nullptr) {
      GetInstance().m_View = GetInstance().m_Camera->GetViewMatrix();
      GetInstance().m_Projection = GetInstance().m_Camera->GetProjectionMatrix();
   }

   GetInstance().m_Shader->Use();
   GetInstance().m_Shader->SetMat4("projection", GetInstance().m_Projection);
   GetInstance().m_Shader->SetMat4("model", GetInstance().m_Model);
   GetInstance().m_Shader->SetMat4("view", GetInstance().m_View);
   GetInstance().m_Shader->SetVec4("Tint", glm::vec4(1.0f));
}


void Renderer::Flush() {
   for (uint32_t i = 0; i < GetInstance().m_Textures.size(); i++) {
      GetInstance().m_Textures.at(i)->Bind(i);
   }

   glBindVertexArray(GetInstance().m_VAO);
   glDrawElements(GL_TRIANGLES, GetInstance().m_IndexData.size(), GL_UNSIGNED_INT,
                  0);
   g_BatchData.DrawCalls++;

   for (uint32_t i = 0; i < GetInstance().m_Textures.size(); i++) {
      GetInstance().m_Textures.at(i)->Unbind(i);
   }
}

Renderer::~Renderer() {
   delete m_Shader;
   glDeleteVertexArrays(1, &m_VAO);
   glDeleteBuffers(1, &m_VBO);
   glDeleteBuffers(1, &m_EBO);
}

} // namespace Echo2D
