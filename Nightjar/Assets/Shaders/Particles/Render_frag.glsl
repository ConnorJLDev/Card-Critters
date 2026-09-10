#version 330 core

#include "Common/Lighting.glsl"
#include "Particles/Render_shared.glsl"

in GS_OUT fs_in;

out vec4 pixelColor;

//uniform sampler2D inTexture;

layout (std140) uniform FX
{
    vec4 colorTint;
    vec4 colorAdd;
};

void main(void) 
{
    pixelColor = fs_in.color;
    pixelColor.rgb *= DiffuseLighting(fs_in.position.xyz, fs_in.normal.xyz).rgb;
    pixelColor *= colorTint;
    pixelColor += colorAdd;
    //pixelColor = vec4(1,0,1,1);
}
