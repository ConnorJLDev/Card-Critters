#version 330 core

#include "Common/Viewport.glsl"
#include "Common/Transform.glsl"
#include "Particles/Render_shared.glsl"
#include "Particles/Shared_vert.glsl"

out VS_OUT vs_out;

void main(void) 
{
    //gl_Position = inPosition;
    //gl_ClipDistance[0] = inLifeTime;
    vs_out.transform = world * TranslationMatrix(inPosition.xyz) * mat4(RotationMatrixEulerXYZ(inRotation.xyz)) * mat4(ScaleMatrix(inScale));
    //vs_out.transform = world * (transpose(TranslationMatrix(inPosition.xyz)) * mat4(ScaleMatrix(inScale)));
    //vs_out.transform = TranslationMatrix(inPosition.xyz) * world;
    //vs_out.transform = world;
    vs_out.transformNormal = transpose(inverse(mat3(vs_out.transform)));
    vs_out.particleIndex = gl_VertexID;
}
