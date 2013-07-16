
#include "event.h"

FwEvent::FwEvent()
{
  Type = EVENT_UNDEFINED;
}

FwEvent::FwEvent( ALLEGRO_EVENT* Source )
{
  switch( Source->type )
  {
    case ALLEGRO_EVENT_KEY_DOWN:
      Type = EVENT_KEY_DOWN;
      break;
    case ALLEGRO_EVENT_KEY_UP:
      Type = EVENT_KEY_UP;
      break;
    case ALLEGRO_EVENT_MOUSE_AXES:
      Type = EVENT_MOUSE_MOVE;
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_DOWN:
      Type = EVENT_MOUSE_DOWN;
      break;
    case ALLEGRO_EVENT_MOUSE_BUTTON_UP:
      Type = EVENT_MOUSE_UP;
      break;
    case ALLEGRO_EVENT_JOYSTICK_AXIS:
      Type = EVENT_JOYSTICK_AXIS;
      break;
    case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
      Type = EVENT_JOYSTICK_BUTTON_DOWN;
      break;
    case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
      Type = EVENT_JOYSTICK_BUTTON_UP;
      break;
    case ALLEGRO_EVENT_TIMER:
      Type = EVENT_TIMER_TICK;
      break;
    case ALLEGRO_EVENT_DISPLAY_SWITCH_IN:
      Type = EVENT_WINDOW_ACTIVATE;
      break;
    case ALLEGRO_EVENT_DISPLAY_SWITCH_OUT:
      Type = EVENT_WINDOW_DEACTIVATE;
      break;
    default:
      Type = EVENT_UNDEFINED;
      break;
  }
  memcpy( &Data, Source, sizeof(ALLEGRO_EVENT) );
}
