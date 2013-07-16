
#pragma once

#include "graphicslib.h"
#include "memory.h"
#include <string>

enum EventTypes
{
  EVENT_WINDOW_ACTIVATE,
  EVENT_WINDOW_DEACTIVATE,
  EVENT_KEY_DOWN,
  EVENT_KEY_UP,
  EVENT_MOUSE_DOWN,
  EVENT_MOUSE_UP,
  EVENT_MOUSE_MOVE,
  EVENT_JOYSTICK_AXIS,
  EVENT_JOYSTICK_BUTTON_DOWN,
  EVENT_JOYSTICK_BUTTON_UP,
  EVENT_TIMER_TICK,
  EVENT_NETWORK_CONNECTION_REQUEST,
  EVENT_NETWORK_PACKET_RECEIVED,
  EVENT_NETWORK_DISCONNECTED,
  EVENT_DOWNLOAD_PROGRESS,
  EVENT_DOWNLOAD_COMPLETE,
  EVENT_AUDIO_FINISHED,
  EVENT_UNDEFINED
};

typedef struct FRAMEWORK_DOWNLOAD_EVENT
{
  std::string* URL;
  Memory* Contents;
  double DownloadedBytes;
  double TotalBytesToDownload;
  double UploadedBytes;
  double TotalBytesToUpload;
} FRAMEWORK_DOWNLOAD_EVENT;

typedef struct FRAMEWORK_NETWORK_EVENT
{
  int Reserved;
} FRAMEWORK_NETWORK_EVENT;


typedef union FwEvent_Data
{
  ALLEGRO_DISPLAY_EVENT     Display;
  ALLEGRO_JOYSTICK_EVENT    Joystick;
  ALLEGRO_KEYBOARD_EVENT    Keyboard;
  ALLEGRO_MOUSE_EVENT       Mouse;
  ALLEGRO_TIMER_EVENT       Timer;
  ALLEGRO_TOUCH_EVENT       Touch;
  ALLEGRO_USER_EVENT        User;
  FRAMEWORK_NETWORK_EVENT   Network;
  FRAMEWORK_DOWNLOAD_EVENT  Download;
} FwEvent_Data;


class FwEvent
{
  public:
    EventTypes Type;
    FwEvent_Data Data;

    FwEvent();
    FwEvent( ALLEGRO_EVENT* Source );
};
