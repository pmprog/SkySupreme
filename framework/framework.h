
#pragma once

#include "graphicslib.h"
#include "event.h"

#include "configfile.h"
#include "stagestack.h"

#include "../managers/audiomanager.h"
#include "../managers/downloadmanager.h"
#include "../managers/fontmanager.h"
#include "../managers/imagemanager.h"
#include "../managers/networkmanager.h"
#include "../managers/languagemanager.h"

#define FRAMES_PER_SECOND 100
#define FRAME_TIME_IN_MS  1000 / FRAMES_PER_SECOND

class Framework
{
  private:
    bool quitProgram;
    int deltaTime;
    int framesToProcess;
    ALLEGRO_DISPLAY* displaySurface;
    ALLEGRO_EVENT_QUEUE* eventQueue;
    bool audioInitialised;
    ALLEGRO_MIXER* mixer;
    ALLEGRO_VOICE* voice;
    ALLEGRO_TIMER* fpsTimer;

    ALLEGRO_MUTEX* extraEventsMutex;
    std::list<FwEvent*> extraEvents;

    ImageManager* imageMgr;
    AudioManager* audioMgr;
    FontManager* fontMgr;
    NetworkManager* networkMgr;
    DownloadManager* downloadMgr;
    LanguageManager* languageMgr;

    ALLEGRO_AUDIO_STREAM* musicStream;

  public:
    static Framework* SystemFramework;

    Framework();
    ~Framework();

    void Run();
    void ProcessEvents();
    void PushEvent( FwEvent* e );
    void ShutdownFramework();

    ConfigFile* Settings;
    StageStack* ProgramStages;

    ImageManager* GetImageManager();
    FontManager* GetFontManager();
    AudioManager* GetAudioManager();
    DownloadManager* GetDownloadManager();
    NetworkManager* GetNetworkManager();
    LanguageManager* GetLanguageManager();

    int GetDisplayWidth();
    int GetDisplayHeight();
    void RestoreDisplayTarget();

    void PlayMusic( std::string Filename, ALLEGRO_PLAYMODE Mode );
    void StopMusic();

    void SaveSettings();
    void SetWindowTitle( std::string* NewTitle );

    void InitialiseAudioSystem();
    void ShutdownAudioSystem();

    void InitialiseDisplay();
    void ShutdownDisplay();
};
