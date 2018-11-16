#version 150 core

uniform mat4 projMatrix;
uniform mat4 viewMatrix;

in vec3 position;
in vec2 texCoord;
in vec3 normal;

out Vertex {
    vec2 texCoord;
    vec3 normal;
} OUT;

void main(void) {
    gl_Position = projMatrix * vec4(position, 1.0);
    OUT.texCoord = texCoord;

    // vec3 temp = position;
    // temp.y = -temp.y;

    // OUT.normal =
      //transpose(mat3(viewMatrix)) *
      //normalize(temp + vec3(0, 0, 1));
}
