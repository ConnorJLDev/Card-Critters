#version 330 core

#include "Common/Viewport.glsl"
#include "Common/Trails.glsl"
#include "Trails_shared.glsl"

layout (location = 0) in vec3 inPosition;     // 3D Position of the trail segment.
layout (location = 1) in float inTrailFront;  // How close to the front this segment is (normalized).
layout (location = 2) in vec4 inRotation;     // Quaternion rotation of the trail segment.

out VS_OUT vs_out;

void main(void) 
{
    gl_Position = PositionPerspective(inPosition);
    vs_out.pos = inPosition;
    vs_out.trailFront = inTrailFront;
    vs_out.rot = inRotation;
}
