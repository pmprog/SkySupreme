
#include "vector2.h"

Vector2::Vector2( double X, double Y )
{
  this->X = X;
  this->Y = Y;
}

Vector2::Vector2( Vector2* CopyFrom )
{
  X = CopyFrom->X;
  Y = CopyFrom->Y;
}

Vector2::Vector2( double AngleDegrees )
{
	double rotSin = sin(AngleDegrees * (M_PI/180.0));
	double rotCos = cos(AngleDegrees * (M_PI/180.0));
	X = rotCos - rotSin;
	Y = rotCos + rotSin;
}

void Vector2::RotateVector( double AngleDegrees )
{
  Vector2* tmp = new Vector2( 0, 0 );
  RotateVector( AngleDegrees, tmp );
  delete tmp;
}

void Vector2::RotateVector( double AngleDegrees, Vector2* RotationOrigin )
{
	double rotSin = sin(AngleDegrees * (M_PI/180.0));
	double rotCos = cos(AngleDegrees * (M_PI/180.0));
	double tmpX;
	tmpX = ((X - RotationOrigin->X) * rotCos) - ((Y - RotationOrigin->Y) * rotSin);
	Y = ((Y - RotationOrigin->Y) * rotCos) + ((X - RotationOrigin->X) * rotSin);
	X = tmpX;
}

bool Vector2::operator==(Vector2 A)
{
  return (this->X == A.X && this->Y == A.Y);
}

bool Vector2::operator!=(Vector2 A)
{
  return (this->X != A.X || this->Y != A.Y);
}

void Vector2::Add(Vector2* Point)
{
  X += Point->X;
  Y += Point->Y;
}

void Vector2::Multiply(double Multiplier)
{
  X *= Multiplier;
  Y *= Multiplier;
}

double Vector2::AngleTo( Vector2* CheckPoint )
{
  double r = atan2( CheckPoint->Y - Y, CheckPoint->X - X ) * 180.0 / M_PI;
	while( r >= 360.0 )
	{
		r -= 360.0;
	}
	while( r < 0.0 )
	{
		r += 360.0;
	}
  return r;
}

double Vector2::DistanceTo( Vector2* CheckPoint )
{
	return sqrt( pow(CheckPoint->X - X, 2.0) + pow(CheckPoint->Y - Y, 2.0) );
}
