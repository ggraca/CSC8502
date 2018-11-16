#version 150 core

uniform mat4 shadowMatrix;

uniform sampler2D depthTex;
uniform sampler2D normTex;
uniform sampler2DShadow shadowTex;

uniform vec2 pixelSize;
uniform vec3 cameraPos;

uniform float lightRadius;
uniform vec3 lightPos;
uniform vec4 lightColour;

in Vertex {
  mat4 inverseProjView;
} IN;

out vec4 fragColour[2];

void main(void) {
  // Find screen position
  vec3 screen_pos = vec3(
      gl_FragCoord.x * pixelSize.x,
      gl_FragCoord.y * pixelSize.y,
      0.0
  );
  screen_pos.z = texture(depthTex, screen_pos.xy).r;

  // Find world position
  vec4 clip = IN.inverseProjView * vec4(screen_pos * 2.0 - 1.0, 1.0);
  vec3 pos = clip.xyz / clip.w;

  // Get vectors
  vec3 incident = normalize(lightPos - pos);
  vec3 viewDir = normalize(cameraPos - pos);
  vec3 halfDir = normalize(incident + viewDir);
  vec3 normal = normalize(texture(normTex, screen_pos.xy).xyz * 2.0 - 1.0);
  vec4 shadowProj = shadowMatrix * vec4(pos + (normal * 1.5), 1);

  // Get scalars
  float dist = length(lightPos - pos);
  float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);
  if (atten == 0.0) discard;

  float lambert = clamp(dot(incident, normal), 0.0, 1.0);
  if (shadowProj.w > 0.0)
    lambert *= textureProj(shadowTex, shadowProj);

  // Light components params
  float rFactor = clamp(dot(halfDir, normal), 0.0, 1.0);
  float sFactor = pow(rFactor, 33.0);

  // Output
  fragColour[0] = vec4(lightColour.xyz * lambert * atten, 1.0);
  fragColour[1] = vec4(lightColour.xyz * sFactor * atten * 0.33, 1.0);
}
