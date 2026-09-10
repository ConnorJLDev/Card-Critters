//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.h
// Author(s):	Doug Schilling (dschilling), Jason Nguyen (jason.nguyen)
// Project:		Nightjar
// Course:		GAM200
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#pragma once

class Matrix2D {
public:
    float m[4][4];

    Matrix2D();

    void setIdentity();
    void transpose();
    void concat(const Matrix2D &other);
    void translate(float x, float y);
    void scale(float x, float y);
    void rotateDeg(float angle);
    void rotateRad(float angle);

    Vector2D multVec(const Vector2D& vec) const;
};
