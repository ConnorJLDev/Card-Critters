#version 330 core

layout (lines) in;
layout (triangle_strip, max_vertices = 128) out;

#include "Common/Viewport.glsl"
#include "Common/Trails.glsl"
#include "Trails_shared.glsl"

in VS_OUT gs_in[];

out GS_OUT gs_out;

void DrawShell(int shellPointIndex, _VS_OUT shellDepth)
{
	ShellPoint shellPoint = shellPoints[shellPointIndex];
    // TODO: Change to mat4 transformation and set output vertex variables.
	EmitVertex();
}

void main(void) 
{
    for (int i = 0; i < shellPointCount; ++i)
    {
        DrawShell(i, gl_in[0]);
        DrawShell(i, gl_in[1]);
    }
	EndPrimitive();
}
