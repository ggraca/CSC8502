#version 150

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;

in vec3 position;
in vec4 colour;
in vec2 texCoord;
in vec3 normal;
in vec3 tangent;

out Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
  vec3 binormal;
	vec3 worldPos;
} OUT;

void main(void) {
	mat3 normalMatrix = transpose(inverse(mat3(modelMatrix)));

	OUT.colour = colour;
	OUT.texCoord = (textureMatrix * vec4(texCoord, 0.0, 1.0)).xy;

  OUT.normal = normalize(normalMatrix * normalize(normal));
	OUT.tangent = normalize(normalMatrix * normalize(tangent));
	OUT.binormal = normalize(normalMatrix * normalize(cross(normal, tangent)));
	OUT.worldPos = (modelMatrix * vec4(position, 1)).xyz;

	mat4 mvp = projMatrix * viewMatrix * modelMatrix;
	gl_Position = mvp * vec4(position, 1.0);
}
