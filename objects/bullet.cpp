
#include "bullet.h"
#include "../stages/game.h"

Bullet::Bullet( GameObject* Shooter ) : Particle( BULLET_LIFE, Shooter->Position, BULLET_VELOCITY, Shooter->Angle )
{
	Owner = Shooter;
}

void Bullet::Update()
{
	Particle::Update();
	Animation_CurrentFrame = Animation_CurrentFrame % 2;


/*
	for( std::vector<GameObject*>::iterator ptr = Objects.begin(); ptr != Objects.end(); ptr++ )
	{
		GameObject* gObj = (GameObject*)(*ptr);
		Plane* pObj = dynamic_cast<Plane*>(gObj);
		if( pObj != 0 )
		{
			pObj->Event( e );
		}
	}
*/
}

void Bullet::Render()
{
	if( ForRemoval )
	{
		return;
	}

	ALLEGRO_BITMAP* tileset = Game->GetGameScaledImage();
	int tileX = (64 + (Animation_CurrentFrame * 16)) * Game->graphicsMultiplier;
	int tileY = 16 * Game->graphicsMultiplier;

	ALLEGRO_BITMAP* tmp = al_create_sub_bitmap( tileset, tileX, tileY, 16 * Game->graphicsMultiplier, 16 * Game->graphicsMultiplier );
	al_draw_rotated_bitmap( tmp, 8 * Game->graphicsMultiplier, 8 * Game->graphicsMultiplier, Position->X * Game->graphicsMultiplier, Position->Y * Game->graphicsMultiplier, Angle * (M_PI / 180), 0 );
	al_destroy_bitmap( tmp );
	
}
