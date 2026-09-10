#version 330 core

#include "Common/Viewport.glsl"

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec4 inColor;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec4 Color;

void main(void) 
{
    vec4 worldPos = world * vec4(inPosition, 1);
    gl_Position = PositionPerspectiveWorld(worldPos);
    Position = worldPos.xyz;
    Normal = transpose(mat3(worldNormal)) * inNormal;
    TexCoord = inTexCoord;
    Color = inColor;
}
