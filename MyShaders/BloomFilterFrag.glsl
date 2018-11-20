#version 150

uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void) {
	vec4 colour = texture(diffuseTex, IN.texCoord);

	float brightness = dot(colour.rgb, vec3(0.55, 0.55, 0.55));
  if(brightness > 1.0)
      gl_FragColor = vec4(colour.rgb, 1.0);
  else
      gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}
