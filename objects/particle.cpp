
#include "particle.h"

Particle::Particle( int StartLife, Vector2* StartPosition, double StartVelocity, double StartAngle ) : GameObject( StartPosition )
{
	LifeSpan = StartLife;
	Velocity = StartVelocity;
	Angle = StartAngle;
}

void Particle::Update()
{
	GameObject::Update();
	LifeSpan--;
	if( LifeSpan <= 0 )
	{
		Position->X = -100;
		Position->Y = -100;
		Velocity = 0.0;
		ForRemoval = true;
	}
}
