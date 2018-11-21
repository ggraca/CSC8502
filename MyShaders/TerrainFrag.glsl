#version 150 core

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;
uniform sampler2D terrainTex[5];

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
	vec4 shadowProj;
} IN;

out vec4 fragColour[2];

vec4 chooseTex(float y) {

	// Mountain, Sand
	int size = 5;
	int bots[5] = int[](210, 70, 55, 40, 0);
	int tops[5] = int[](256, 230, 200, 60, 45);
	float ammounts[5];

	for(int i = 0; i < size; i++) {
		int current = i;
		int next = i + 1;

		if (i == 0 && y > tops[next]) {
			ammounts[current] = 1;
			break;
		}
		else if (i < size - 1 && y <= tops[next] && y > bots[current]) {
			float val = (tops[next] - y) /  (tops[next] - bots[current]);
			ammounts[current] = 1 - val;
			ammounts[next] = val;
			break;
		} else if (y <= tops[current] && y > bots[current]) {
			ammounts[current] = 1;
			break;
		}
	}

	return
		ammounts[0] * texture2D(terrainTex[0], IN.texCoord) +
		ammounts[1] * texture2D(terrainTex[1], IN.texCoord) +
		ammounts[2] * texture2D(terrainTex[2], IN.texCoord) +
		ammounts[3] * texture2D(terrainTex[3], IN.texCoord) +
		ammounts[4] * texture2D(terrainTex[4], IN.texCoord);
}

void main(void) {
	// Utils
	mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);

	// Get vectors
	vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2 - 1));

	fragColour[0] = chooseTex(IN.worldPos.y);
	fragColour[1] = vec4(normal.xyz * 0.5 + 0.5, 1.0);
}
