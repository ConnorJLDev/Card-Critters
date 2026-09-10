
struct Symbol
{
    vec2 pos;
    vec2 size;
    vec2 worldSize;
};

#pragma UBO_USAGE Symbols ASSET
layout (std140) uniform Symbols
{
    Symbol symbols[TEXT_MAX_SYMBOLS];
    int symbolStart;
    int symbolCount;
};

#pragma UBO_USAGE SymbolEffects MATERIAL
layout (std140) uniform SymbolEffects
{
    float currentEffectTimes[TEXT_MAX_EFFECTS];
};
