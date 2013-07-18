
#pragma once

#include "particle.h"
#include "plane.h"

#define BULLET_VELOCITY			PLANE_VELOCITY_MAX * 2.5
#define BULLET_LIFE					120

class Bullet : public Particle
{

	public:
		Bullet( GameObject* Shooter );

		virtual void Update();
		virtual void Render();

};