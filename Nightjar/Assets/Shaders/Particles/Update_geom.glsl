#version 330 core

layout (points) in;
layout (points, max_vertices = 1) out;

#include "Common/Viewport.glsl"

in vec4 geomPosition[];
in vec4 geomRotation[];
in vec4 geomScale_geomDrag[];
in vec4 geomVelocity_geomDragRotational[];
in vec4 geomAcceleration_geomLifeTime[];
in vec4 geomRotationalVelocity[];

#pragma UBO_OUTPUT outPosition
#pragma UBO_OUTPUT outRotation
#pragma UBO_OUTPUT outScale_outDrag
#pragma UBO_OUTPUT outVelocity_outDragRotational
#pragma UBO_OUTPUT outAcceleration_outLifeTime
#pragma UBO_OUTPUT outRotationalVelocity
out vec4 outPosition;
out vec4 outRotation;
out vec4 outScale_outDrag;
out vec4 outVelocity_outDragRotational;
out vec4 outAcceleration_outLifeTime;
out vec4 outRotationalVelocity;

void main(void) 
{
    // The geometry shader is only used for removing expired particles.
    if (gl_in[0].gl_ClipDistance[0] < 0)
    {
        return;
    }
    // All values are passed through from the vertex shader.
    outPosition = geomPosition[0];
    outRotation = geomRotation[0];
    outScale_outDrag = geomScale_geomDrag[0];
    outVelocity_outDragRotational = geomVelocity_geomDragRotational[0];
    outAcceleration_outLifeTime = geomAcceleration_geomLifeTime[0];
    outRotationalVelocity = geomRotationalVelocity[0];
    EmitVertex();
}
