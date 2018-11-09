#version 150
uniform sampler2D diffuseTex;

in Vertex {
	vec3 position;
	vec4 colour;
	vec2 texCoord;
} IN;

out vec4 gl_FragColor;

void main(void) {
	if(IN.position.y > 350) gl_FragColor = vec4(1, 1, 1, 1);
	else gl_FragColor = texture(diffuseTex, IN.texCoord);
}
