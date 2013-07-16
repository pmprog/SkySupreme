
#pragma once

#include "particle.h"

class Bullet : public Particle
{

	public:
		Bullet( GameObject* Shooter );

		virtual void Update();
		virtual void Render();

};