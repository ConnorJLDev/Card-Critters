//------------------------------------------------------------------------------
//
// File Name:	Vector2D.h
// Author(s):	Barend Brink (barend.brink)
// Project:		Nightjar
// Course:		GAM200
//
// Copyright � 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#define _USE_MATH_DEFINES
#include <cmath>

class Vector2D {
private:
	float x;
	float y;

public: 
	//Constructor, makes 2D vector with values xVal and yVal
	Vector2D(float xVal, float yVal) : x(xVal), y(yVal) {}

	// This function sets the coordinates of the 2D vector to 0
  void Zero();

  // This function sets the coordinates of the 2D vector to a & b
  void Set(float a, float b);

  // In this function, the vector will be set to the opposite of itself (e.g. x = -x
  // & y = -y)
  void Neg();

  // In this function, the vector will be the sum of Vec0 and Vec1 (x result = x0
  // + x1)
  void Add(const Vector2D& Vec0, const Vector2D& Vec1);

  // In this function, the vector will be the difference between Vec0 and Vec1
  // (specifically, Vec0 - Vec1)
  void Sub(const Vector2D& Vec0, const Vector2D& Vec1);

  // In this function, the vector will be the unit vector of itself
  // (NOTE: Care must be taken when pResult = pVec0!)
  void Normalize();

  Vector2D Normalized() const;

  // In this function, the vectors elements will be scaled by the value
  // 'scale'
  void Scale(float scale);

  // In this function, the vector will be the vector Vec0 scaled by 'scale' and
  // added to Vec1
  void ScaleAdd(const Vector2D& Vec0, float scale, const Vector2D& Vec1);

  // In this function, the vector will be the vector Vec0 scaled by 'scale' and
  // Vec1 will be subtracted from it
  void ScaleSub(const Vector2D& Vec0, float scale, const Vector2D& Vec1);

  // This function returns the length of the vector
  float Length() const;

  // This function returns the square of the vectors length.
  // NOTE: The square root function must NOT be called by this function.
  float SquareLength() const;

  // This function returns the distance between two points.
  static float Distance(const Vector2D& Vec0, const Vector2D& Vec1);

  // This function returns the distance squared between two points.
  // NOTE: The square root function must NOT be called by this function.
  float SquareDistance(const Vector2D& Vec0, const Vector2D& Vec1);

  // This function returns the dot product between Vec0 and Vec1
  float DotProduct(const Vector2D& Vec0, const Vector2D Vec1);

  // This function computes the coordinates of the unit vector represented by
  // the angle "angle", which is in Degrees. Converting from degrees to radians
  // can be performed as follows:
  //	 radians = (angle * M_PI) / 180.0f
  // M_PI is defined in "math.h", which may be included as follows:
  //   #define _USE_MATH_DEFINES
  //   #include <math.h>
  void FromAngleDeg(float angle);

  // This function computes the coordinates of the unit vector represented by
  // the angle "angle", which is in Radians. HINT: x = cos(angle), y =
  // sin(angle).
  void FromAngleRad(float angle);

  // This function computes the angle, in radians, of the specified vector.
  // HINT: Angle (radians) = atan2f(direction Y, direction X).
  // NOTE: If the pointer is NULL, then return 0.0f.
  float ToAngleRad();

};

/*----------------------------------------------------------------------------*/


