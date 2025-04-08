#include <cmath>
#include <core/core.h>
#include <engine/ApplicationInfo.h>
#include <engine/Renderer.h>
#include <external/glad.h>
#include <iostream>
#include <utils/ShaderUtils.h>

namespace Engine {

BatchRendererData BatchData = {0};

glm::vec2 ScreenToNDC(float ScreenX, float ScreenY) {
   float ndcX = (2.0f * ScreenX) / AppInfo.ScreenWidth - 1.0f;
   float ndcY = 1.0f - (2.0f * ScreenY) / AppInfo.ScreenHeight;
   return glm::vec2(ndcX, ndcY);
}

Renderer::Renderer() {
   this->Shader = new Utils::Shader();
   this->VBOMaxSize = sizeof(Utils::Vertex) * 1024;
   this->EBOMaxSize = sizeof(GLuint) * 1024;

   glGenVertexArrays(1, &this->VAO);
   glGenBuffers(1, &this->VBO);
   glGenBuffers(1, &this->EBO);

   glBindVertexArray(this->VAO);
   glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
   glBufferData(GL_ARRAY_BUFFER, this->VBOMaxSize, nullptr, GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->EBOMaxSize, nullptr,
                GL_DYNAMIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float),
                         (void *)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float),
                         (void *)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float),
                         (void *)(7 * sizeof(float)));
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float),
                         (void *)(9 * sizeof(float)));
   glEnableVertexAttribArray(3);
}

void Renderer::InitDraw() {
   GetInstance().VertexData.clear();
   GetInstance().IndexData.clear();
}

void Renderer::ClearScreenColor(glm::vec4 ScreenColor) {
   glm::vec4 pcColor = (1.0f / 255.0f) * ScreenColor;
   glClearColor(pcColor.r, pcColor.g, pcColor.b, pcColor.a);
}

void Renderer::DrawCircle(float Radius, glm::vec2 Center, glm::vec4 Color) {
   const int VertexCount = 48;

   if ((GetInstance().IndexData.size() + (VertexCount - 2) * 3) * sizeof(GLuint) >=
      GetInstance().EBOMaxSize ||
      (GetInstance().VertexData.size() + VertexCount) * sizeof(Utils::Vertex) >=
      GetInstance().VBOMaxSize) {
      EndDraw();
      Flush();
      InitDraw();
   }

   float Angle = 360.0f / (float)VertexCount;

   // Vertex calculation
   for (int i = 0; i < VertexCount; i++) {
      float CurrAngle = Angle * i;
      Utils::Vertex TempVert;
      TempVert.Position.x =
         ScreenToNDC((Radius * std::cos(glm::radians(CurrAngle))) + Center.x,
                     0.0f)
         .x;
      TempVert.Position.y =
         ScreenToNDC(0.0f,
                     (Radius * std::sin(glm::radians(CurrAngle))) + Center.y)
         .y;
      TempVert.Position.z = 0.0f;
      TempVert.Color = (1.0f / 255.0f) * Color;
      TempVert.TexCoords = {0.0f, 0.0f};
      TempVert.TextureIndex = -1.0f;
      GetInstance().VertexData.push_back(TempVert);
   }

   // Index calculation
   GLuint StartingIndex = GetInstance().VertexData.size() - (GLuint)VertexCount;

   for (GLuint i = 0; i < VertexCount - 2; i++) {
      GetInstance().IndexData.push_back(StartingIndex);
      GetInstance().IndexData.push_back(StartingIndex + i + 1);
      GetInstance().IndexData.push_back(StartingIndex + i + 2);
   }
}

void Renderer::DrawRect(glm::vec2 Dimensions, glm::vec2 Center,
                        glm::vec4 Color) {
   const GLuint VertexCount = 4;

   if ((GetInstance().IndexData.size() + (VertexCount - 2) * 3) * sizeof(GLuint) >=
      GetInstance().EBOMaxSize ||
      (GetInstance().VertexData.size() + VertexCount) * sizeof(Utils::Vertex) >=
      GetInstance().VBOMaxSize) {
      EndDraw();
      Flush();
      InitDraw();
   }

   Utils::Vertex v0;
   glm::vec2 TransformedPos = ScreenToNDC(Center.x, Center.y);
   v0.Position.x = TransformedPos.x;
   v0.Position.y = TransformedPos.y;
   v0.Position.z = 0.0f;
   v0.Color = (1.0f / 255.f) * Color;
   v0.TexCoords = {0.0f, 0.0f};
   v0.TextureIndex = -1.0f;

   Utils::Vertex v1;
   TransformedPos = ScreenToNDC(Center.x + Dimensions.x, Center.y);
   v1.Position.x = TransformedPos.x;
   v1.Position.y = TransformedPos.y;
   v1.Position.z = 0.0f;
   v1.Color = (1.0f / 255.f) * Color;
   v1.TexCoords = {0.0f, 0.0f};
   v1.TextureIndex = -1.0f;

   Utils::Vertex v2;
   TransformedPos =
      ScreenToNDC(Center.x + Dimensions.x, Center.y + Dimensions.y);
   v2.Position.x = TransformedPos.x;
   v2.Position.y = TransformedPos.y;
   v2.Position.z = 0.0f;
   v2.Color = (1.0f / 255.f) * Color;
   v2.TexCoords = {0.0f, 0.0f};
   v2.TextureIndex = -1.0f;

   Utils::Vertex v3;
   TransformedPos = ScreenToNDC(Center.x, Center.y + Dimensions.y);
   v3.Position.x = TransformedPos.x;
   v3.Position.y = TransformedPos.y;
   v3.Position.z = 0.0f;
   v3.Color = (1.0f / 255.f) * Color;
   v3.TexCoords = {0.0f, 0.0f};
   v3.TextureIndex = -1.0f;

   GetInstance().VertexData.push_back(v0);
   GetInstance().VertexData.push_back(v1);
   GetInstance().VertexData.push_back(v2);
   GetInstance().VertexData.push_back(v3);

   GLuint StartingIndex = GetInstance().VertexData.size() - VertexCount;

   GLuint TempIndexArray[] = {StartingIndex,     StartingIndex + 1,
      StartingIndex + 2, StartingIndex,
      StartingIndex + 3, StartingIndex + 2};

   for (GLuint index : TempIndexArray) {
      GetInstance().IndexData.push_back(index);
   }
}

void Renderer::DrawTriangle(glm::vec2 V0, glm::vec2 V1, glm::vec2 V2,
                            glm::vec4 Color) {
   const GLuint VertexCount = 3;

   if ((GetInstance().IndexData.size() + (VertexCount - 2) * 3) * sizeof(GLuint) >=
      GetInstance().EBOMaxSize ||
      (GetInstance().VertexData.size() + VertexCount) * sizeof(Utils::Vertex) >=
      GetInstance().VBOMaxSize) {
      EndDraw();
      Flush();
      InitDraw();
   }

   Utils::Vertex v0;
   glm::vec2 TransformedPos = ScreenToNDC(V0.x, V0.y);
   v0.Position.x = TransformedPos.x;
   v0.Position.y = TransformedPos.y;
   v0.Position.z = 0.0f;
   v0.Color = (1.0f / 255.f) * Color;
   v0.TexCoords = {0.0f, 0.0f};
   v0.TextureIndex = -1.0f;

   Utils::Vertex v1;
   TransformedPos = ScreenToNDC(V1.x, V1.y);
   v1.Position.x = TransformedPos.x;
   v1.Position.y = TransformedPos.y;
   v1.Position.z = 0.0f;
   v1.Color = (1.0f / 255.f) * Color;
   v1.TexCoords = {0.0f, 0.0f};
   v1.TextureIndex = -1.0f;

   Utils::Vertex v2;
   TransformedPos = ScreenToNDC(V2.x, V2.y);
   v2.Position.x = TransformedPos.x;
   v2.Position.y = TransformedPos.y;
   v2.Position.z = 0.0f;
   v2.Color = (1.0f / 255.f) * Color;
   v2.TexCoords = {0.0f, 0.0f};
   v2.TextureIndex = -1.0f;

   GetInstance().VertexData.push_back(v0);
   GetInstance().VertexData.push_back(v1);
   GetInstance().VertexData.push_back(v2);

   GLuint StartingIndex = GetInstance().VertexData.size() - VertexCount;

   GLuint TempIndexArray[] = {StartingIndex, StartingIndex + 1,
      StartingIndex + 2};

   for (GLuint index : TempIndexArray) {
      GetInstance().IndexData.push_back(index);
   }
}

void Renderer::EndDraw() {
   static bool printVertex = false;

   if (printVertex) {
      for (Utils::Vertex vertex : GetInstance().VertexData) {
         std::cout << "Vertex: ";
         std::cout << vertex.Position.x << ", ";
         std::cout << vertex.Position.y << ", ";
         std::cout << vertex.Position.z << ", ";
         std::cout << vertex.Color.r << ", ";
         std::cout << vertex.Color.g << ", ";
         std::cout << vertex.Color.b << ", ";
         std::cout << vertex.Color.a << ", ";
         std::cout << vertex.TexCoords.x << ", ";
         std::cout << vertex.TexCoords.y << ", ";
         std::cout << vertex.TextureIndex << std::endl;
      }

      int count = 0;
      for (GLuint index : GetInstance().IndexData) {
         if (count % 3 == 0)
            std::cout << std::endl;
         std::cout << index << ", ";
         count++;
      }
      printVertex = !printVertex;
   }

   glBindBuffer(GL_ARRAY_BUFFER, GetInstance().VBO);
   glBufferSubData(GL_ARRAY_BUFFER, 0,
                   sizeof(Utils::Vertex) * GetInstance().VertexData.size(),
                   GetInstance().VertexData.data());
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetInstance().EBO);
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0,
                   sizeof(GLuint) * GetInstance().IndexData.size(),
                   GetInstance().IndexData.data());

   GetInstance().Shader->Use();
   GetInstance().Shader->SetMat4("model", glm::mat4(1.0f));
   GetInstance().Shader->SetMat4("view", glm::mat4(1.0f));
   GetInstance().Shader->SetMat4("projection", glm::mat4(1.0f));
   GetInstance().Shader->SetVec4("Tint", glm::vec4(1.0f));
}

void Renderer::Flush() {
   glBindVertexArray(GetInstance().VAO);
   glDrawElements(GL_TRIANGLES, GetInstance().IndexData.size(), GL_UNSIGNED_INT,
                  0);
   BatchData.DrawCalls++;
}

Renderer::~Renderer() {
   delete this->Shader;
   glDeleteVertexArrays(1, &this->VAO);
   glDeleteBuffers(1, &this->VBO);
   glDeleteBuffers(1, &this->EBO);
}

} // namespace Engine
