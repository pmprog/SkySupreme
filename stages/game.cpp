
#include "game.h"
#include "../objects/bullet.h"
#include "../objects/cloud.h"
#include "../objects/explosion.h"
#include "../objects/plane.h"
#include "../objects/smokecloud.h"

GameStage::GameStage()
{
	graphicsMultiplier = Framework::SystemFramework->Settings->GetQuickIntegerValue( "Visual.GraphicScale" );
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
		case EVENT_MOUSE_DOWN:
			if( e->Data.Mouse.button == 1 )
				Objects.push_back( new SmokeCloud( new Vector2( e->Data.Mouse.x / (double)graphicsMultiplier, e->Data.Mouse.y / (double)graphicsMultiplier ), 0.1, 45.0 ) );
			else
				Objects.push_back( new SmokeCloud( new Vector2( e->Data.Mouse.x / (double)graphicsMultiplier, e->Data.Mouse.y / (double)graphicsMultiplier ), 0.1, 90.0 + 45.0 ) );
			break;
	}
}

void GameStage::Update()
{


	int ObjIdx = 0;
	ObjectsToRemove.clear();
	for( std::vector<GameObject*>::iterator ptr = Objects.begin(); ptr != Objects.end(); ptr++ )
	{
		GameObject* gObj = (GameObject*)(*ptr);
		gObj->Update();
		if( gObj->ForRemoval )
		{
			ObjectsToRemove.push_back( ObjIdx );
		}
		ObjIdx++;
	}

	while( (int)ObjectsToRemove.size() > 0 )
	{
		Objects.erase( Objects.begin() + ObjectsToRemove.back() );
		ObjectsToRemove.pop_back();
	}
}

void GameStage::Render()
{
	al_clear_to_color( al_map_rgb( 92, 220, 218 ) ); // al_map_rgb( 43, 169, 168 ) );

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
