#version 150 core

uniform float deltaTime;

uniform sampler2D diffuseTex;
uniform sampler2D depthTex;
uniform sampler2D emissiveTex;
uniform sampler2D specularTex;

in Vertex {
  vec2 texCoord;
  vec3 normal;
} IN;

out vec4 fragColour;

void main(void) {
  if (texture(depthTex, IN.texCoord).r == 1) discard;
  if (texture(diffuseTex, IN.texCoord).r >= 1 &&
      texture(diffuseTex, IN.texCoord).g >= 1 &&
      texture(diffuseTex, IN.texCoord).b >= 1) {
    //fragColour = vec4(cos(deltaTime * 0.001) / 4 + 0.75, cos(deltaTime * 0.002) / 4 + 0.75, cos(deltaTime * 0.003) / 4 + 0.75, 1);
    fragColour = vec4(1, 1, 1, 1);
    return;
  }

  vec3 diffuse = texture(diffuseTex, IN.texCoord).xyz;
  vec3 light = texture(emissiveTex, IN.texCoord).xyz;
  vec3 specular = texture(specularTex, IN.texCoord).xyz;

  fragColour.xyz = diffuse * 0.2;
  fragColour.xyz += diffuse * light;
  fragColour.xyz += specular;
  fragColour.a = 1.0;
}
