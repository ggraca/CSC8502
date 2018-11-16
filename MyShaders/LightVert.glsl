#version 150 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec3 normal;

out Vertex {
  mat4 inverseProjView;
} OUT;

void main(void) {
    OUT.inverseProjView = inverse(projMatrix * viewMatrix);
    
    gl_Position = (projMatrix * viewMatrix * modelMatrix) * vec4(position, 1.0);
}
