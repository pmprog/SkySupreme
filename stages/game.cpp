
#include "game.h"
#include "survivalcontrollerselect.h"
#include "multicontrollerselect.h"
#include "multiplayerhighscore.h"
#include "survivalgameover.h"

int GameStage::SurvivalArrivals[SURVIVAL_INTERVALS] = { 5, 10, 10, 10, 10, 9, 9, 9, 9, 9, 8, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 3 };
//int GameStage::SurvivalArrivals[SURVIVAL_INTERVALS] = { 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 8, 8, 8, 8, 8, 7, 7, 7, 7, 7, 7, 7, 6, 6, 6, 6, 6, 6, 5, 5, 5, 5, 5, 5, 4, 4, 4, 4, 3 };

GameStage::GameStage( int Mode )
{
	graphicsMultiplier = Framework::SystemFramework->Settings->GetQuickIntegerValue( "Visual.GraphicScale" );

	Rules_GameStarted = false;
	Rules_GameMode = Mode;
	Rules_PlaneToPlaneCollisions = false;
	Rules_BulletToBulletCollisions = false;
	Rules_FriendlyFire = false;
	Rules_HasGround = true;

	SurvivalPlayer = 0;
	SurvivalDelay = 0;
	SurvivalTimer = 0;
	SurvivalTotalTimer = 0;
	SurvivalIndex = 0;
}

void GameStage::Begin()
{
	GameObject::Game = this;

	switch( Rules_GameMode )
	{
		case GAMEMODE_SURVIVAL:
			Rules_PlaneToPlaneCollisions = false;
			Rules_BulletToBulletCollisions = false;
			Rules_FriendlyFire = true;
			Rules_HasGround = true;
			Framework::SystemFramework->ProgramStages->Push( (Stage*)new SurvivalControllerSelectStage() );
			break;
		case GAMEMODE_FREEFLIGHT:
			Rules_PlaneToPlaneCollisions = false;
			Rules_BulletToBulletCollisions = false;
			Rules_FriendlyFire = false;
			Rules_HasGround = false;
			for( int i = 0; i < FREEFLIGHT_CLOUD_COUNT; i++ )
			{
				Cloud* c = new Cloud(new Vector2( rand() % Framework::SystemFramework->GetDisplayWidth(), rand() % Framework::SystemFramework->GetDisplayHeight() ), (float)(rand() % 80) / 10, 180.0 );
				c->SetTileScale( graphicsMultiplier );
				AddGameObject( c );
			}
			Framework::SystemFramework->ProgramStages->Push( (Stage*)new MultipleControllerSelectStage() );
			break;
		case GAMEMODE_LASTMANSTANDING:
			Rules_PlaneToPlaneCollisions = false;
			Rules_BulletToBulletCollisions = false;
			Rules_FriendlyFire = true;
			Rules_HasGround = true;
			Framework::SystemFramework->ProgramStages->Push( (Stage*)new MultipleControllerSelectStage() );
			break;
		case GAMEMODE_TEAMBATTLES:
			Rules_PlaneToPlaneCollisions = false;
			Rules_BulletToBulletCollisions = false;
			Rules_FriendlyFire = false;
			Rules_HasGround = true;
			Framework::SystemFramework->ProgramStages->Push( (Stage*)new MultipleControllerSelectStage() );
			break;
	}
}

void GameStage::Pause()
{
}

void GameStage::Resume()
{
	Rules_GameStarted = true;
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

	bool AddedObjects = false;
	while( (int)ObjectsToAdd.size() > 0 )
	{
		Objects.push_back( ObjectsToAdd.front() );
		ObjectsToAdd.pop_front();
		AddedObjects = true;
	}
	if( AddedObjects )
	{
		SortObjectsList();
	}


	// Only process game rules if we're the active stage (ie, highscores will just update planes
	if( Framework::SystemFramework->ProgramStages->Current() == this )
	{
		UpdateByRules();
	}

}

void GameStage::UpdateByRules()
{
	bool PlayerStillAlive = false;
	std::list<Plane*>* PlanesList;
	std::list<int> TeamsAlive;

	if( Rules_GameStarted )
	{
		switch( Rules_GameMode )
		{

			case GAMEMODE_FREEFLIGHT:
				if( GetPlaneObjects()->size() == 0 )
				{
					delete Framework::SystemFramework->ProgramStages->Pop();
				}
				break;


			case GAMEMODE_SURVIVAL:
				for( std::vector<GameObject*>::const_iterator o = Objects.begin(); o != Objects.end(); o++ )
				{
					Plane* p = dynamic_cast<Plane*>( (*o) );
					if( p != 0 )
					{
						if( p->Controller_Keyboard || p->Controller_Joystick != 0 )
						{
							PlayerStillAlive = (p->State != STATE_EXPLODED && p->State != STATE_EXPLODING);
							break;
						}
					}
				}
				if( !PlayerStillAlive )
				{
					Framework::SystemFramework->ProgramStages->Push( (Stage*)new SurvivalGameOverStage( SurvivalPlayer ) );
				} else {
					UpdateSurvival();
				}
				break;


			case GAMEMODE_LASTMANSTANDING:
				PlanesList = GetPlaneObjects();
				if( PlanesList->size() == 1 )
				{
					PlanesList->front()->Score += 5;
				}
				if( PlanesList->size() <= 1 )
				{
					Framework::SystemFramework->ProgramStages->Push( (Stage*)new MultiplayerHighScoreStage() );
				}
				break;


			case GAMEMODE_TEAMBATTLES:
				PlanesList = GetPlaneObjects();
				if( PlanesList->size() == 0 ) {
					Framework::SystemFramework->ProgramStages->Push( (Stage*)new MultiplayerHighScoreStage() );
				} else {

					for( std::list<Plane*>::iterator p = PlanesList->begin(); p != PlanesList->end(); p++ )
					{
						Plane* pl = (Plane*)(*p);

						bool foundTeam = false;
						for( std::list<int>::iterator i = TeamsAlive.begin(); i != TeamsAlive.end(); i++ )
						{
							if( (*i) == pl->Team )
							{
								foundTeam = true;
								break;
							}
						}
						if( !foundTeam )
						{
							TeamsAlive.push_back( pl->Team );
						}
					}
					if( TeamsAlive.size() == 1 )
					{
						Framework::SystemFramework->ProgramStages->Push( (Stage*)new MultiplayerHighScoreStage() );
					}
				}
				break;
		}
	}
}

void GameStage::UpdateSurvival()
{
	Plane* pObj;

	if( Rules_GameMode == GAMEMODE_SURVIVAL )
	{
		SurvivalDelay = (SurvivalDelay + 1) % FRAMES_PER_SECOND;
		if( SurvivalDelay == 0 )
		{
			SurvivalTotalTimer++;
			SurvivalTimer++;
			pObj = 0;
			if( SurvivalTimer >= SurvivalArrivals[SurvivalIndex] )
			{
				SurvivalTimer = 0;
				pObj = AddPlayer( (ALLEGRO_JOYSTICK*)-1 );
				if( SurvivalPlayer != 0 )
				{
					SurvivalPlayer->Score++;
				}
				if( SurvivalIndex < SURVIVAL_INTERVALS - 1 )
				{
					SurvivalIndex++;
				} else {
					if( rand() % 3 == 0 )
					{
						int tmpY;
						if( Rules_HasGround )
						{
							tmpY = rand() % ((Framework::SystemFramework->GetDisplayHeight() / graphicsMultiplier) - 32);
						} else {
							tmpY = rand() % (Framework::SystemFramework->GetDisplayHeight() / graphicsMultiplier);
						}
						AddGameObject( new Cloud( new Vector2( Framework::SystemFramework->GetDisplayWidth(), tmpY ), (double)(rand() % 300) / 10.0, 180.0 ) );
					}
				}
			}
			if( pObj != 0 )
			{
				pObj->Team = (Objects.size() % 7) + 1;
			}
		}
	}
}

void GameStage::Render()
{
	al_clear_to_color( al_map_rgb( 194, 217, 254 ) ); // al_map_rgb( 43, 169, 168 ) );

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

	switch( Rules_GameMode )
	{
		case GAMEMODE_SURVIVAL:
			RenderSurvival();
			break;
	}
}

void GameStage::RenderSurvival()
{
	ALLEGRO_FONT* survivalFont = Framework::SystemFramework->GetFontManager()->GetFont( "resource/falsepos.ttf", Framework::SystemFramework->GetDisplayHeight() / 18, 0 );
	ALLEGRO_COLOR survivalBorder = al_map_rgb( 0, 0, 0 );
	ALLEGRO_COLOR survivalNormal = al_map_rgb( 255, 255, 255 );
	int Lsiz = al_get_font_line_height( survivalFont );
	int curScore = 0;

	for( std::vector<GameObject*>::const_iterator o = Objects.begin(); o != Objects.end(); o++ )
	{
		Plane* p = dynamic_cast<Plane*>( (*o) );
		if( p != 0 )
		{
			if( p->Controller_Keyboard || p->Controller_Joystick != 0 )
			{
				curScore = p->Score;
				break;
			}
		}
	}

	char cdTime[10];
	memset( (void*)&cdTime, 0, 10 );
	sprintf( (char*)&cdTime, "%d", SurvivalArrivals[SurvivalIndex] - SurvivalTimer );
	int Xpos = Framework::SystemFramework->GetDisplayWidth() - al_get_text_width( survivalFont, "Next Plane:" ) - 6;
	int XposCD = Xpos + ((Framework::SystemFramework->GetDisplayWidth() - Xpos) / 2) - (al_get_text_width( survivalFont, cdTime ) / 2);

	for( int yBrd = -1; yBrd < 2; yBrd++ )
	{
		for( int xBrd = -1; xBrd < 2; xBrd++ )
		{
			al_draw_textf( survivalFont, survivalBorder, 6 + xBrd, Framework::SystemFramework->GetDisplayHeight() - Lsiz - 4 + yBrd, 0, "Score: %d", curScore );
			al_draw_text( survivalFont, survivalBorder, Xpos + xBrd, Framework::SystemFramework->GetDisplayHeight() - Lsiz - Lsiz - 4 + yBrd, 0, "Next Plane:" );
			al_draw_text( survivalFont, survivalBorder, XposCD + xBrd, Framework::SystemFramework->GetDisplayHeight() - Lsiz - 4 + yBrd, 0, (char*)&cdTime );
		}
	}

	al_draw_textf( survivalFont, survivalNormal, 4, Framework::SystemFramework->GetDisplayHeight() - Lsiz - 4, 0, "Score: %d", curScore );
	al_draw_text( survivalFont, survivalNormal, Xpos, Framework::SystemFramework->GetDisplayHeight() - Lsiz - Lsiz - 4, 0, "Next Plane:" );
	al_draw_text( survivalFont, survivalNormal, XposCD, Framework::SystemFramework->GetDisplayHeight() - Lsiz - 4, 0, (char*)&cdTime );
}

ALLEGRO_BITMAP* GameStage::GetGameScaledImage()
{
	if( graphicsMultiplier > 4 )
	{
		graphicsMultiplier = 4;
	}
	return GetGameImageAtScale( graphicsMultiplier );
}

void GameStage::AddGameObject( GameObject* NewObject )
{
	ObjectsToAdd.push_back( NewObject );
}

Plane* GameStage::AddPlayer( ALLEGRO_JOYSTICK* Controller )
{
	int Xpos = rand() % (Framework::SystemFramework->GetDisplayWidth() / graphicsMultiplier);
	int Ypos = rand() % (((Framework::SystemFramework->GetDisplayHeight() / graphicsMultiplier) / 4) * 3);

	Plane* ply = new Plane( Controller, new Vector2( Xpos, Ypos ), PLANE_VELOCITY_MAX, 0.0 );
	if( rand() % 2 == 1 )
	{
		ply->Angle = 180.0;
		ply->Flipped = true;
	}
	AddGameObject( ply );

	if( Controller != (ALLEGRO_JOYSTICK*)-1 )
	{
		SurvivalPlayer = ply;
	}

	return ply;
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

ALLEGRO_BITMAP* GameStage::GetGameImageAtScale( int Scale )
{
	ALLEGRO_BITMAP* tileSet;
	switch( (int)Scale )
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
			break;
	}
	return tileSet;
}

std::list<Plane*>* GameStage::GetAllPlaneObjects()
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

void GameStage::SortObjectsList()
{
	std::sort( Objects.begin(), Objects.end(), GameObject::CompareZ );
}
