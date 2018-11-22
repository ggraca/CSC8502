#version 330 core
layout (triangles) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform float deltaTime;

void main() {
  mat4 mvp = projMatrix * viewMatrix * modelMatrix;

  vec3 P0 = gl_in[0].gl_Position.xyz;
  vec3 P1 = gl_in[1].gl_Position.xyz;
  vec3 P2 = gl_in[2].gl_Position.xyz;

  vec3 V0 = P0 - P1;
  vec3 V1 = P2 - P1;

  vec3 N = cross(V1, V0);
  N = normalize(N);

  // Center of the triangle
  vec3 P = (P0 + P1 + P2) / 3.0;


  gl_Position = mvp * vec4(P, 1.0);

  if (N.y < 1) return;
  if (P.y < 56 || P.y > 58) return;
  EmitVertex();

  float tval = deltaTime / 1000;
  vec3 NP = P;
  NP.x += sin(tval) / 3 + cos(tval / 2) / 3;
  NP.z += cos(tval) / 3 + sin(tval / 2) / 3;

  gl_Position = mvp * vec4(NP + N, 1.0);
  EmitVertex();

  EndPrimitive();
}
