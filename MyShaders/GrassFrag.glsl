#version 150 core

uniform int bloom;

out vec4 fragColour[2];

void main(void) {
  if (bloom == 1) fragColour[0] = vec4(1, 1, 1, 1);
  else fragColour[0] = vec4(0, 0.3, 0.05, 1);
  fragColour[1] = vec4(0, 1, 0, 1);
}
