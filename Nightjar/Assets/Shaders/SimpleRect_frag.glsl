#version 330 core

#include "Simple_shared.glsl"

out vec4 pixelColor;

in vec2 TexCoord;

void main(void) 
{
    // Find how deep we are into the object for calculating border and clipping.
    vec2 vecDepth = abs(TexCoord);
    float objDepth = max(vecDepth.x, vecDepth.y);
    pixelColor = objDepth >= borderSize ? borderColor : fillColor;
}
