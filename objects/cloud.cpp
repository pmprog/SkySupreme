
#include "cloud.h"
#include "../stages/game.h"

Cloud::Cloud( Vector2* StartPosition, double StartVelocity, double StartAngle ) : Particle( -1, StartPosition, StartVelocity, StartAngle )
{
	CloudStyle = rand() % 2;
}

void Cloud::Update()
{
	int gMul;

	if( Game == 0 )
	{
		gMul = TileMultiplier;
	} else {
		gMul = Game->graphicsMultiplier;
	}

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
	if( Position->X * gMul >= Framework::SystemFramework->GetDisplayWidth() )
	{
		Position->X -= Framework::SystemFramework->GetDisplayWidth() / gMul;
	} else if( Position->X < 0 ) {
		Position->X += Framework::SystemFramework->GetDisplayWidth() / gMul;
	}

	if( Position->Y * gMul >= Framework::SystemFramework->GetDisplayHeight() )
	{
		Position->Y -= Framework::SystemFramework->GetDisplayHeight() / gMul;
	} else if( Position->Y < 0 ) {
		Position->Y += Framework::SystemFramework->GetDisplayHeight() / gMul;
	}
}

void Cloud::Render()
{
	ALLEGRO_BITMAP* tset;
	int gMul;

	if( Game == 0 )
	{
		gMul = TileMultiplier;
	} else {
		gMul = Game->graphicsMultiplier;
	}
	tset = Game->GetGameImageAtScale( gMul );

	switch( CloudStyle )
	{
		case 0:
			al_draw_bitmap_region( tset, 96 * gMul, 32 * gMul, 48 * gMul, 32 * gMul, (Position->X - 24) * gMul, (Position->Y - 16) * gMul, 0 );
			break;
		case 1:
			al_draw_bitmap_region( tset, 112 * gMul, 64 * gMul, 32 * gMul, 32 * gMul, (Position->X - 16) * gMul, (Position->Y - 16) * gMul, 0 );
			break;
	}
}

void Cloud::SetTileScale( int ScreenMultiplier )
{
	TileMultiplier = ScreenMultiplier;
}
