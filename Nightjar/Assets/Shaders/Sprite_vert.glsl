#version 330 core

#include "Common/Viewport.glsl"
#include "Sprite_shared.glsl"

layout (location = 0) in vec2 inPosition;
layout (location = 1) in vec2 inTexCoord;

out vec2 TexCoord;

void main(void) 
{
    gl_Position = PositionOrthographic(vec3(inPosition, zLayer));
    TexCoord = inTexCoord * texCoordScale + texCoordOffset;
}
