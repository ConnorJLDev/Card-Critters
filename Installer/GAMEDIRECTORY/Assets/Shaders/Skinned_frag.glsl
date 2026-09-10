#version 330 core

#include "Common/Lighting.glsl"

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec4 Color;

out vec4 pixelColor;

uniform sampler2D inTexture;

void main(void) 
{
    vec4 texColor = texture(inTexture, TexCoord);
    texColor = vec4(texColor.r); // Single channel texture
    //texColor.a = 0; // Solid white
    texColor = mix(vec4(1), texColor, texColor.a);
    pixelColor = vec4(Color.rgb + texColor.rgb, 1.0);
    pixelColor.rgb *= DiffuseLighting(Position, Normal).rgb;
}
