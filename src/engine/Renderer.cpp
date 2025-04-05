#include "core/core.h"
#include "external/glad.h"
#include "utils/ShaderUtils.h"
#include <iostream>
#include <cmath>
#include <engine/Renderer.h>

namespace Engine {

Renderer::Renderer() {
   this->Shader = new Utils::Shader();
   this->ScreenWidth = 800;
   this->ScreenHeight = 600;
   this->VBOCurrSize = sizeof(Utils::Vertex) * 256;
   this->EBOCurrSize= sizeof(GLuint) * 256;

   glGenVertexArrays(1, &this->VAO);
   glGenBuffers(1, &this->VBO);
   glGenBuffers(1, &this->EBO);

   glBindVertexArray(this->VAO);
   glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
   glBufferData(GL_ARRAY_BUFFER, this->VBOCurrSize , nullptr, GL_DYNAMIC_DRAW);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->EBOCurrSize, nullptr, GL_DYNAMIC_DRAW);

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(9 * sizeof(float)));
   glEnableVertexAttribArray(3);
}

void Renderer::InitDraw(int ScreenWidth, int ScreenHeight) {
   GetInstance().VertexData.clear();
   GetInstance().IndexData.clear();
   GetInstance().ScreenHeight = ScreenHeight;
   GetInstance().ScreenWidth = ScreenWidth;
}

glm::vec2 Renderer::ScreenToNDC(float ScreenX, float ScreenY) {
    float ndcX = (2.0f * ScreenX) / GetInstance().ScreenWidth - 1.0f;
    float ndcY = 1.0f - (2.0f * ScreenY) / GetInstance().ScreenHeight;
    return glm::vec2(ndcX, ndcY);
}

void Renderer::ClearScreenColor(glm::vec4 ScreenColor) {
   glm::vec4 pcColor = (1.0f/255.0f) * ScreenColor;
   glClearColor(pcColor.r, pcColor.g, pcColor.b, pcColor.a);
}

void Renderer::DrawCircle(float Radius, glm::vec2 Center, glm::vec4 Color) {
   const int VertexCount = 32;
   float Angle = 360.0f / (float) VertexCount;

   // Vertex calculation
   for (int i = 0; i < VertexCount; i++) {
      float CurrAngle = Angle * i;
      Utils::Vertex TempVert;
      TempVert.Position.x = GetInstance().ScreenToNDC((Radius * std::cos(glm::radians(CurrAngle))) + Center.x, 0.0f).x;
      TempVert.Position.y = GetInstance().ScreenToNDC(0.0f , (Radius * std::sin(glm::radians(CurrAngle))) + Center.y).y;
      TempVert.Position.z = 0.0f;
      TempVert.Color = (1.0f/255.0f) * Color;
      TempVert.TexCoords = {0.0f, 0.0f};
      TempVert.TextureIndex = -1.0f;
      GetInstance().VertexData.push_back(TempVert);
   }

   // Index calculation
   GLuint StartingIndex = GetInstance().VertexData.size() - (GLuint) VertexCount;

   for (GLuint i = 0; i < VertexCount - 2; i++) {
      GetInstance().IndexData.push_back(StartingIndex);
      GetInstance().IndexData.push_back(StartingIndex + i + 1);
      GetInstance().IndexData.push_back(StartingIndex + i + 2);
   }
}

void Renderer::DrawRect(glm::vec2 Dimensions, glm::vec2 Center, glm::vec4 Color) {
   const GLuint VertexCount = 4;

   Utils::Vertex v0;
   glm::vec2 TransformedPos = GetInstance().ScreenToNDC(Center.x, Center.y);
   v0.Position.x = TransformedPos.x;
   v0.Position.y = TransformedPos.y;
   v0.Position.z = 0.0f;
   v0.Color = (1.0f/255.f) * Color;
   v0.TexCoords = {0.0f, 0.0f};
   v0.TextureIndex = -1.0f;

   Utils::Vertex v1;
   TransformedPos = GetInstance().ScreenToNDC(Center.x + Dimensions.x, Center.y);
   v1.Position.x = TransformedPos.x;
   v1.Position.y = TransformedPos.y;
   v1.Position.z = 0.0f;
   v1.Color = (1.0f/255.f) * Color;
   v1.TexCoords = {0.0f, 0.0f};
   v1.TextureIndex = -1.0f;

   Utils::Vertex v2;
   TransformedPos = GetInstance().ScreenToNDC(Center.x + Dimensions.x, Center.y + Dimensions.y);
   v2.Position.x = TransformedPos.x;
   v2.Position.y = TransformedPos.y;
   v2.Position.z = 0.0f;
   v2.Color = (1.0f/255.f) * Color;
   v2.TexCoords = {0.0f, 0.0f};
   v2.TextureIndex = -1.0f;

   Utils::Vertex v3;
   TransformedPos = GetInstance().ScreenToNDC(Center.x , Center.y + Dimensions.y);
   v3.Position.x = TransformedPos.x;
   v3.Position.y = TransformedPos.y;
   v3.Position.z = 0.0f;
   v3.Color = (1.0f/255.f) * Color;
   v3.TexCoords = {0.0f, 0.0f};
   v3.TextureIndex = -1.0f;

   GetInstance().VertexData.push_back(v0);
   GetInstance().VertexData.push_back(v1);
   GetInstance().VertexData.push_back(v2);
   GetInstance().VertexData.push_back(v3);

   GLuint StartingIndex = GetInstance().VertexData.size() -  VertexCount;
   
   GLuint TempIndexArray[] = {
      StartingIndex, StartingIndex + 1, StartingIndex + 2,
      StartingIndex, StartingIndex + 3, StartingIndex + 2
   };

   for (GLuint index : TempIndexArray) {
      GetInstance().IndexData.push_back(index);
   }

}

void Renderer::DrawTriangle(glm::vec2 V0, glm::vec2 V1, glm::vec2 V2, glm::vec4 Color) {
   const GLuint VertexCount = 3;

   Utils::Vertex v0;
   glm::vec2 TransformedPos = GetInstance().ScreenToNDC(V0.x, V0.y);
   v0.Position.x = TransformedPos.x;
   v0.Position.y = TransformedPos.y;
   v0.Position.z = 0.0f;
   v0.Color = (1.0f/255.f) * Color;
   v0.TexCoords = {0.0f, 0.0f};
   v0.TextureIndex = -1.0f;

   Utils::Vertex v1;
   TransformedPos = GetInstance().ScreenToNDC(V1.x, V1.y);
   v1.Position.x = TransformedPos.x;
   v1.Position.y = TransformedPos.y;
   v1.Position.z = 0.0f;
   v1.Color = (1.0f/255.f) * Color;
   v1.TexCoords = {0.0f, 0.0f};
   v1.TextureIndex = -1.0f;

   Utils::Vertex v2;
   TransformedPos = GetInstance().ScreenToNDC(V2.x, V2.y);
   v2.Position.x = TransformedPos.x;
   v2.Position.y = TransformedPos.y;
   v2.Position.z = 0.0f;
   v2.Color = (1.0f/255.f) * Color;
   v2.TexCoords = {0.0f, 0.0f};
   v2.TextureIndex = -1.0f;

   GetInstance().VertexData.push_back(v0);
   GetInstance().VertexData.push_back(v1);
   GetInstance().VertexData.push_back(v2);

   GLuint StartingIndex = GetInstance().VertexData.size() - VertexCount;

   GLuint TempIndexArray[] = {
      StartingIndex, StartingIndex + 1, StartingIndex + 2
   };

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
         if (count % 3 == 0) std::cout << std::endl;
         std::cout << index << ", ";
         count++;
      }
      printVertex = !printVertex;
   }

   CheckAndExpandBuffers();

   glBindBuffer(GL_ARRAY_BUFFER, GetInstance().VBO);
   glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Utils::Vertex) * GetInstance().VertexData.size(), GetInstance().VertexData.data());
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetInstance().EBO);
   glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(GLuint) * GetInstance().IndexData.size(), GetInstance().IndexData.data());

   GetInstance().Shader->Use();
   GetInstance().Shader->SetMat4("model", glm::mat4(1.0f));
   GetInstance().Shader->SetMat4("view", glm::mat4(1.0f));
   GetInstance().Shader->SetMat4("projection", glm::mat4(1.0f));
   GetInstance().Shader->SetVec4("Tint", glm::vec4(1.0f));


   glBindVertexArray(GetInstance().VAO);
   glDrawElements(GL_TRIANGLES, GetInstance().IndexData.size(), GL_UNSIGNED_INT, 0);
}

void Renderer::CheckAndExpandBuffers () {
   const GLuint VertexBufferSize = sizeof(Utils::Vertex) * GetInstance().VertexData.size();
   const GLuint IndexBufferSize = sizeof(GLuint) * GetInstance().IndexData.size();
   const GLuint VertexMargin = sizeof(Utils::Vertex) * 4;
   const GLuint IndexMargin = sizeof(GLuint) * 4;

   if (VertexBufferSize + VertexMargin >= GetInstance().VBOCurrSize) {
      GLuint NewSize = VertexBufferSize * 2;
      
      // Delete old buffer
      glDeleteBuffers(1, &GetInstance().VBO);

      // Create new buffer
      glBindVertexArray(GetInstance().VAO);
      glGenBuffers(1, &GetInstance().VBO);
      glBindBuffer(GL_ARRAY_BUFFER, GetInstance().VBO);
      glBufferData(GL_ARRAY_BUFFER, NewSize, nullptr, GL_DYNAMIC_DRAW);
      
      GetInstance().VBOCurrSize = NewSize;
      std::cout << "Expanded VBO to: " << NewSize << "bytes" << std::endl;
   }

   if (IndexBufferSize + IndexMargin>= GetInstance().EBOCurrSize) {
      GLuint NewSize = IndexBufferSize * 2;

      glDeleteBuffers(1, &GetInstance().EBO);

      // Create new buffer
      glBindVertexArray(GetInstance().VAO);
      glGenBuffers(1, &GetInstance().EBO);
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetInstance().EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, NewSize, nullptr, GL_DYNAMIC_DRAW);


      GetInstance().EBOCurrSize = NewSize;
      std::cout << "Expanded EBO to: " << NewSize << "bytes" << std::endl;
   }

   glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);
   glEnableVertexAttribArray(0);
   glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));
   glEnableVertexAttribArray(1);
   glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(7 * sizeof(float)));
   glEnableVertexAttribArray(2);
   glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(9 * sizeof(float)));
   glEnableVertexAttribArray(3);
}
Renderer::~Renderer() { 
   delete this->Shader; 
   glDeleteVertexArrays(1, &this->VAO);
   glDeleteBuffers(1, &this->VBO);
   glDeleteBuffers(1, &this->EBO);
}

} // namespace Engine
