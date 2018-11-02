#version 150 core

in Vertex {
    vec4 colour;
} IN;

out vec4 fragColour;

float threshold = 0.0f;
bool similar(float a, float b) {
    return (abs(b - a) <= threshold);
}

bool greater(float a, float b, float c) {
    return (a - threshold) > b && (a - threshold) > c;

}

void main(void) {
    if(similar(IN.colour.r, IN.colour.g) && similar(IN.colour.g, IN.colour.b) && similar(IN.colour.b, IN.colour.r))
        fragColour = vec4(1, 1, 1, 1);
    else if(greater(IN.colour.r, IN.colour.g, IN.colour.b))
        fragColour = vec4(1, 0, 0, 1);
    else if(greater(IN.colour.g, IN.colour.b, IN.colour.r))
        fragColour = vec4(0, 1, 0, 1);
    else if(greater(IN.colour.b, IN.colour.r, IN.colour.g))
        fragColour = vec4(0, 0, 1, 1);
    else
        fragColour = IN.colour;
}
