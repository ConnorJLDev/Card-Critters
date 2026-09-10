#version 330 core

#include "Common/TextSymbols.glsl"
#include "Text_shared.glsl"

in GS_OUT fs_in;

out vec4 pixelColor;

uniform sampler2D inTexture;

void main(void) 
{
    vec2 texData = texture(inTexture, fs_in.texCoord).rg;
    texData.y = min(texData.y * borderStrength, 1.0);

    float alpha = max(texData.x, texData.y);
    if (alpha < 0.01)
        discard;

    vec4 texColor = vec4(vec3(1.0), alpha);
    texColor *= mix(borderColor, textColor, texData.x);

    pixelColor = texColor;
}