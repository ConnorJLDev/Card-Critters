
#pragma UBO_USAGE Viewport GLOBAL
layout (shared) uniform Viewport
{
    mat4 projectionOrtho;
    mat4 projectionPersp;
    mat4 viewOrtho;
    mat4 viewPersp;
    uvec2 resolution;
};

#pragma UBO_USAGE Time GLOBAL
layout (std140) uniform Time
{
    float dt;
};

uniform mat4 world;
uniform mat4 worldNormal;

vec4 PositionOrthographicWorld(vec4 posWorld)
{
    return projectionOrtho * viewOrtho * posWorld;
}

vec4 PositionOrthographic(vec4 pos)
{
    return PositionOrthographicWorld(world * pos);
}

vec4 PositionOrthographic(vec3 pos)
{
    return PositionOrthographic(vec4(pos,1));
}

vec4 PositionOrthographic(vec2 pos)
{
    return PositionOrthographic(vec4(pos,0,1));
}

vec4 PositionPerspectiveWorld(vec4 posWorld)
{
    return projectionPersp * viewPersp * posWorld;
}

vec4 PositionPerspective(vec4 pos)
{
    return PositionPerspectiveWorld(world * pos);
}

vec4 PositionPerspective(vec3 pos)
{
    return PositionPerspective(vec4(pos, 1));
}
