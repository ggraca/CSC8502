#version 150 core

out vec4 fragColour[2];

void main(void) {
  fragColour[0] = vec4(0, 0.3, 0.05, 1);
  fragColour[1] = vec4(0, 1, 0, 1);
}
