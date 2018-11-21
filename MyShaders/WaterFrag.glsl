#version 150 core

uniform sampler2D diffuseTex;
uniform samplerCube cubeTex;

uniform vec3 cameraPos;

in Vertex {
  vec4 colour;
  vec3 normal;
  vec2 texCoord;
  vec3 worldPos;
} IN;

out vec4 fragColour[2];

void main(void) {
  vec4 diffuse = texture(diffuseTex, IN.texCoord) * 0.4;
  vec3 incident = normalize(IN.worldPos - cameraPos);
  vec4 reflection = texture(cubeTex, reflect(incident, normalize(IN.normal)));

  fragColour[0] = diffuse + reflection;
	fragColour[1] = vec4(IN.normal.xyz * 0.5 + 0.5, 0.5);
}
