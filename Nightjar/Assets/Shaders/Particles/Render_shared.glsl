
#define VS_OUT _VS_OUT\
{\
	mat4 transform;\
	mat3 transformNormal;\
    int particleIndex;\
}\

#define GS_OUT _GS_OUT\
{\
	vec3 position;\
	vec3 normal;\
	vec2 texCoord;\
    vec4 color;\
    flat int particleIndex;\
}\

struct InstVert
{
    vec3 pos;
    vec3 nrm;
    vec2 tex;
    vec4 col;
};

layout (shared) uniform InstanceMesh
{
    int instVertCount;
    int instIndices[PARTICLE_BATCH_GEOMETRY_VERTS_MAX];
    InstVert insts[PARTICLE_BATCH_GEOMETRY_VERTS_MAX];
    //vec3 instPositions[PARTICLE_BATCH_GEOMETRY_VERTS_MAX];
    //vec3 instNormals[PARTICLE_BATCH_GEOMETRY_VERTS_MAX];
    //vec2 instTexCoords[PARTICLE_BATCH_GEOMETRY_VERTS_MAX];
    //vec4 instColors[PARTICLE_BATCH_GEOMETRY_VERTS_MAX];
};
