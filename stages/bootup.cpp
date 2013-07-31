
#include "bootup.h"
#include "menu.h"
#include "../framework/framework.h"

void BootUp::Begin()
{
	GameObject::Game = 0;
	Framework::SystemFramework->SetWindowTitle( new std::string("Sky Supreme") );
	InitialiseSettings(); // Configure defaults
	Framework::SystemFramework->SaveSettings();
#ifndef DELAYBOOTUP
	FinishedBooting();
#endif // DELAYBOOTUP
}

void BootUp::Pause()
{

}

void BootUp::Resume()
{

}

void BootUp::Finish()
{

}

void BootUp::Event(FwEvent *e)
{
  if( e->Type == EVENT_KEY_DOWN )
  {
    FinishedBooting();
  }
}

void BootUp::Update()
{

}

void BootUp::Render()
{

}

void BootUp::FinishedBooting()
{
	delete Framework::SystemFramework->ProgramStages->Pop();
	Framework::SystemFramework->ProgramStages->Push( (Stage*)new MenuStage() );
}

void BootUp::InitialiseSettings()
{
	if( !Framework::SystemFramework->Settings->KeyExists( "Application.Language" ) )
	{
		Framework::SystemFramework->Settings->SetStringValue( "Application.Language", new std::string("en") );
	}
	if( !Framework::SystemFramework->Settings->KeyExists( "Visual.ScreenWidth" ) )
  {
    Framework::SystemFramework->Settings->SetIntegerValue( "Visual.ScreenWidth", 800 );
  }
	if( !Framework::SystemFramework->Settings->KeyExists( "Visual.ScreenHeight" ) )
  {
    Framework::SystemFramework->Settings->SetIntegerValue( "Visual.ScreenHeight", 480 );
  }
	if( !Framework::SystemFramework->Settings->KeyExists( "Visual.FullScreen" ) )
  {
    Framework::SystemFramework->Settings->SetBooleanValue( "Visual.FullScreen", false );
  }
	if( !Framework::SystemFramework->Settings->KeyExists( "Visual.GraphicScale" ) )
  {
    Framework::SystemFramework->Settings->SetIntegerValue( "Visual.GraphicScale", 1 );
  }
}
