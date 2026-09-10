
#define VS_OUT _VS_OUT\
{\
	vec2 texCoordOffset;\
	vec2 texCoordSize;\
	vec2 worldSize;\
	vec3 pos;\
}

#define GS_OUT _GS_OUT\
{\
	vec2 texCoord;\
}

layout (std140) uniform TextSettings
{
    vec4 textColor;
    vec4 borderColor;
	float borderStrength;
    float alphaClip;
    float zLayer;
};
