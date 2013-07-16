
#include "object.h"

GameStage* GameObject::Game;

GameObject::GameObject( double StartX, double StartY )
{
	Animation_Delay = 0;
	Animation_CurrentFrame = 0;
	Animation_TicksPerFrame = FRAMES_PER_SECOND / 4;
	Velocity = 0;
	Angle = 0;
	ForRemoval = false;
	Position = new Vector2( StartX, StartY );
}

GameObject::GameObject( Vector2* StartPosition )
{
	Animation_Delay = 0;
	Animation_CurrentFrame = 0;
	Animation_TicksPerFrame = FRAMES_PER_SECOND / 4;
	Velocity = 0;
	Angle = 0;
	ForRemoval = false;
	Position = new Vector2( StartPosition->X, StartPosition->Y );
}

void GameObject::Update()
{
	if( Angle < 0 )
	{
		Angle = Angle + 360.0;
	}
	if( Angle >= 360.0 )
	{
		Angle = Angle - 360.0;
	}

	Animation_Delay = (Animation_Delay + 1) % Animation_TicksPerFrame;
	if( Animation_Delay == 0 )
	{
		Animation_CurrentFrame++;
	}

	Position->X += cos(Angle * (M_PI / 180.0)) * Velocity;
	Position->Y += sin(Angle * (M_PI / 180.0)) * Velocity;

}
