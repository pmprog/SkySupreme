
#pragma once

#include <math.h>

#ifndef M_PI
#include "graphicslib.h"
#define M_PI      ALLEGRO_PI
#endif

class Vector2
{
  public:
    double X;
    double Y;

    Vector2( double X, double Y );
    Vector2( Vector2* CopyFrom );
    Vector2( double AngleDegrees );
    void RotateVector( double AngleDegrees );
    void RotateVector( double AngleDegrees, Vector2* RotationOrigin );

    bool operator==(Vector2 A);
    bool operator!=(Vector2 A);

    void Add(Vector2* Point);
    void Multiply(double Multiplier);
    double AngleTo( Vector2* CheckPoint );
		double DistanceTo( Vector2* CheckPoint );


};

