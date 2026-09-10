
layout (location = 0) in vec4 inPosition;
layout (location = 1) in vec4 inRotation;
layout (location = 2) in vec4 inScale_inDrag;
layout (location = 3) in vec4 inVelocity_inDragRotational;
layout (location = 4) in vec4 inAcceleration_inLifeTime;
layout (location = 5) in vec4 inRotationalVelocity;

#define inScale inScale_inDrag.xyz
#define inDrag inScale_inDrag.w
#define inVelocity inVelocity_inDragRotational.xyz
#define inDragRotational inVelocity_inDragRotational.w
#define inAcceleration inAcceleration_inLifeTime.xyz
#define inLifeTime inAcceleration_inLifeTime.w
