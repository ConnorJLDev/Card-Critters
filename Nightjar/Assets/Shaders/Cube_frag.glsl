#version 330 core

#include "Common/Lighting.glsl"

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec4 Color;

out vec4 pixelColor;

layout (std140) uniform FX
{
    vec4 colorTint;
    vec4 colorAdd;
};

//uniform sampler2D inTexture;

void main(void) 
{
    //vec4 texColor = texture(inTexture, TexCoord);
    //texColor = vec4(texColor.r);
    //texColor = mix(vec4(1), texColor, texColor.a);
    pixelColor = Color; //vec4(Color.rgb + texColor.rgb, 1.0);
    pixelColor.rgb *= DiffuseLighting(Position, Normal).rgb;
    pixelColor *= colorTint;
    pixelColor += colorAdd;
}
