
#pragma once

#include "object.h"

class Particle : public GameObject
{

	public:
		int LifeSpan;

		Particle( int StartLife, Vector2* StartPosition, double StartVelocity, double StartAngle );

		virtual void Update();
		virtual void Render() = 0;

};