#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in float aTexId;

out vec2 TexCoord;
out vec4 VertexColor;
out float TexId;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
   TexCoord = aTexCoord;
   VertexColor = aColor;
   TexId = aTexId;
   gl_Position = projection * view * model * vec4(aPos, 1.0);
}

