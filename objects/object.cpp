
#include "object.h"
#include "../stages/game.h"

GameStage* GameObject::Game;

GameObject::GameObject( double StartX, double StartY )
{
	Owner = 0;
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
	Owner = 0;
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

	Position->X += cos(Angle * (M_PI / 180.0)) * (Velocity / 10);
	if( Position->X * Game->graphicsMultiplier >= Framework::SystemFramework->GetDisplayWidth() )
	{
		Position->X -= Framework::SystemFramework->GetDisplayWidth() / Game->graphicsMultiplier;
	} else if( Position->X < 0 ) {
		Position->X += Framework::SystemFramework->GetDisplayWidth() / Game->graphicsMultiplier;
	}

	if( Game->Rules_HasGround )
	{
		// Framework::SystemFramework->GetDisplayHeight() - (32 * graphicsMultiplier)
	} else {
		if( Position->Y * Game->graphicsMultiplier >= Framework::SystemFramework->GetDisplayHeight() )
		{
			Position->Y -= Framework::SystemFramework->GetDisplayHeight() / Game->graphicsMultiplier;
		} else if( Position->Y < 0 ) {
			Position->Y += Framework::SystemFramework->GetDisplayHeight() / Game->graphicsMultiplier;
		}
	}


	Position->Y += sin(Angle * (M_PI / 180.0)) * (Velocity / 10);

}
