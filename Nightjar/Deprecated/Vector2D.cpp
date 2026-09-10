//------------------------------------------------------------------------------
//
// File Name:	Vector2D.cpp
// Author(s):	Barend Brink (barend.brink)
// Project:		Nightjar
// Course:		GAM200
//
// Copyright � 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include <stdarg.h>

#include "Vector2D.h"

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

	// This function sets the coordinates of the 2D vector to 0
void Vector2D::Zero()
{
	x = 0;
	y = 0;
}

// This function sets the coordinates of the 2D vector to a & b
void Vector2D::Set(float a, float b)
{
	x = a;
	y = b;
}

// In this function, the vector will be set to the opposite of itself (e.g. x = -x
// & y = -y)
void Vector2D::Neg()
{
	x = -x;
	y = -y;
}

// In this function, the vector will be the sum of Vec0 and Vec1 (x result = x0
// + x1)
void Vector2D::Add(const Vector2D& Vec0, const Vector2D& Vec1)
{
	x = Vec0.x + Vec1.x;
	y = Vec0.y + Vec1.y;
}

// In this function, the vector will be the difference between Vec0 and Vec1
// (specifically, Vec0 - Vec1)
void Vector2D::Sub(const Vector2D& Vec0, const Vector2D& Vec1)
{
	x = Vec0.x - Vec1.x;
	y = Vec0.y - Vec1.y;
}

// In this function, the vector will be the unit vector of itself
// (NOTE: Care must be taken when pResult = pVec0!)
void Vector2D::Normalize()
{
	float magnitude = std::sqrt(x * x + y * y);

	if (magnitude != 0)
	{
		x /= magnitude;
		y /= magnitude;
	}
}

Vector2D Vector2D::Normalized() const
{
	float mag = std::sqrt(x * x + y * y);

	if (mag == 0) return Vector2D(0.0f, 0.0f);

	return Vector2D(x / mag, y / mag);
}

// In this function, the vectors elements will be scaled by the value
// 'scale'
void Vector2D::Scale(float scale)
{
	x = (x * scale);
	y = (y * scale);
}

// In this function, the vector will be the vector Vec0 scaled by 'scale' and
// added to Vec1
void Vector2D::ScaleAdd(const Vector2D& Vec0, float scale, const Vector2D& Vec1)
{
	x = Vec0.x * scale + Vec1.x;
	y = Vec0.y * scale + Vec1.y;
}

// In this function, the vector will be the vector Vec0 scaled by 'scale' and
// Vec1 will be subtracted from it
void Vector2D::ScaleSub(const Vector2D& Vec0, float scale, const Vector2D& Vec1)
{
	x = Vec0.x * scale - Vec1.x;
	y = Vec0.y * scale - Vec1.y;
}

// This function returns the length of the vector
float Vector2D::Length() const
{
	return std::sqrt((x * x) + (y * y));
}

// This function returns the square of the vectors length.
// NOTE: The square root function must NOT be called by this function.
float Vector2D::SquareLength() const
{
	return ((x * x) + (y * y));
}

// This function returns the distance between two points.
float Vector2D::Distance(const Vector2D& Vec0, const Vector2D& Vec1)
{
	return std::sqrt(((Vec1.x - Vec0.x) * (Vec1.x - Vec0.x)) + ((Vec1.y - Vec0.y) * (Vec1.y - Vec0.y)));
}

// This function returns the distance squared between two points.
// NOTE: The square root function must NOT be called by this function.
float Vector2D::SquareDistance(const Vector2D& Vec0, const Vector2D& Vec1)
{
	return (((Vec1.x - Vec0.x) * (Vec1.x - Vec0.x)) + ((Vec1.y - Vec0.y) * (Vec1.y - Vec0.y)));
}

// This function returns the dot product between Vec0 and Vec1
float Vector2D::DotProduct(const Vector2D& Vec0, const Vector2D Vec1)
{
	return (Vec0.x * Vec1.x) + (Vec0.y * Vec1.y);
}

// This function computes the coordinates of the unit vector represented by
// the angle "angle", which is in Degrees. Converting from degrees to radians
// can be performed as follows:
//	 radians = (angle * M_PI) / 180.0f
// M_PI is defined in "math.h", which may be included as follows:
//   #define _USE_MATH_DEFINES
//   #include <math.h>
void Vector2D::FromAngleDeg(float angle)
{
	x = (std::cos((angle * M_PI) / 180.0f));
	y = (std::sin((angle * M_PI) / 180.0f));
}

// This function computes the coordinates of the unit vector represented by
// the angle "angle", which is in Radians. HINT: x = cos(angle), y =
// sin(angle).
void Vector2D::FromAngleRad(float angle)
{
	x = std::cos(angle);
	y = std::sin(angle);
}

// This function computes the angle, in radians, of the specified vector.
// HINT: Angle (radians) = atan2f(direction Y, direction X).
// NOTE: If the pointer is NULL, then return 0.0f.
float Vector2D::ToAngleRad()
{
	return std::atan2(y, x);
}

/*----------------------------------------------------------------------------*/


