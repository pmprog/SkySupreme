
#include "menu.h"
#include "game.h"

MenuStage::MenuStage()
{
	OptionSelected = 0;
	OptionGlowIndex = 0;
	OptionGlowDelay = 0;

	//OptionGlow[0] = al_map_rgb( 160, 200, 120 );
	//OptionGlow[1] = al_map_rgb( 180, 220, 140 );
	//OptionGlow[2] = al_map_rgb( 200, 240, 160 );
	//OptionGlow[3] = al_map_rgb( 220, 255, 180 );
	OptionGlow[0] = al_map_rgb( 60, 100, 20 );
	OptionGlow[1] = al_map_rgb( 80, 120, 40 );
	OptionGlow[2] = al_map_rgb( 100, 140, 60 );
	OptionGlow[3] = al_map_rgb( 120, 160, 80 );
	OptionGlow[4] = OptionGlow[2];
	OptionGlow[5] = OptionGlow[1];

}

void MenuStage::Begin()
{
}

void MenuStage::Pause()
{
}

void MenuStage::Resume()
{
}

void MenuStage::Finish()
{
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
					if( OptionSelected < 3 )
					{
						OptionSelected++;
					}
					break;
				case ALLEGRO_KEY_SPACE:
				case ALLEGRO_KEY_ENTER:
					ProcessMenuOption();
					break;
			}
			break;

		case EVENT_JOYSTICK_AXIS:
			if( e->Data.Joystick.id == al_get_joystick( 0 ) )
			{
				if( e->Data.Joystick.stick == 0 && e->Data.Joystick.axis == 0 )
				{
					if( e->Data.Joystick.pos < 0.0 )
					{
						if( OptionSelected > 0 )
						{
							OptionSelected--;
						}
					} else if( e->Data.Joystick.pos > 0.0 ) {
						if( OptionSelected < 3 )
						{
							OptionSelected++;
						}
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
	OptionGlowDelay = (OptionGlowDelay + 1) % 10;
	if( OptionGlowDelay == 0 )
	{
		OptionGlowIndex = (OptionGlowIndex + 1) % 6;
	}
}

void MenuStage::Render()
{
	ALLEGRO_BITMAP* tileSet;
	ALLEGRO_BITMAP* titleImg = Framework::SystemFramework->GetImageManager()->GetImage( "resource/title.png" );
	int tileMultiplier = Framework::SystemFramework->Settings->GetQuickIntegerValue( "Visual.GraphicScale" );

	switch( tileMultiplier )
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
			tileMultiplier = 1;
			break;
	}

	al_clear_to_color( al_map_rgb( 92, 220, 218 ) ); // al_map_rgb( 43, 169, 168 ) );

	if( al_get_bitmap_width( titleImg ) > Framework::SystemFramework->GetDisplayWidth() )
	{
		double scaler = (double)Framework::SystemFramework->GetDisplayWidth() / (double)al_get_bitmap_width( titleImg );
		al_draw_scaled_bitmap( titleImg, 0, 0, al_get_bitmap_width( titleImg ), al_get_bitmap_height( titleImg ), 0, 4, Framework::SystemFramework->GetDisplayWidth(), al_get_bitmap_height( titleImg ) * scaler, 0 );
	} else {
		al_draw_bitmap( titleImg, (Framework::SystemFramework->GetDisplayWidth() / 2) - (al_get_bitmap_width( titleImg ) / 2), 8, 0 );
	}

	ALLEGRO_FONT* menuFont = Framework::SystemFramework->GetFontManager()->GetFont( "resource/falsepos.ttf", Framework::SystemFramework->GetDisplayHeight() / 12, 0 );
	ALLEGRO_COLOR menuSelected = OptionGlow[OptionGlowIndex];
	ALLEGRO_COLOR menuNormal = al_map_rgb( 0, 0, 0 );
	int Ypos = Framework::SystemFramework->GetDisplayHeight() - (al_get_font_line_height( menuFont ) * 5);

	al_draw_text( menuFont, ( OptionSelected == 0 ? menuSelected : menuNormal ), 20, Ypos, 0, "Single Player" );
	Ypos += al_get_font_line_height( menuFont );
	al_draw_text( menuFont, ( OptionSelected == 1 ? menuSelected : menuNormal ), 20, Ypos, 0, "Local Multiplayer" );
	Ypos += al_get_font_line_height( menuFont );
	al_draw_text( menuFont, ( OptionSelected == 2 ? menuSelected : menuNormal ), 20, Ypos, 0, "Settings" );
	Ypos += al_get_font_line_height( menuFont );
	al_draw_text( menuFont, ( OptionSelected == 3 ? menuSelected : menuNormal ), 20, Ypos, 0, "Quit" );


}

void MenuStage::ProcessMenuOption()
{
	switch( OptionSelected )
	{
		case 0:
			Framework::SystemFramework->ProgramStages->Push( new GameStage() );
			break;
		case 1:
			break;
		case 2:
			break;
		case 3:
			delete Framework::SystemFramework->ProgramStages->Pop();
			break;
	}
}
