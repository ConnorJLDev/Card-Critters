#version 330 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

#include "Common/Viewport.glsl"
#include "Text_shared.glsl"

in VS_OUT gs_in[];

out GS_OUT gs_out;

void DrawSymbol(vec4 pos, vec2 size)
{
	vec2 uvSize = gs_in[0].texCoordSize;
	vec2 uvBase = gs_in[0].texCoordOffset;
	for (int i = 0; i < 4; ++i)
	{
		vec2 cornerUV = vec2(i % 2, i / 2);
		vec2 cornerPos = size * (cornerUV - vec2(0.5));
		//vec4 offset = world * vec4(size * vec2(-0.5,-0.5) * scale,0,0);
		//gl_Position = pos + offset;
		//gl_Position = PositionOrthographic(vec3(pos.xy + cornerPos, pos.z + 1));
		gl_Position = PositionOrthographic(vec3(pos.xy + cornerPos, zLayer));
		gs_out.texCoord = uvBase + uvSize * cornerUV;
		EmitVertex();
	}
	EndPrimitive();
}

void main(void) 
{
	if (gl_in[0].gl_ClipDistance[0] < 0)
	{
		return;
	}
	//vec4 basePos = gl_in[0].gl_Position;
	//vec2 screenScale = basePos.w / resolution.xy;
	//vec2 size = gs_in[0].worldSize * screenScale;
	vec2 size = gs_in[0].worldSize * 0.0075;
	DrawSymbol(vec4(gs_in[0].pos,1), size);
}
