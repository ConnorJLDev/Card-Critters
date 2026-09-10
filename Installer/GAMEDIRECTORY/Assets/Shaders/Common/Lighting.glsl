
struct Light
{
    vec4 color;
    vec3 position;
    vec3 normal;
    vec3 attenDistance;
    vec3 attenAngle;
};

#define LIGHT_COUNT 8

layout (std140) uniform Lighting
{
    vec4 lightAmbientColor;
    Light lights[LIGHT_COUNT];
};

vec4 DiffuseLighting(vec3 pos, vec3 nrm)
{
    vec4 lightResult = lightAmbientColor;
    vec3 nNormal = -normalize(nrm);
    for (int i = 0; i < LIGHT_COUNT; ++i)
    {
        Light light = lights[i];
        vec3 lNormal = normalize(light.normal);
        // Diffuse
        vec3 offset = pos - light.position;
        float distSqr = dot(offset, offset);
        float dist = sqrt(distSqr);
        vec3 oNormal = offset / dist;
        float lightStrength = clamp(dot(nNormal, oNormal), 0.0, 1.0);
        // Calculate distance attenuation.
        float attenDistanceFinal = 
            light.attenDistance.x + 
            light.attenDistance.y * dist + 
            light.attenDistance.z * distSqr;
        lightStrength *= clamp(attenDistanceFinal, 0, 1);
        // Calculate angle attenuation.
        float attenAngle = dot(oNormal, lNormal);
        float attenAngleFinal = 
            light.attenAngle.x + 
            light.attenAngle.y * attenAngle + 
            light.attenAngle.z * attenAngle * attenAngle;
        lightStrength *= clamp(attenAngleFinal, 0, 1);
        lightResult += lightStrength * light.color;
    }
    return clamp(lightResult, 0, 1);
}
