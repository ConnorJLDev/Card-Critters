#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = PARTICLE_BATCH_GEOMETRY_VERTS_MAX) out;

#include "Common/Viewport.glsl"
#include "Particles/Render_shared.glsl"

in VS_OUT gs_in[];

out GS_OUT gs_out;

void main(void) 
{
    for (int i = 0; i < instVertCount; ++i)
    {
        int vertIndex = instIndices[i];
        if (vertIndex < 0)
        {
            EndPrimitive();
            continue;
        }
        vec4 pos = gs_in[0].transform * vec4(insts[vertIndex].pos, 1);
        gl_Position = PositionPerspectiveWorld(pos);
        gs_out.position.xyz = pos.xyz;
        gs_out.normal.xyz = gs_in[0].transformNormal * insts[vertIndex].nrm;
        gs_out.texCoord = insts[vertIndex].tex;
        gs_out.color = insts[vertIndex].col;
        //gs_out.particleIndex = gs_in[0].particleIndex;
        EmitVertex();
    }
}
