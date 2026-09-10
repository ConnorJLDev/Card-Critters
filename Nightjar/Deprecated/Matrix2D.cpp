//------------------------------------------------------------------------------
//
// File Name:	Matrix2D.cpp
// Author(s):	Doug Schilling (dschilling), Jason Nguyen (jason.nguyen)
// Project:		Nightjar
// Course:		GAM200
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "Matrix2D.h"
#include "Vector2D.h"

#define _USE_MATH_DEFINES
#include <cmath> // for M_PI

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

Matrix2D::Matrix2D() { 
  setIdentity(); 
}

// This function sets the matrix equal to the identity matrix.
void Matrix2D::setIdentity() {
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      m[i][j] = (i == j) ? 1.0f : 0.0f;
    }
  }
}

// This function calculates the transpose matrix of this matrix.
void Matrix2D::transpose() {
  for (int i = 0; i < 4; ++i) {
    for (int j = i + 1; j < 4; ++j) {
      float temp = m[i][j];
      m[i][j] = m[j][i];
      m[j][i] = temp;
    }
  }
}

// This function multiplies this matrix with another matrix and saves the result in this matrix.
void Matrix2D::concat(const Matrix2D &other) {
  Matrix2D temp;

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      temp.m[i][j] = 0.0f;
      for (int k = 0; k < 4; k++) {
        temp.m[i][j] += m[i][k] * other.m[k][j];
      }
    }
  }

  *this = temp;
}

// This function creates a translation matrix from x & y and concatenates it with this matrix.
void Matrix2D::translate(float x, float y) {
  Matrix2D temp;
  temp.setIdentity();

  temp.m[0][3] = x;
  temp.m[1][3] = y;

  concat(temp);
}

// This function creates a scaling matrix from x & y and concatenates it with this matrix.
void Matrix2D::scale(float x, float y) {
  Matrix2D temp;
  temp.setIdentity();

  temp.m[0][0] = x;
  temp.m[1][1] = y;

  concat(temp);
}

// This matrix creates a rotation matrix from "angle" whose value is in degrees and concatenates it with this matrix.
void Matrix2D::rotateDeg(float angle) {
  float radians = angle * static_cast<float>(M_PI) / 180.0f;
  rotateRad(radians);
}

// This matrix creates a rotation matrix from "angle" whose value is in radians and concatenates it with this matrix.
void Matrix2D::rotateRad(float angle) {
  Matrix2D temp;
  temp.setIdentity();

  float cosA = cosf(angle);
  float sinA = sinf(angle);

  temp.m[0][0] = cosA;
  temp.m[0][1] = -sinA;
  temp.m[1][0] = sinA;
  temp.m[1][1] = cosA;

  concat(temp);
}

// This function multiplies the matrix with the vector Vec and returns the transformed vector.
Vector2D Matrix2D::multVec(const Vector2D &vec) const {
  Vector2D result;

  result.x = m[0][0] * vec.x + m[0][1] * vec.y + m[0][3];
  result.y = m[1][0] * vec.x + m[1][1] * vec.y + m[1][3];

  return result;
}

//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------