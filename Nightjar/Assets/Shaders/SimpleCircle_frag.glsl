#version 330 core

#include "Simple_shared.glsl"

out vec4 pixelColor;

in vec2 TexCoord;

void main(void) 
{
    // Find how deep we are into the object for calculating border and clipping.
    float objDepth = length(TexCoord);
    if (objDepth > 1)
    {
        discard;
    }
    pixelColor = objDepth >= borderSize ? borderColor : fillColor;
}
