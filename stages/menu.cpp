
#include "menu.h"
#include "game.h"
#include "multiplayermode.h"

MenuStage::MenuStage()
{
	OptionSelected = 1;
	OptionGlowIndex = 0;
	OptionGlowDelay = 0;

	WasJoyUp = false;
	WasJoyDown = false;

	OptionGlow[0] = al_map_rgb( 10, 10, 255 );
	OptionGlow[1] = al_map_rgb( 30, 30, 255 );
	OptionGlow[2] = al_map_rgb( 50, 50, 255 );
	OptionGlow[3] = al_map_rgb( 70, 70, 255 );
	OptionGlow[4] = OptionGlow[2];
	OptionGlow[5] = OptionGlow[1];

	tileMultiplier = Framework::SystemFramework->Settings->GetQuickIntegerValue( "Visual.GraphicScale" );
	if( tileMultiplier > 4 )
	{
		tileMultiplier = 4;
	}
	tileSet = GameStage::GetGameImageAtScale( tileMultiplier );

	for( int i = 0; i < MENU_CLOUD_COUNT; i++ )
	{
		Cloud* c = new Cloud(new Vector2( rand() % Framework::SystemFramework->GetDisplayWidth(), rand() % Framework::SystemFramework->GetDisplayHeight() ), (float)(rand() % 80) / 10, 180.0 );
		c->SetTileScale( tileMultiplier );
		BackgroundClouds.push_back( c );
	}

	Framework::SystemFramework->PlayMusic( "resource/506187_dogfight.ogg", ALLEGRO_PLAYMODE_LOOP );
}

void MenuStage::Begin()
{
	GameObject::Game = 0;
}

void MenuStage::Pause()
{
}

void MenuStage::Resume()
{
	GameObject::Game = 0;
}

void MenuStage::Finish()
{
	Framework::SystemFramework->StopMusic();
}

void MenuStage::Event(FwEvent *e)
{
	switch( e->Type )
	{
		case EVENT_KEY_DOWN:
			switch( e->Data.Keyboard.keycode )
			{
				case ALLEGRO_KEY_ESCAPE:
					delete Framework::SystemFramework->ProgramStages->Pop();
					break;

				case ALLEGRO_KEY_UP:
					if( OptionSelected > 0 )
					{
						OptionSelected--;
					}
					break;
				case ALLEGRO_KEY_DOWN:
					if( OptionSelected < 5 )
					{
						OptionSelected++;
					}
					break;
#ifdef PANDORA
				case ALLEGRO_KEY_PGUP:
				case ALLEGRO_KEY_PGDN:
				case ALLEGRO_KEY_HOME:
				case ALLEGRO_KEY_END:
#endif
				case ALLEGRO_KEY_SPACE:
				case ALLEGRO_KEY_ENTER:
				case ALLEGRO_KEY_Z:
				case ALLEGRO_KEY_X:
					ProcessMenuOption();
					break;
			}
			break;

		case EVENT_JOYSTICK_AXIS:
			if( e->Data.Joystick.id == al_get_joystick( 0 ) )
			{
				if( e->Data.Joystick.axis == 1 )
				{
					if( e->Data.Joystick.pos < 0.0 )
					{
						if( !WasJoyUp && OptionSelected > 0 )
						{
							OptionSelected--;
							WasJoyUp = true;
						}
					} else {
						WasJoyUp = false;
					}
					if( e->Data.Joystick.pos > 0.0 )
					{
						if( !WasJoyDown && OptionSelected < 5 )
						{
							OptionSelected++;
							WasJoyDown = true;
						}
					} else {
						WasJoyDown = false;
					}
				}
			}
			break;
		case EVENT_JOYSTICK_BUTTON_DOWN:
			if( e->Data.Joystick.id == al_get_joystick( 0 ) )
			{
				ProcessMenuOption();
			}
			break;
	}
}

void MenuStage::Update()
{
	OptionGlowDelay = (OptionGlowDelay + 1) % 12;
	if( OptionGlowDelay == 0 )
	{
		OptionGlowIndex = (OptionGlowIndex + 1) % 6;
	}
	for( std::list<Cloud*>::iterator c = BackgroundClouds.begin(); c != BackgroundClouds.end(); c++ )
	{
		((Cloud*)*c)->Update();
	}
}

void MenuStage::Render()
{
	
	ALLEGRO_BITMAP* titleImg = Framework::SystemFramework->GetImageManager()->GetImage( "resource/title.png" );

	al_clear_to_color( al_map_rgb( 92, 220, 218 ) ); // al_map_rgb( 43, 169, 168 ) );

	GameStage::GetGameImageAtScale( tileMultiplier );
	for( std::list<Cloud*>::iterator c = BackgroundClouds.begin(); c != BackgroundClouds.end(); c++ )
	{
		((Cloud*)*c)->Render();
	}

	double scaler = min( (double)((Framework::SystemFramework->GetDisplayHeight() / 3.0) / (double)al_get_bitmap_height( titleImg )), (double)Framework::SystemFramework->GetDisplayWidth() / (double)al_get_bitmap_width( titleImg ));
	al_draw_scaled_bitmap( titleImg, 0, 0, al_get_bitmap_width( titleImg ), al_get_bitmap_height( titleImg ), (Framework::SystemFramework->GetDisplayWidth() / 2) - ((al_get_bitmap_width( titleImg ) * scaler) / 2), 4, al_get_bitmap_width( titleImg ) * scaler, al_get_bitmap_height( titleImg ) * scaler, 0 );

	ALLEGRO_FONT* menuFont = Framework::SystemFramework->GetFontManager()->GetFont( "resource/falsepos.ttf", Framework::SystemFramework->GetDisplayHeight() / 12, 0 );
	ALLEGRO_COLOR menuSelected = OptionGlow[OptionGlowIndex];
	ALLEGRO_COLOR menuNormal = al_map_rgb( 0, 0, 0 );
	int Lsiz = al_get_font_line_height( menuFont );
	int Ypos = Framework::SystemFramework->GetDisplayHeight() - (al_get_font_line_height( menuFont ) * 7);

	al_draw_text( menuFont, ( OptionSelected == 0 ? menuSelected : menuNormal ), 20, Ypos, 0, "Free Flight" );
	Ypos += Lsiz;
	al_draw_text( menuFont, ( OptionSelected == 1 ? menuSelected : menuNormal ), 20, Ypos, 0, "Survival (Single Player)" );
	Ypos += Lsiz;
	al_draw_text( menuFont, ( OptionSelected == 2 ? menuSelected : menuNormal ), 20, Ypos, 0, "Local Mutliplayer" );
	Ypos += Lsiz;
	al_draw_text( menuFont, ( OptionSelected == 3 ? menuSelected : menuNormal ), 20, Ypos, 0, "Network Multiplayer" );
	Ypos += Lsiz;
	al_draw_text( menuFont, ( OptionSelected == 4 ? menuSelected : menuNormal ), 20, Ypos, 0, "Settings" );
	Ypos += Lsiz;
	al_draw_text( menuFont, ( OptionSelected == 5 ? menuSelected : menuNormal ), 20, Ypos, 0, "Quit" );
}

void MenuStage::ProcessMenuOption()
{
	switch( OptionSelected )
	{
		case 0:
			Framework::SystemFramework->ProgramStages->Push( new GameStage( GAMEMODE_FREEFLIGHT ) );
			break;
		case 1:
			Framework::SystemFramework->ProgramStages->Push( new GameStage( GAMEMODE_SURVIVAL ) );
			break;
		case 2:
			Framework::SystemFramework->ProgramStages->Push( new MultiplayerModeStage() );
			break;
		case 3:
			//Framework::SystemFramework->ProgramStages->Push( new GameStage( GAMEMODE_TEAMBATTLES ) );
			break;
		case 4:
			break;
		case 5:
			delete Framework::SystemFramework->ProgramStages->Pop();
			break;
	}
}
