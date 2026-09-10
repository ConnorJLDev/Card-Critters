#version 330 core

#include "Common/TextSymbols.glsl"
#include "Text_shared.glsl"

in GS_OUT fs_in;

out vec4 pixelColor;

uniform sampler2D inTexture;

void main(void) 
{
    vec4 texColor = vec4(texture(inTexture, fs_in.texCoord).r);
    if (texColor.a < 0.01)
        discard;

    // Replace text color with a gray tone
	texColor.rgb = vec3(1.0);  

    pixelColor = texColor;
}