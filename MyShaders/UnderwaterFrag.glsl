#version 150

uniform sampler2D diffuseTex;
uniform float cameraHeight;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void) {
	gl_FragColor = texture(diffuseTex, IN.texCoord);
	if (cameraHeight < 54) gl_FragColor += vec4(0, 0.2, 0.3, 0.2);
}
