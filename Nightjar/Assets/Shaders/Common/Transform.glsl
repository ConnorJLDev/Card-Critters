
// glsl constructs matrices as column major, so everything here appears transposed to what it should be.

mat4 TranslationMatrix(vec3 translation)
{
    return mat4(
        1.0, 0.0, 0.0, 0.0,
        0.0, 1.0, 0.0, 0.0,
        0.0, 0.0, 1.0, 0.0,
        translation.x, translation.y, translation.z, 1.0
    );
}

mat3 _RotationEulerMatrix_X(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return mat3(
        1.0, 0.0, 0.0,
        0.0,   c,   s,
        0.0,  -s,   c
    );
}

mat3 _RotationEulerMatrix_Y(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return mat3(
          c, 0.0,  -s,
        0.0, 1.0, 0.0,
          s, 0.0,   c
    );
}

mat3 _RotationEulerMatrix_Z(float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return mat3(
          c,   s, 0.0,
         -s,   c, 0.0,
        0.0, 0.0, 1.0
    );
}

mat3 RotationMatrixEulerXYZ(vec3 rotation)
{
    // Referenced from https://en.wikipedia.org/wiki/Rotation_matrix#In_three_dimensions
    float cx = cos(rotation.x);
    float sx = sin(rotation.x);
    float cy = cos(rotation.y);
    float sy = sin(rotation.y);
    float cz = cos(rotation.z);
    float sz = sin(rotation.z);
    return mat3(
                 cy*cx,          cy*sx,   -sy,
        sz*sy*cx-cz*sx, sz*sy*sx+cz*cx, sz*cy,
        cz*sy*cx+sz*sx, cz*sy*sx-sz*cx, cz*cy
    );
}

mat3 ScaleMatrix(vec3 scale)
{
    return mat3(
        scale.x, 0.0, 0.0,
        0.0, scale.y, 0.0,
        0.0, 0.0, scale.z
    );
}
