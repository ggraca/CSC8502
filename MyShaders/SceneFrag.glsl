#version 150

uniform sampler2D diffuseTex;
uniform sampler2D bumpTex;

uniform vec3 cameraPos;
uniform vec4 lightColour;
uniform vec3 lightPos;
uniform float lightRadius;

in Vertex {
	vec4 colour;
	vec2 texCoord;
	vec3 normal;
	vec3 tangent;
	vec3 binormal;
	vec3 worldPos;
} IN;

out vec4 fragColour;

void main(void) {
	// Utils
	mat3 TBN = mat3(IN.tangent, IN.binormal, IN.normal);

	// Get vectors
	vec3 normal = normalize(TBN * (texture(bumpTex, IN.texCoord).rgb * 2 - 1));
	vec3 incident = normalize(lightPos - IN.worldPos);
	vec3 viewDir = normalize(cameraPos - IN.worldPos);
	vec3 halfDir = normalize(incident + viewDir);

	// Get scalars
	float dist = length(lightPos - IN.worldPos);
	float lambert = clamp(dot(incident, normal), 0.0, 1.0);
	float atten = 1.0 - clamp(dist / lightRadius, 0.0, 1.0);

	// Light components params
	float rFactor = max(0.5, dot(halfDir, normal));
	float sFactor = pow(rFactor, 33.0);

	// Colour operations
	vec4 original_colour = texture(diffuseTex, IN.texCoord);
	vec3 colour =
		(original_colour.rgb * lightColour.rgb) +
		(lightColour.rgb * sFactor * 0.33);

	fragColour = vec4(colour * atten * lambert, original_colour.a);
	fragColour.rgb += original_colour.rgb * lightColour.rgb * 0.1;
}
