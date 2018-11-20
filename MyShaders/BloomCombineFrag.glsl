#version 150

uniform sampler2D diffuseTex;
uniform sampler2D bloomTex;

in Vertex {
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void) {
	gl_FragColor =
		texture(diffuseTex, IN.texCoord) +
		texture(bloomTex, IN.texCoord);
}
