
#ifndef SHADERUTILS_H
#define SHADERUTILS_H

#include "core/core.h"
#include <glm/glm.hpp>
#include <string>

namespace Utils {

struct Vertex {
   glm::vec3 Position;
   glm::vec4 Color;
   glm::vec2 TexCoords;
   float TextureIndex;
};

class Shader {
public:
   Shader (const char* VertexPath = "shaders/Vert.glsl", const char* FragmentPath = "shaders/Frag.glsl");
   ~Shader ();

   void Use();
   void SetMat4(const std::string Name, const glm::mat4 &Value) const;
   void SetVec4(const std::string Name, const glm::vec4 &Value) const;
   void SetBool(const std::string Name, bool Value) const;
   void SetInt(const std::string Name, int Value) const;
   void SetIntV(const std::string Name, GLuint Size, int *Value) const;
   void SetFloat(const std::string Name, float Value) const;
   GLuint GetID();

private:
   GLuint ID;

   void CheckCompileErrors(GLuint Shader, std::string Type);
};

}

#endif // !SHADERUTILS_H
