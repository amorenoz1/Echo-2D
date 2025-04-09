#version 410 core
out vec4 FragColor;

in vec2 TexCoord;
in vec4 VertexColor;
in float TexId;

uniform sampler2D Textures[16];
uniform vec4 Tint;

void main() {
   vec4 TexColor;

   int index = int(TexId);

   if (index == -1) {
      TexColor = vec4(1.0);
   } else {
      TexColor = texture(Textures[index], TexCoord);
   }

   FragColor = VertexColor * TexColor * Tint;
}
