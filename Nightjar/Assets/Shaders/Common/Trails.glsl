
#define MAX_SHELL_POINTS 64

struct ShellPoint
{
	vec2 pos;
	vec2 nrm;
};

layout (std140) uniform ShellPoints
{
    ShellPoint shellPoints[MAX_SHELL_POINTS];
    vec2 shellScale;
    int shellPointCount;
    int subdivideCount;
    float subdivideInverse; // = 1.0 / subdivideCount
};
