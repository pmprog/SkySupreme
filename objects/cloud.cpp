
#include "cloud.h"
#include "../stages/game.h"

Cloud::Cloud( Vector2* StartPosition, double StartVelocity, double StartAngle ) : Particle( -1, StartPosition, StartVelocity, StartAngle )
{
	CloudStyle = rand() % 4;
	Velocity *= (CloudStyle + 1);
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
	if( Position->X < -64 )
	{
		Position->X += (Framework::SystemFramework->GetDisplayWidth() / gMul) + 128;
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
			al_draw_bitmap_region( tset, 48 * gMul, 160 * gMul, 80 * gMul, 48 * gMul, (Position->X - 40) * gMul, (Position->Y - 24) * gMul, 0 );
			break;
		case 1:
			al_draw_bitmap_region( tset, 0, 208 * gMul, 128 * gMul, 64 * gMul, (Position->X - 64) * gMul, (Position->Y - 32) * gMul, 0 );
			break;
		case 2:
			al_draw_bitmap_region( tset, 0, 272 * gMul, 112 * gMul, 48 * gMul, (Position->X - 56) * gMul, (Position->Y - 24) * gMul, 0 );
			break;
		case 3:
			al_draw_bitmap_region( tset, 0, 320 * gMul, 144 * gMul, 48 * gMul, (Position->X - 72) * gMul, (Position->Y - 24) * gMul, 0 );
			break;
	}
}

void Cloud::SetTileScale( int ScreenMultiplier )
{
	TileMultiplier = ScreenMultiplier;
}
