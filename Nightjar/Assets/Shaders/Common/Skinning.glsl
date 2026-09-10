
#pragma UBO_USAGE Armature MATERIAL
layout (std140) uniform Armature
{
    mat4 bones[BONE_MAX];
#ifndef SHADER_NORMAL
    mat4 bonesNrm[BONE_MAX];
#endif
};

#pragma UBO_USAGE ArmatureBind ASSET
layout (std140) uniform ArmatureBind
{
    mat4 bonesBindInv[BONE_MAX];
};

void CalculateDeform(uvec4 boneIndices, vec4 boneWeights, out mat4 deformPos
#ifndef SHADER_NORMAL
    , out mat4 deformNrm
#endif
    )
{
    deformPos = mat4(0.0);
#ifndef SHADER_NORMAL
    deformNrm = mat4(0.0);
#endif
    for (int i = 0; i < BONES_PER_VERT; ++i)
    {
        uint boneIndex = boneIndices[i];
        float boneWeight = boneWeights[i];
        deformPos += (bones[boneIndex] * bonesBindInv[boneIndex]) * boneWeight;
#ifndef SHADER_NORMAL
        deformNrm += (bonesNrm[boneIndex] * bonesBindInv[boneIndex]) * boneWeight;
#endif
    }
}
