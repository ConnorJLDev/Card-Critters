
#define VS_OUT _VS_OUT\
{\
	vec3 pos;\
	float trailFront;\
	vec4 rot;\
}

#define GS_OUT _GS_OUT\
{\
	float trailFront;\
    float trailTwist;\
    vec2 trailShellCrossSectionPos;\
}

layout (std140) uniform TextSettings
{
    vec4 textColor;
    vec4 borderColor;
	float borderStrength;
    float alphaClip;
    float zLayer;
};
