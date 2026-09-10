#version 330 core

#include "Common/Viewport.glsl"
#include "Particles/Shared_vert.glsl"

out vec4 geomPosition;
out vec4 geomRotation;
out vec4 geomScale_geomDrag;
out vec4 geomVelocity_geomDragRotational;
out vec4 geomAcceleration_geomLifeTime;
out vec4 geomRotationalVelocity;

#define geomScale geomScale_geomDrag.xyz
#define geomDrag geomScale_geomDrag.w
#define geomVelocity geomVelocity_geomDragRotational.xyz
#define geomDragRotational geomVelocity_geomDragRotational.w
#define geomAcceleration geomAcceleration_geomLifeTime.xyz
#define geomLifeTime geomAcceleration_geomLifeTime.w

void main(void) 
{
    geomLifeTime = inLifeTime - dt;
    gl_ClipDistance[0] = geomLifeTime;
    float drag = exp(-inDrag * dt);
    float rotDrag = exp(-inDragRotational * dt);
    geomVelocity = (inVelocity + dt * inAcceleration) * drag;
    geomRotationalVelocity = inRotationalVelocity * rotDrag;
    geomPosition = vec4(inPosition.xyz + dt * inVelocity, 1.0);
    geomRotation = inRotation + dt * inRotationalVelocity;
    geomScale = inScale;
    geomAcceleration = inAcceleration;
    geomDrag = inDrag;
    geomDragRotational = inDragRotational;
    geomPosition = world * geomPosition;
    //geomPosition = world * inPosition;
    //geomRotation = inRotation;
    //geomScale_geomDrag = inScale_inDrag;
    //geomVelocity_geomDragRotational = inVelocity_inDragRotational;
    //geomAcceleration = inAcceleration;
    //geomRotationalVelocity = inRotationalVelocity;
}
