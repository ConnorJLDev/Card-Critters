#version 330 core

#include "Common/Viewport.glsl"
#include "Common/TextSymbols.glsl"
#include "Text_shared.glsl"

layout (location = 0) in vec3 inPosition;           // Symbol position in 3D
//layout (location = 1) in vec3 inRotation;           // Symbol rotation in 3D
//layout (location = 2) in mat2 inMtx;                // Symbol transform in 2D (for things like shear and scale)
layout (location = 1) in int inSymbol;              // Symbol Index
//layout (location = 4) in ivec4 inEffectIndices;     // Effect Indices (multiple effects per symbol, separate effects on symbols in same string)
//layout (location = 5) in vec4 inEffectTimes;        // Effect Times (for things like scrolling or revealing effects)

out VS_OUT vs_out;

void main(void) 
{
    int symbolIndex = inSymbol - symbolStart;
    gl_Position = PositionPerspective(inPosition);
    gl_ClipDistance[0] = symbolIndex;
    Symbol symbol = symbols[symbolIndex];
    vs_out.pos = inPosition;
    vs_out.texCoordOffset = symbol.pos;
    vs_out.texCoordSize = symbol.size;
    vs_out.worldSize = symbol.worldSize;
}
