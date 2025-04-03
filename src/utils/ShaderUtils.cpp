#include "core/core.h"
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <sstream>
#include <string>
#include <utils/ShaderUtils.h>
namespace Utils {

Shader::Shader(const char *VertexPath, const char *FragmentPath) {
   // 1. retrieve the vertex/fragment source code from filePath
   std::string vertexCode;
   std::string fragmentCode;
   std::ifstream vShaderFile;
   std::ifstream fShaderFile;
   // ensure ifstream objects can throw exceptions:
   vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
   try {
      // open files
      vShaderFile.open(VertexPath);
      fShaderFile.open(FragmentPath);
      std::stringstream vShaderStream, fShaderStream;
      // read file's buffer contents into streams
      vShaderStream << vShaderFile.rdbuf();
      fShaderStream << fShaderFile.rdbuf();
      // close file handlers
      vShaderFile.close();
      fShaderFile.close();
      // convert stream into string
      vertexCode = vShaderStream.str();
      fragmentCode = fShaderStream.str();
   } catch (std::ifstream::failure &e) {
      std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what()
         << std::endl;
   }
   const char *vShaderCode = vertexCode.c_str();
   const char *fShaderCode = fragmentCode.c_str();
   // 2. compile shaders
   unsigned int vertex, fragment;
   // vertex shader
   vertex = glCreateShader(GL_VERTEX_SHADER);
   glShaderSource(vertex, 1, &vShaderCode, NULL);
   glCompileShader(vertex);
   CheckCompileErrors(vertex, "VERTEX");
   // fragment Shader
   fragment = glCreateShader(GL_FRAGMENT_SHADER);
   glShaderSource(fragment, 1, &fShaderCode, NULL);
   glCompileShader(fragment);
   CheckCompileErrors(fragment, "FRAGMENT");
   // shader Program
   ID = glCreateProgram();
   glAttachShader(ID, vertex);
   glAttachShader(ID, fragment);
   glLinkProgram(ID);
   CheckCompileErrors(ID, "PROGRAM");
   // delete the shaders as they're linked into our program now and no longer
   // necessary
   glDeleteShader(vertex);
   glDeleteShader(fragment);
}

Shader::~Shader() {

}

void Shader::CheckCompileErrors(GLuint Shader, std::string Type) {
   int success;
   char infoLog[1024];
   if (Type != "PROGRAM") {
      glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
      if (!success) {
         glGetShaderInfoLog(Shader, 1024, NULL, infoLog);
         std::cout
            << "ERROR::SHADER_COMPILATION_ERROR of type: " << Type << "\n"
            << infoLog
            << "\n -- --------------------------------------------------- -- "
            << std::endl;
      }
   } else {
      glGetProgramiv(Shader, GL_LINK_STATUS, &success);
      if (!success) {
         glGetProgramInfoLog(Shader, 1024, NULL, infoLog);
         std::cout
            << "ERROR::PROGRAM_LINKING_ERROR of type: " << Type << "\n"
            << infoLog
            << "\n -- --------------------------------------------------- -- "
            << std::endl;
      }
   }
}

void Shader::Use() { glUseProgram(this->ID); }

void Shader::SetMat4(const std::string Name, const glm::mat4 &Value) const {
   int Loc = glGetUniformLocation(this->ID, Name.c_str());
   glUniformMatrix4fv(Loc, 1, GL_FALSE, glm::value_ptr(Value));
}

void Shader::SetVec4(const std::string Name, const glm::vec4 &Value) const {
   int Loc = glGetUniformLocation(this->ID, Name.c_str());
   glUniform4fv(Loc, 1, glm::value_ptr(Value));
   
}

void Shader::SetBool(const std::string Name, bool Value) const {
   int Loc = glGetUniformLocation(this->ID, Name.c_str());
   glUniform1i(Loc, (int)Value);
}

void Shader::SetInt(const std::string Name, int Value) const {
   int Loc = glGetUniformLocation(this->ID, Name.c_str());
   glUniform1i(Loc, Value);
}

void Shader::SetFloat(const std::string Name, float Value) const {
   int Loc = glGetUniformLocation(this->ID, Name.c_str());
   glUniform1f(Loc, Value);
}

GLuint Shader::GetID() { return this->ID; }

} // namespace Utils
