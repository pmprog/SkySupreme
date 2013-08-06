
#include "framework.h"
#include "../stages/bootup.h"

Framework* Framework::SystemFramework;

Framework::Framework()
{
#ifdef WRITE_LOG
  printf( "Framework: Startup\n" );
#endif

  // Init Allegro
	if( !al_init() )
	{
		return;
	}
	al_init_font_addon();
	if( !al_install_keyboard() || !al_install_mouse() || !al_init_primitives_addon() || !al_init_ttf_addon() || !al_init_image_addon() )
	{
		return;
	}
	if( al_install_joystick() )
  {
    al_reconfigure_joysticks();
  }

  audioInitialised = false;
  InitialiseAudioSystem();

  std::string selectedLanguage;
  quitProgram = false;
  ProgramStages = new StageStack();
  framesToProcess = 0;
  Settings = new ConfigFile( "settings.cfg" );

  eventQueue = al_create_event_queue();

  InitialiseDisplay();

	al_register_event_source( eventQueue, al_get_keyboard_event_source() );
	al_register_event_source( eventQueue, al_get_mouse_event_source() );
	al_register_event_source( eventQueue, al_get_joystick_event_source() );

	fpsTimer = al_create_timer( 1.0 / (float)FRAMES_PER_SECOND );
	al_register_event_source( eventQueue, al_get_timer_event_source( fpsTimer ) );
	al_start_timer( fpsTimer );

	al_hide_mouse_cursor( displaySurface );


  imageMgr = new ImageManager( this );
  fontMgr = new FontManager( this );
  audioMgr = new AudioManager( this );
  int maxDownloads = 4;
  if( Settings->KeyExists( "Downloads.MaxConcurrentDownloads" ) )
  {
    Settings->GetIntegerValue( "Downloads.MaxConcurrentDownloads", &maxDownloads );
  }
  downloadMgr = new DownloadManager( this, maxDownloads );
  networkMgr = new NetworkManager( this );
  languageMgr = new LanguageManager();
  if( Settings->KeyExists( "Application.Language" ) )
  {
    Settings->GetStringValue( "Application.Language", &selectedLanguage );
    languageMgr->SetActiveLanguage( selectedLanguage );
  }

  SystemFramework = this;

  extraEventsMutex = al_create_mutex();
}

Framework::~Framework()
{

  Settings->Save( "settings.cfg" );

#ifdef WRITE_LOG
  printf( "Framework: Shutdown\n" );
#endif

  al_destroy_event_queue( eventQueue );
  al_destroy_display( displaySurface );
  al_destroy_mutex( extraEventsMutex );

  delete imageMgr;
  delete audioMgr;
  delete fontMgr;
  delete networkMgr;
  delete downloadMgr;

  // Shutdown Allegro
  if( mixer != 0 )
  {
    al_destroy_mixer( mixer );
  }
  if( voice != 0 )
  {
    al_destroy_voice( voice );
  }

	al_uninstall_keyboard();
	al_uninstall_mouse();
	al_shutdown_primitives_addon();
	al_shutdown_ttf_addon();
	al_shutdown_image_addon();
	al_shutdown_font_addon();
	al_uninstall_audio();
  al_uninstall_joystick();
}

void Framework::Run()
{


#ifdef WRITE_LOG
  printf( "Framework: Run.Program Loop\n" );
#endif

  // Set BootUp Stage active
  ProgramStages->Push( (Stage*)new BootUp() );

  while( !quitProgram )
  {
    ProcessEvents();
  }
  downloadMgr->Tidy();
}

void Framework::ProcessEvents()
{
  ALLEGRO_EVENT e;
  FwEvent* fwev;

#ifdef WRITE_LOG
  printf( "Framework: ProcessEvents.AllegroEvents\n" );
#endif
  while( al_get_next_event( eventQueue, &e ) )
  {
    switch( e.type )
    {
      case ALLEGRO_EVENT_DISPLAY_CLOSE:
        quitProgram = true;
        break;
      case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
        al_reconfigure_joysticks();
        break;
      case ALLEGRO_EVENT_TIMER:
        if( e.timer.source == fpsTimer )
        {
          framesToProcess++;
        } else {
          fwev = new FwEvent( &e );
          ProgramStages->Current()->Event( fwev );
          delete fwev;
        }
        break;
      default:
        fwev = new FwEvent( &e );
        ProgramStages->Current()->Event( fwev );
        delete fwev;
    }
    if( ProgramStages->IsEmpty() )
    {
      quitProgram = true;
      return;
    }
  }

#ifdef WRITE_LOG
  printf( "Framework: ProcessEvents.ExtraEvents\n" );
#endif
  al_lock_mutex( extraEventsMutex );
  while( extraEvents.size() > 0 )
  {
    fwev = extraEvents.front();
    extraEvents.pop_front();
    if( fwev->Type == EVENT_DOWNLOAD_COMPLETE )
    {
      downloadMgr->Event( fwev );
    }
    ProgramStages->Current()->Event( fwev );
    delete fwev;
    if( ProgramStages->IsEmpty() )
    {
      quitProgram = true;
      return;
    }
  }
  al_unlock_mutex( extraEventsMutex );

#ifdef WRITE_LOG
  printf( "Framework: ProcessEvents.Update\n" );
#endif
  while( framesToProcess > 0 )
  {
    if( ProgramStages->IsEmpty() )
    {
      quitProgram = true;
      return;
    }
    ProgramStages->Current()->Update();
    downloadMgr->Update();
    networkMgr->Update();
    framesToProcess--;
  }

	// Not bothered if these update every frame, only cache clearing
  imageMgr->Update();
  fontMgr->Update();
	if( musicFilename != "" )
	{
		audioMgr->GetMusic( musicFilename );	// Prevent uncaching of current music
	}
  audioMgr->Update();


#ifdef WRITE_LOG
  printf( "Framework: ProcessEvents.Render\n" );
#endif
  if( !ProgramStages->IsEmpty() )
  {
    ProgramStages->Current()->Render();
    al_flip_display();
  }

}

void Framework::PushEvent( FwEvent* e )
{
  al_lock_mutex( extraEventsMutex );
  extraEvents.push_back( e );
  al_unlock_mutex( extraEventsMutex );
}

void Framework::ShutdownFramework()
{
  while( !ProgramStages->IsEmpty() )
  {
    delete ProgramStages->Pop();
  }
  quitProgram = true;
}

ImageManager* Framework::GetImageManager()
{
  return imageMgr;
}

FontManager* Framework::GetFontManager()
{
  return fontMgr;
}

AudioManager* Framework::GetAudioManager()
{
  return audioMgr;
}

DownloadManager* Framework::GetDownloadManager()
{
  return downloadMgr;
}

NetworkManager* Framework::GetNetworkManager()
{
  return networkMgr;
}

LanguageManager* Framework::GetLanguageManager()
{
  return languageMgr;
}

int Framework::GetDisplayWidth()
{
  return al_get_display_width( displaySurface );
}

int Framework::GetDisplayHeight()
{
  return al_get_display_height( displaySurface );
}

void Framework::RestoreDisplayTarget()
{
  al_set_target_backbuffer( displaySurface );
}

void Framework::PlayMusic( std::string Filename, ALLEGRO_PLAYMODE Mode )
{
#ifdef WRITE_LOG
  printf( "Framework: Play Music '%s'\n", Filename.c_str() );
#endif

	if( voice != 0 )
	{
		musicStream = audioMgr->GetMusic( Filename );
		if( musicStream != 0 )
    {
			musicFilename = Filename;
      al_set_audio_stream_playmode( musicStream, Mode );
      al_attach_audio_stream_to_mixer( musicStream, mixer );
      al_set_audio_stream_playing( musicStream, true );
    } else {
#ifdef WRITE_LOG
      printf( "Framework: Could not load music '%s'\n", Filename.c_str() );
#endif
    }
	}
}

void Framework::StopMusic()
{
	if( voice != 0 && musicStream != 0 )
	{
		al_set_audio_stream_playing( musicStream, false );
	}
	musicFilename = "";
}

void Framework::SaveSettings()
{
  // Just to keep the filename consistant
  Settings->Save( "settings.cfg" );
}

void Framework::SetWindowTitle( std::string* NewTitle )
{
  al_set_window_title( displaySurface, NewTitle->c_str() );
}

void Framework::InitialiseAudioSystem()
{
#ifdef WRITE_LOG
  printf( "Framework: Initialise Audio\n" );
#endif
  voice = 0;
  mixer = 0;

	if( audioInitialised || al_install_audio() )
	{
		if( audioInitialised || al_init_acodec_addon() )
		{
			voice = al_create_voice(44100, ALLEGRO_AUDIO_DEPTH_INT16, ALLEGRO_CHANNEL_CONF_2);
			if( voice != 0 )
			{
				mixer = al_create_mixer(44100, ALLEGRO_AUDIO_DEPTH_FLOAT32, ALLEGRO_CHANNEL_CONF_2);
				if( mixer != 0 )
        {
					al_attach_mixer_to_voice(mixer, voice);
        } else {
          al_destroy_voice( voice );
          voice = 0;
        }
			}
		}
	}
}

void Framework::ShutdownAudioSystem()
{
#ifdef WRITE_LOG
  printf( "Framework: Shutdown Audio\n" );
#endif

  if( mixer != 0 )
  {
		al_detach_mixer( mixer );
		al_destroy_mixer( mixer );
		mixer = 0;
  }
  if( voice != 0 )
  {
    al_destroy_voice( voice );
    voice = 0;
  }
}

void Framework::InitialiseDisplay()
{
#ifdef WRITE_LOG
  printf( "Framework: Initialise Display\n" );
#endif

	int scrW = 800;
	int scrH = 480;
	bool scrFS = false;

	if( Settings->KeyExists( "Visual.ScreenWidth" ) )
  {
    Settings->GetIntegerValue( "Visual.ScreenWidth", &scrW );
  }
	if( Settings->KeyExists( "Visual.ScreenHeight" ) )
  {
    Settings->GetIntegerValue( "Visual.ScreenHeight", &scrH );
  }
	if( Settings->KeyExists( "Visual.FullScreen" ) )
  {
    Settings->GetBooleanValue( "Visual.FullScreen", &scrFS );
  }
  if( scrFS )
  {
    al_set_new_display_flags( ALLEGRO_FULLSCREEN_WINDOW );
  }
  al_set_new_display_option(ALLEGRO_VSYNC, 1, ALLEGRO_SUGGEST);
  displaySurface = al_create_display( scrW, scrH );
  if( displaySurface == 0 )
  {
    return;
  }
  al_set_blender( ALLEGRO_ADD, ALLEGRO_ALPHA, ALLEGRO_INVERSE_ALPHA );
  al_register_event_source( eventQueue, al_get_display_event_source( displaySurface ) );
}

void Framework::ShutdownDisplay()
{
#ifdef WRITE_LOG
  printf( "Framework: Shutdown Display\n" );
#endif
  al_unregister_event_source( eventQueue, al_get_display_event_source( displaySurface ) );
  fontMgr->Clear();
  imageMgr->Clear();
  al_destroy_display( displaySurface );

}

