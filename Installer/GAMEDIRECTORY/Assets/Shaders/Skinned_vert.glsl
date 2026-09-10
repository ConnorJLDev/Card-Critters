#version 330 core

#include "Common/Viewport.glsl"
#include "Common/Skinning.glsl"

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inTexCoord;
layout (location = 3) in vec4 inColor;
layout (location = 4) in uvec4 inBoneIndices;
layout (location = 5) in vec4 inBoneWeights;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;
out vec4 Color;

void main(void) 
{
    mat4 deformPos;
#ifdef SHADER_NORMAL
    CalculateDeform(inBoneIndices, inBoneWeights, deformPos);
#else
    mat4 deformNrm;
    CalculateDeform(inBoneIndices, inBoneWeights, deformPos, deformNrm);
#endif
    mat4 deformFinal = world * deformPos;
    vec4 pos = deformFinal * vec4(inPosition, 1);
    gl_Position = PositionPerspectiveWorld(pos);
    Position = pos.xyz;
#ifdef SHADER_NORMAL
    Normal = mat3(transpose(inverse(deformFinal))) * inNormal;
#else
    Normal = transpose(mat3(deformNrm * worldNormal)) * inNormal;
#endif
    TexCoord = inTexCoord;
    Color = inColor;
}
