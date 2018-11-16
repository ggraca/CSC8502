#version 150 core

uniform sampler2D diffuseTex;

in Vertex {
  vec2 texCoord;
} IN;

out vec4 fragColour;

void main(void) {
  fragColour = texture2D(diffuseTex, IN.texCoord);
}
