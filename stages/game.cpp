
#include "game.h"

GameStage::GameStage()
{
	graphicsMultiplier = Framework::SystemFramework->Settings->GetQuickIntegerValue( "Visual.GraphicScale" );
	Plane* ply = new Plane( 0, new Vector2( 20, 20 ), PLANE_VELOCITY_MAX * 0.7, 0.0 );
	AddGameObject( ply );

	ply = new Plane( (ALLEGRO_JOYSTICK*)-1, new Vector2( 20, 220 ), PLANE_VELOCITY_MAX * 0.7, 0.0 );
	ply->Team = 3;
	AddGameObject( ply );


	Rules_PlaneToPlaneCollisions = false;
	Rules_BulletToBulletCollisions = false;
	Rules_FriendlyFire = false;
	Rules_HasGround = true;
}

void GameStage::Begin()
{
	GameObject::Game = this;
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
			// Not strictly true, fix
			gObj->ForRemoval = true;
			ObjectsToRemove.push_back( ObjIdx );
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

std::list<Plane*>* GameStage::GetPlaneObjects()
{
	std::list<Plane*>* list = new std::list<Plane*>();
	for( std::vector<GameObject*>::iterator ptr = Objects.begin(); ptr != Objects.end(); ptr++ )
	{
		Plane* pObj = dynamic_cast<Plane*>(*ptr);
		if( pObj != 0 )
		{
			list->push_back( pObj );
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
			list->push_back( bObj );
		}
	}
	return list;
}

