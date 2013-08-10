
#include "settings.h"

void SettingsStage::Begin()
{
}

void SettingsStage::Pause()
{
}

void SettingsStage::Resume()
{
}

void SettingsStage::Finish()
{
}

void SettingsStage::Event( FwEvent* e )
{
	if( e->Type == EVENT_KEY_DOWN || e->Type == EVENT_JOYSTICK_BUTTON_DOWN )
	{
		delete Framework::SystemFramework->ProgramStages->Pop();
	}
}

void SettingsStage::Update()
{
}

void SettingsStage::Render()
{
	ALLEGRO_FONT* optionFont = Framework::SystemFramework->GetFontManager()->GetFont( "resource/falsepos.ttf", Framework::SystemFramework->GetDisplayHeight() / 12, 0 );
	int opFntH = al_get_font_line_height( optionFont );
	int qtrX = Framework::SystemFramework->GetDisplayWidth() / 4;
	int qtrY = Framework::SystemFramework->GetDisplayHeight() / 3;

	Framework::SystemFramework->ProgramStages->Previous( this )->Render();

	al_draw_filled_rectangle( 0, 0, qtrX * 4, qtrY * 3, al_map_rgba( 0, 0, 0, 128 ) );

	al_draw_filled_rounded_rectangle( qtrX, qtrY, qtrX * 3, qtrY * 2, 8, 8, al_map_rgb( 0, 0, 0 ) );

	al_draw_text( optionFont, al_map_rgb( 255, 255, 255 ), (qtrX * 2) - (al_get_text_width( optionFont, "Under Development" ) / 2), (Framework::SystemFramework->GetDisplayHeight() / 2) - (opFntH * 1.5), 0, "Under Development" );
	al_draw_text( optionFont, al_map_rgb( 255, 255, 255 ), (qtrX * 2) - (al_get_text_width( optionFont, "Press Any Key" ) / 2), (Framework::SystemFramework->GetDisplayHeight() / 2) + (opFntH / 2), 0, "Press Any Key" );

}
