#version 330 core

#include "Sprite_shared.glsl"

in vec2 TexCoord;

out vec4 pixelColor;

uniform sampler2D inTexture;

void main(void) 
{
    vec4 texColor = texture(inTexture, TexCoord);
    if (texColor.a < alphaClip)
    {
        discard;
    }
    pixelColor = texColor * colorTint;
}
