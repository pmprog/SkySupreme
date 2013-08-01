
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

	std::list<Plane*> *curPlanes = Game->GetPlaneObjects();
	for( std::list<Plane*>::iterator ptr = curPlanes->begin(); ptr != curPlanes->end(); ptr++ )
	{
		GameObject* gObj = (GameObject*)(*ptr);
		Plane* pObj = (Plane*)(*ptr);
		if( pObj != Owner && (Game->Rules_FriendlyFire || pObj->Team != ((Plane*)Owner)->Team) )
		{
			if( pObj->State == STATE_FLYING || pObj->State == STATE_STALLED || pObj->State == STATE_SHOOT )
			{
				 if( Position->DistanceTo( pObj->Position ) < 24.0 )
				 {
					 pObj->SetState( STATE_HIT );
					 pObj->LastHitBy = (Plane*)Owner;
					 ((Plane*)Owner)->Score++;
					 ForRemoval = true;
					 break;
				 }
			}
		}
	}
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
