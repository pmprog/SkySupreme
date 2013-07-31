
#include "game.h"
#include "survivalcontrollerselect.h"
#include "multicontrollerselect.h"
#include "freeflightcontrollerselect.h"

GameStage::GameStage( int Mode )
{
	graphicsMultiplier = Framework::SystemFramework->Settings->GetQuickIntegerValue( "Visual.GraphicScale" );

	Rules_GameMode = Mode;
	Rules_PlaneToPlaneCollisions = false;
	Rules_BulletToBulletCollisions = false;
	Rules_FriendlyFire = false;
	Rules_HasGround = true;

}

void GameStage::Begin()
{
	GameObject::Game = this;

	switch( Rules_GameMode )
	{
		case GAMEMODE_SURVIVAL:
			Framework::SystemFramework->ProgramStages->Push( (Stage*)new SurvivalControllerSelectStage() );
			break;
		case GAMEMODE_FREEFLIGHT:
			Framework::SystemFramework->ProgramStages->Push( (Stage*)new FreeFlightControllerSelectStage() );
			break;
		case GAMEMODE_LASTMANSTANDING:
		case GAMEMODE_TEAMBATTLES:
			Framework::SystemFramework->ProgramStages->Push( (Stage*)new MultipleControllerSelectStage() );
			break;
	}
}

void GameStage::Pause()
{
}

void GameStage::Resume()
{
}

void GameStage::Finish()
{
	GameObject::Game = 0;
}


void GameStage::Event(FwEvent *e)
{
	switch( e->Type )
	{
		case EVENT_KEY_DOWN:
			if( e->Data.Keyboard.keycode == ALLEGRO_KEY_ESCAPE )
			{
				delete Framework::SystemFramework->ProgramStages->Pop();
				return;
			}
			break;
	}

	for( std::vector<GameObject*>::iterator ptr = Objects.begin(); ptr != Objects.end(); ptr++ )
	{
		Plane* pObj = dynamic_cast<Plane*>(*ptr);
		if( pObj != 0 )
		{
			pObj->Event( e );
		}
	}
}

void GameStage::Update()
{
	GameObject* gObj;
	Plane* pObj;

	int ObjIdx = 0;
	ObjectsToRemove.clear();
	for( std::vector<GameObject*>::iterator ptr = Objects.begin(); ptr != Objects.end(); ptr++ )
	{
		gObj = (GameObject*)(*ptr);
		gObj->Update();
		if( gObj->ForRemoval )
		{
			ObjectsToRemove.push_back( ObjIdx );
		}

		if( Rules_HasGround && gObj->Position->Y >= (Framework::SystemFramework->GetDisplayHeight() / graphicsMultiplier) - 32 )
		{
			pObj = dynamic_cast<Plane*>(*ptr);
			if( pObj != 0 )
			{
				if( pObj->State != STATE_EXPLODING && pObj->State != STATE_EXPLODED )
				{
					pObj->LastHitBy = 0;
					pObj->SetState( STATE_EXPLODING );
				}
			} else {
				gObj->ForRemoval = true;
				ObjectsToRemove.push_back( ObjIdx );
			}
		}
		ObjIdx++;
	}

	while( (int)ObjectsToRemove.size() > 0 )
	{
		Objects.erase( Objects.begin() + ObjectsToRemove.back() );
		ObjectsToRemove.pop_back();
	}

	while( (int)ObjectsToAdd.size() > 0 )
	{
		Objects.push_back( ObjectsToAdd.front() );
		ObjectsToAdd.pop_front();
	}

	switch( Rules_GameMode )
	{
		case GAMEMODE_SURVIVAL:
			// TODO: Check player is alive
			// TODO: Spawn opponents
			break;
		case GAMEMODE_LASTMANSTANDING:
			if( GetPlaneObjects()->size() == 1 )
			{
				// TODO: Push Winner
				delete Framework::SystemFramework->ProgramStages->Pop();
			} else if( GetPlaneObjects()->size() == 0 ) {
				// TODO: Push HighScoring
				delete Framework::SystemFramework->ProgramStages->Pop();
			}
			break;
		case GAMEMODE_TEAMBATTLES:
			// TODO: Find a list of active teams, if only one remaining, push highscoring
			break;
	}

}

void GameStage::Render()
{
	al_clear_to_color( al_map_rgb( 92, 220, 218 ) ); // al_map_rgb( 43, 169, 168 ) );

	if( Rules_HasGround )
	{
		for( int i = 0; i < Framework::SystemFramework->GetDisplayWidth(); i += 16 * graphicsMultiplier )
		{
			al_draw_bitmap_region( GetGameScaledImage(), 96 * graphicsMultiplier, 64 * graphicsMultiplier, 16 * graphicsMultiplier, 32 * graphicsMultiplier, i, Framework::SystemFramework->GetDisplayHeight() - (32 * graphicsMultiplier), 0 );
		}
	}

	for( std::vector<GameObject*>::iterator ptr = Objects.begin(); ptr != Objects.end(); ptr++ )
	{
		GameObject* gObj = (GameObject*)(*ptr);
		gObj->Render();
	}

}

ALLEGRO_BITMAP* GameStage::GetGameScaledImage()
{
	ALLEGRO_BITMAP* tileSet;
	switch( (int)graphicsMultiplier )
	{
		case 2:
			tileSet = Framework::SystemFramework->GetImageManager()->GetScale2xImage( "resource/tileset.png" );
			break;
		case 3:
			tileSet = Framework::SystemFramework->GetImageManager()->GetScale3xImage( "resource/tileset.png" );
			break;
		case 4:
			tileSet = Framework::SystemFramework->GetImageManager()->GetScale4xImage( "resource/tileset.png" );
			break;
		default:
			tileSet = Framework::SystemFramework->GetImageManager()->GetImage( "resource/tileset.png" );
			graphicsMultiplier = 1;
			break;
	}
	return tileSet;
}

void GameStage::AddGameObject( GameObject* NewObject )
{
	ObjectsToAdd.push_back( NewObject );
}

void GameStage::AddPlayer( ALLEGRO_JOYSTICK* Controller )
{
	int Xpos = rand() % Framework::SystemFramework->GetDisplayWidth();
	int Ypos = rand() % ((Framework::SystemFramework->GetDisplayHeight() / 4) * 3);

	Plane* ply = new Plane( Controller, new Vector2( Xpos, Ypos ), PLANE_VELOCITY_MAX, 0.0 );
	AddGameObject( ply );
}

std::list<Plane*>* GameStage::GetPlaneObjects()
{
	std::list<Plane*>* list = new std::list<Plane*>();
	for( std::vector<GameObject*>::iterator ptr = Objects.begin(); ptr != Objects.end(); ptr++ )
	{
		Plane* pObj = dynamic_cast<Plane*>(*ptr);
		if( pObj != 0 )
		{
			if( pObj->State != STATE_EXPLODED && pObj->State != STATE_EXPLODING )
			{
				list->push_back( pObj );
			}
		}
	}
	return list;
}

std::list<Bullet*>* GameStage::GetBulletObjects()
{
	std::list<Bullet*>* list = new std::list<Bullet*>();
	for( std::vector<GameObject*>::iterator ptr = Objects.begin(); ptr != Objects.end(); ptr++ )
	{
		Bullet* bObj = dynamic_cast<Bullet*>(*ptr);
		if( bObj != 0 )
		{
			if( !bObj->ForRemoval )
			{
				list->push_back( bObj );
			}
		}
	}
	return list;
}

