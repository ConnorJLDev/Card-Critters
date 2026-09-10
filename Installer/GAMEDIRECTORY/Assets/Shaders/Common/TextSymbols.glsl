
#define MAX_SYMBOLS 192 // ASCII 0x20 - 0x7F for now
struct Symbol
{
	vec2 pos;
	vec2 size;
    vec2 worldSize;
};

layout (std140) uniform Symbols
{
    Symbol symbols[MAX_SYMBOLS];
    int symbolStart;
    int symbolCount;
    vec4 textColor;
};

#define MAX_EFFECTS 4 // 4 for now
uniform float currentEffectTimes[MAX_EFFECTS];
