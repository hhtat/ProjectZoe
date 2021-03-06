#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <X11/extensions/XTest.h>

#include "xzoe.h"
#include "pru.h"

static int xzoe_init_kb_map( xzoe_kb_map_entry kb_map[ XZOE_NUM_KEYS ] );
static bool xzoe_validate_kb_buffer( const pru_kb_buffer* buffer, xzoe_kb_map_entry kb_map[ XZOE_NUM_KEYS ] );

void* xzoe_kb_thread_start( void* arg )
{
  xzoe_kb_map_entry kb_map[ XZOE_NUM_KEYS ];
  pru_kb_buffer buffer;
  int i;

  if( xzoe_init_kb_map( kb_map ) )
  {
    fprintf( stderr, "xzoe_init_kb_map failed\n" );
    return NULL;
  }

  do
  {
    if( pru_kb_wait() )
    {
      fprintf( stderr, "pru_kb_wait failed\n" );
      return NULL;
    }

    if( pru_kb_get( &buffer ) )
    {
      fprintf( stderr, "pru_kb_get failed\n" );
      return NULL;
    }

    if( !xzoe_validate_kb_buffer( &buffer, kb_map ) )
    {
      continue;
    }

    for( i = 0; i < XZOE_NUM_KEYS; i++ )
    {
      if( kb_map[ i ].down )
      {
        if( buffer.data[ kb_map[ i ].row ][ kb_map[ i ].col ] == PRU_KB_UP )
        {
          kb_map[ i ].down = false;

          XLockDisplay( xzoe_display );

          if( XTestFakeKeyEvent( xzoe_display, kb_map[ i ].keycode, false, 0 ) != 1 )
          {
            fprintf( stderr, "XTestFakeKeyEvent failed\n" );
            return NULL;
          }

          XUnlockDisplay( xzoe_display );
        }
      }
      else
      {
        if( buffer.data[ kb_map[ i ].row ][ kb_map[ i ].col ] == PRU_KB_DOWN )
        {
          kb_map[ i ].down = true;

          XLockDisplay( xzoe_display );

          if( XTestFakeKeyEvent( xzoe_display, kb_map[ i ].keycode, true, 0 ) != 1 )
          {
            fprintf( stderr, "XTestFakeKeyEvent failed\n" );
            return NULL;
          }

          XUnlockDisplay( xzoe_display );
        }
      }
    }
  }
  while( true );

  return NULL;
}

static int xzoe_init_kb_map( xzoe_kb_map_entry kb_map[ XZOE_NUM_KEYS ] )
{
  kb_map[ 0 ].row = 8;
  kb_map[ 0 ].col = 0;
  kb_map[ 0 ].down = false;
  kb_map[ 0 ].keycode = XKeysymToKeycode( xzoe_display, XK_Escape );
  kb_map[ 0 ].value = "on/off";

  kb_map[ 1 ].row = 3;
  kb_map[ 1 ].col = 3;
  kb_map[ 1 ].down = false;
  kb_map[ 1 ].keycode = XKeysymToKeycode( xzoe_display, XK_F1 );
  kb_map[ 1 ].value = "f1";

  kb_map[ 2 ].row = 3;
  kb_map[ 2 ].col = 2;
  kb_map[ 2 ].down = false;
  kb_map[ 2 ].keycode = XKeysymToKeycode( xzoe_display, XK_F2 );
  kb_map[ 2 ].value = "f2";

  kb_map[ 3 ].row = 7;
  kb_map[ 3 ].col = 2;
  kb_map[ 3 ].down = false;
  kb_map[ 3 ].keycode = XKeysymToKeycode( xzoe_display, XK_F3 );
  kb_map[ 3 ].value = "f3";

  kb_map[ 4 ].row = 6;
  kb_map[ 4 ].col = 2;
  kb_map[ 4 ].down = false;
  kb_map[ 4 ].keycode = XKeysymToKeycode( xzoe_display, XK_F4 );
  kb_map[ 4 ].value = "f4";

  kb_map[ 5 ].row = 6;
  kb_map[ 5 ].col = 1;
  kb_map[ 5 ].down = false;
  kb_map[ 5 ].keycode = XKeysymToKeycode( xzoe_display, XK_F5 );
  kb_map[ 5 ].value = "f5";

  kb_map[ 6 ].row = 6;
  kb_map[ 6 ].col = 8;
  kb_map[ 6 ].down = false;
  kb_map[ 6 ].keycode = XKeysymToKeycode( xzoe_display, XK_F6 );
  kb_map[ 6 ].value = "f6";

  kb_map[ 7 ].row = 7;
  kb_map[ 7 ].col = 10;
  kb_map[ 7 ].down = false;
  kb_map[ 7 ].keycode = XKeysymToKeycode( xzoe_display, XK_F7 );
  kb_map[ 7 ].value = "f7";

  kb_map[ 8 ].row = 3;
  kb_map[ 8 ].col = 10;
  kb_map[ 8 ].down = false;
  kb_map[ 8 ].keycode = XKeysymToKeycode( xzoe_display, XK_F8 );
  kb_map[ 8 ].value = "f8";

  kb_map[ 9 ].row = 3;
  kb_map[ 9 ].col = 1;
  kb_map[ 9 ].down = false;
  kb_map[ 9 ].keycode = XKeysymToKeycode( xzoe_display, XK_F9 );
  kb_map[ 9 ].value = "date/new";

  kb_map[ 10 ].row = 2;
  kb_map[ 10 ].col = 1;
  kb_map[ 10 ].down = false;
  kb_map[ 10 ].keycode = XKeysymToKeycode( xzoe_display, XK_F10 );
  kb_map[ 10 ].value = "address/cancel";

  kb_map[ 11 ].row = 1;
  kb_map[ 11 ].col = 15;
  kb_map[ 11 ].down = false;
  kb_map[ 11 ].keycode = XKeysymToKeycode( xzoe_display, XK_F11 );
  kb_map[ 11 ].value = "to do/done";

  kb_map[ 12 ].row = 2;
  kb_map[ 12 ].col = 12;
  kb_map[ 12 ].down = false;
  kb_map[ 12 ].keycode = XKeysymToKeycode( xzoe_display, XK_F12 );
  kb_map[ 12 ].value = "memo/note";

  kb_map[ 13 ].row = 4;
  kb_map[ 13 ].col = 12;
  kb_map[ 13 ].down = false;
  kb_map[ 13 ].keycode = XKeysymToKeycode( xzoe_display, XK_Pause );
  kb_map[ 13 ].value = "find/contrast";

  kb_map[ 14 ].row = 1;
  kb_map[ 14 ].col = 13;
  kb_map[ 14 ].down = false;
  kb_map[ 14 ].keycode = XKeysymToKeycode( xzoe_display, XK_Sys_Req );
  kb_map[ 14 ].value = "menu/calc";

  kb_map[ 15 ].row = 3;
  kb_map[ 15 ].col = 14;
  kb_map[ 15 ].down = false;
  kb_map[ 15 ].keycode = XKeysymToKeycode( xzoe_display, XK_Insert );
  kb_map[ 15 ].value = "apps";

  kb_map[ 16 ].row = 3;
  kb_map[ 16 ].col = 15;
  kb_map[ 16 ].down = false;
  kb_map[ 16 ].keycode = XKeysymToKeycode( xzoe_display, XK_Delete );
  kb_map[ 16 ].value = "send/sync";

  kb_map[ 17 ].row = 3;
  kb_map[ 17 ].col = 4;
  kb_map[ 17 ].down = false;
  kb_map[ 17 ].keycode = XKeysymToKeycode( xzoe_display, XK_asciitilde );
  kb_map[ 17 ].value = "`";

  kb_map[ 18 ].row = 2;
  kb_map[ 18 ].col = 4;
  kb_map[ 18 ].down = false;
  kb_map[ 18 ].keycode = XKeysymToKeycode( xzoe_display, XK_1 );
  kb_map[ 18 ].value = "1";

  kb_map[ 19 ].row = 2;
  kb_map[ 19 ].col = 3;
  kb_map[ 19 ].down = false;
  kb_map[ 19 ].keycode = XKeysymToKeycode( xzoe_display, XK_2 );
  kb_map[ 19 ].value = "2";

  kb_map[ 20 ].row = 2;
  kb_map[ 20 ].col = 2;
  kb_map[ 20 ].down = false;
  kb_map[ 20 ].keycode = XKeysymToKeycode( xzoe_display, XK_3 );
  kb_map[ 20 ].value = "3";

  kb_map[ 21 ].row = 2;
  kb_map[ 21 ].col = 5;
  kb_map[ 21 ].down = false;
  kb_map[ 21 ].keycode = XKeysymToKeycode( xzoe_display, XK_4 );
  kb_map[ 21 ].value = "4";

  kb_map[ 22 ].row = 3;
  kb_map[ 22 ].col = 5;
  kb_map[ 22 ].down = false;
  kb_map[ 22 ].keycode = XKeysymToKeycode( xzoe_display, XK_5 );
  kb_map[ 22 ].value = "5";

  kb_map[ 23 ].row = 3;
  kb_map[ 23 ].col = 7;
  kb_map[ 23 ].down = false;
  kb_map[ 23 ].keycode = XKeysymToKeycode( xzoe_display, XK_6 );
  kb_map[ 23 ].value = "6";

  kb_map[ 24 ].row = 2;
  kb_map[ 24 ].col = 7;
  kb_map[ 24 ].down = false;
  kb_map[ 24 ].keycode = XKeysymToKeycode( xzoe_display, XK_7 );
  kb_map[ 24 ].value = "7";

  kb_map[ 25 ].row = 2;
  kb_map[ 25 ].col = 8;
  kb_map[ 25 ].down = false;
  kb_map[ 25 ].keycode = XKeysymToKeycode( xzoe_display, XK_8 );
  kb_map[ 25 ].value = "8";

  kb_map[ 26 ].row = 2;
  kb_map[ 26 ].col = 10;
  kb_map[ 26 ].down = false;
  kb_map[ 26 ].keycode = XKeysymToKeycode( xzoe_display, XK_9 );
  kb_map[ 26 ].value = "9";

  kb_map[ 27 ].row = 2;
  kb_map[ 27 ].col = 11;
  kb_map[ 27 ].down = false;
  kb_map[ 27 ].keycode = XKeysymToKeycode( xzoe_display, XK_0 );
  kb_map[ 27 ].value = "0";

  kb_map[ 28 ].row = 3;
  kb_map[ 28 ].col = 11;
  kb_map[ 28 ].down = false;
  kb_map[ 28 ].keycode = XKeysymToKeycode( xzoe_display, XK_minus );
  kb_map[ 28 ].value = "-";

  kb_map[ 29 ].row = 3;
  kb_map[ 29 ].col = 8;
  kb_map[ 29 ].down = false;
  kb_map[ 29 ].keycode = XKeysymToKeycode( xzoe_display, XK_equal );
  kb_map[ 29 ].value = "=";

  kb_map[ 30 ].row = 7;
  kb_map[ 30 ].col = 1;
  kb_map[ 30 ].down = false;
  kb_map[ 30 ].keycode = XKeysymToKeycode( xzoe_display, XK_BackSpace );
  kb_map[ 30 ].value = "Backspace";

  kb_map[ 31 ].row = 7;
  kb_map[ 31 ].col = 4;
  kb_map[ 31 ].down = false;
  kb_map[ 31 ].keycode = XKeysymToKeycode( xzoe_display, XK_Tab );
  kb_map[ 31 ].value = "Tab";

  kb_map[ 32 ].row = 4;
  kb_map[ 32 ].col = 4;
  kb_map[ 32 ].down = false;
  kb_map[ 32 ].keycode = XKeysymToKeycode( xzoe_display, XK_Q );
  kb_map[ 32 ].value = "q";

  kb_map[ 33 ].row = 4;
  kb_map[ 33 ].col = 3;
  kb_map[ 33 ].down = false;
  kb_map[ 33 ].keycode = XKeysymToKeycode( xzoe_display, XK_W );
  kb_map[ 33 ].value = "w";

  kb_map[ 34 ].row = 4;
  kb_map[ 34 ].col = 2;
  kb_map[ 34 ].down = false;
  kb_map[ 34 ].keycode = XKeysymToKeycode( xzoe_display, XK_E );
  kb_map[ 34 ].value = "e";

  kb_map[ 35 ].row = 4;
  kb_map[ 35 ].col = 5;
  kb_map[ 35 ].down = false;
  kb_map[ 35 ].keycode = XKeysymToKeycode( xzoe_display, XK_R );
  kb_map[ 35 ].value = "r";

  kb_map[ 36 ].row = 7;
  kb_map[ 36 ].col = 5;
  kb_map[ 36 ].down = false;
  kb_map[ 36 ].keycode = XKeysymToKeycode( xzoe_display, XK_T );
  kb_map[ 36 ].value = "t";

  kb_map[ 37 ].row = 7;
  kb_map[ 37 ].col = 7;
  kb_map[ 37 ].down = false;
  kb_map[ 37 ].keycode = XKeysymToKeycode( xzoe_display, XK_Y );
  kb_map[ 37 ].value = "y";

  kb_map[ 38 ].row = 4;
  kb_map[ 38 ].col = 7;
  kb_map[ 38 ].down = false;
  kb_map[ 38 ].keycode = XKeysymToKeycode( xzoe_display, XK_U );
  kb_map[ 38 ].value = "u";

  kb_map[ 39 ].row = 4;
  kb_map[ 39 ].col = 8;
  kb_map[ 39 ].down = false;
  kb_map[ 39 ].keycode = XKeysymToKeycode( xzoe_display, XK_I );
  kb_map[ 39 ].value = "i";

  kb_map[ 40 ].row = 4;
  kb_map[ 40 ].col = 10;
  kb_map[ 40 ].down = false;
  kb_map[ 40 ].keycode = XKeysymToKeycode( xzoe_display, XK_O );
  kb_map[ 40 ].value = "o";

  kb_map[ 41 ].row = 4;
  kb_map[ 41 ].col = 11;
  kb_map[ 41 ].down = false;
  kb_map[ 41 ].keycode = XKeysymToKeycode( xzoe_display, XK_P );
  kb_map[ 41 ].value = "p";

  kb_map[ 42 ].row = 7;
  kb_map[ 42 ].col = 11;
  kb_map[ 42 ].down = false;
  kb_map[ 42 ].keycode = XKeysymToKeycode( xzoe_display, XK_bracketleft );
  kb_map[ 42 ].value = "[";

  kb_map[ 43 ].row = 7;
  kb_map[ 43 ].col = 8;
  kb_map[ 43 ].down = false;
  kb_map[ 43 ].keycode = XKeysymToKeycode( xzoe_display, XK_bracketright );
  kb_map[ 43 ].value = "]";

  kb_map[ 44 ].row = 5;
  kb_map[ 44 ].col = 1;
  kb_map[ 44 ].down = false;
  kb_map[ 44 ].keycode = XKeysymToKeycode( xzoe_display, XK_backslash );
  kb_map[ 44 ].value = "\\";

  kb_map[ 45 ].row = 7;
  kb_map[ 45 ].col = 3;
  kb_map[ 45 ].down = false;
  kb_map[ 45 ].keycode = XKeysymToKeycode( xzoe_display, XK_Caps_Lock );
  kb_map[ 45 ].value = "Caps lock";

  kb_map[ 46 ].row = 5;
  kb_map[ 46 ].col = 4;
  kb_map[ 46 ].down = false;
  kb_map[ 46 ].keycode = XKeysymToKeycode( xzoe_display, XK_A );
  kb_map[ 46 ].value = "a";

  kb_map[ 47 ].row = 5;
  kb_map[ 47 ].col = 3;
  kb_map[ 47 ].down = false;
  kb_map[ 47 ].keycode = XKeysymToKeycode( xzoe_display, XK_S );
  kb_map[ 47 ].value = "s";

  kb_map[ 48 ].row = 5;
  kb_map[ 48 ].col = 2;
  kb_map[ 48 ].down = false;
  kb_map[ 48 ].keycode = XKeysymToKeycode( xzoe_display, XK_D );
  kb_map[ 48 ].value = "d";

  kb_map[ 49 ].row = 5;
  kb_map[ 49 ].col = 5;
  kb_map[ 49 ].down = false;
  kb_map[ 49 ].keycode = XKeysymToKeycode( xzoe_display, XK_F );
  kb_map[ 49 ].value = "f";

  kb_map[ 50 ].row = 6;
  kb_map[ 50 ].col = 5;
  kb_map[ 50 ].down = false;
  kb_map[ 50 ].keycode = XKeysymToKeycode( xzoe_display, XK_G );
  kb_map[ 50 ].value = "g";

  kb_map[ 51 ].row = 6;
  kb_map[ 51 ].col = 7;
  kb_map[ 51 ].down = false;
  kb_map[ 51 ].keycode = XKeysymToKeycode( xzoe_display, XK_H );
  kb_map[ 51 ].value = "h";

  kb_map[ 52 ].row = 5;
  kb_map[ 52 ].col = 7;
  kb_map[ 52 ].down = false;
  kb_map[ 52 ].keycode = XKeysymToKeycode( xzoe_display, XK_J );
  kb_map[ 52 ].value = "j";

  kb_map[ 53 ].row = 5;
  kb_map[ 53 ].col = 8;
  kb_map[ 53 ].down = false;
  kb_map[ 53 ].keycode = XKeysymToKeycode( xzoe_display, XK_K );
  kb_map[ 53 ].value = "k";

  kb_map[ 54 ].row = 5;
  kb_map[ 54 ].col = 10;
  kb_map[ 54 ].down = false;
  kb_map[ 54 ].keycode = XKeysymToKeycode( xzoe_display, XK_L );
  kb_map[ 54 ].value = "l";

  kb_map[ 55 ].row = 5;
  kb_map[ 55 ].col = 11;
  kb_map[ 55 ].down = false;
  kb_map[ 55 ].keycode = XKeysymToKeycode( xzoe_display, XK_semicolon );
  kb_map[ 55 ].value = ";";

  kb_map[ 56 ].row = 6;
  kb_map[ 56 ].col = 11;
  kb_map[ 56 ].down = false;
  kb_map[ 56 ].keycode = XKeysymToKeycode( xzoe_display, XK_apostrophe );
  kb_map[ 56 ].value = "'";

  kb_map[ 57 ].row = 1;
  kb_map[ 57 ].col = 1;
  kb_map[ 57 ].down = false;
  kb_map[ 57 ].keycode = XKeysymToKeycode( xzoe_display, XK_Return );
  kb_map[ 57 ].value = "Enter";

  kb_map[ 58 ].row = 7;
  kb_map[ 58 ].col = 6;
  kb_map[ 58 ].down = false;
  kb_map[ 58 ].keycode = XKeysymToKeycode( xzoe_display, XK_Shift_L );
  kb_map[ 58 ].value = "Left Shift";

  kb_map[ 59 ].row = 1;
  kb_map[ 59 ].col = 4;
  kb_map[ 59 ].down = false;
  kb_map[ 59 ].keycode = XKeysymToKeycode( xzoe_display, XK_Z );
  kb_map[ 59 ].value = "z";

  kb_map[ 60 ].row = 1;
  kb_map[ 60 ].col = 3;
  kb_map[ 60 ].down = false;
  kb_map[ 60 ].keycode = XKeysymToKeycode( xzoe_display, XK_X );
  kb_map[ 60 ].value = "x";

  kb_map[ 61 ].row = 1;
  kb_map[ 61 ].col = 2;
  kb_map[ 61 ].down = false;
  kb_map[ 61 ].keycode = XKeysymToKeycode( xzoe_display, XK_C );
  kb_map[ 61 ].value = "c";

  kb_map[ 62 ].row = 1;
  kb_map[ 62 ].col = 5;
  kb_map[ 62 ].down = false;
  kb_map[ 62 ].keycode = XKeysymToKeycode( xzoe_display, XK_V );
  kb_map[ 62 ].value = "v";

  kb_map[ 63 ].row = 0;
  kb_map[ 63 ].col = 5;
  kb_map[ 63 ].down = false;
  kb_map[ 63 ].keycode = XKeysymToKeycode( xzoe_display, XK_B );
  kb_map[ 63 ].value = "b";

  kb_map[ 64 ].row = 0;
  kb_map[ 64 ].col = 7;
  kb_map[ 64 ].down = false;
  kb_map[ 64 ].keycode = XKeysymToKeycode( xzoe_display, XK_N );
  kb_map[ 64 ].value = "n";

  kb_map[ 65 ].row = 1;
  kb_map[ 65 ].col = 7;
  kb_map[ 65 ].down = false;
  kb_map[ 65 ].keycode = XKeysymToKeycode( xzoe_display, XK_M );
  kb_map[ 65 ].value = "m";

  kb_map[ 66 ].row = 1;
  kb_map[ 66 ].col = 8;
  kb_map[ 66 ].down = false;
  kb_map[ 66 ].keycode = XKeysymToKeycode( xzoe_display, XK_comma );
  kb_map[ 66 ].value = ",";

  kb_map[ 67 ].row = 1;
  kb_map[ 67 ].col = 10;
  kb_map[ 67 ].down = false;
  kb_map[ 67 ].keycode = XKeysymToKeycode( xzoe_display, XK_period );
  kb_map[ 67 ].value = ".";

  kb_map[ 68 ].row = 0;
  kb_map[ 68 ].col = 11;
  kb_map[ 68 ].down = false;
  kb_map[ 68 ].keycode = XKeysymToKeycode( xzoe_display, XK_slash );
  kb_map[ 68 ].value = "/";

  kb_map[ 69 ].row = 1;
  kb_map[ 69 ].col = 6;
  kb_map[ 69 ].down = false;
  kb_map[ 69 ].keycode = XKeysymToKeycode( xzoe_display, XK_Shift_R );
  kb_map[ 69 ].value = "Right Shift";

  kb_map[ 70 ].row = 0;
  kb_map[ 70 ].col = 4;
  kb_map[ 70 ].down = false;
  kb_map[ 70 ].keycode = XKeysymToKeycode( xzoe_display, XK_Control_L );
  kb_map[ 70 ].value = "Left Ctrl";

  kb_map[ 71 ].row = 3;
  kb_map[ 71 ].col = 9;
  kb_map[ 71 ].down = false;
  kb_map[ 71 ].keycode = XKeysymToKeycode( xzoe_display, XK_Alt_L );
  kb_map[ 71 ].value = "Left Alt";

  kb_map[ 72 ].row = 6;
  kb_map[ 72 ].col = 12;
  kb_map[ 72 ].down = false;
  kb_map[ 72 ].keycode = XKeysymToKeycode( xzoe_display, XK_Super_L );
  kb_map[ 72 ].value = "Left Super";

  kb_map[ 73 ].row = 0;
  kb_map[ 73 ].col = 1;
  kb_map[ 73 ].down = false;
  kb_map[ 73 ].keycode = XKeysymToKeycode( xzoe_display, XK_space );
  kb_map[ 73 ].value = "Space";

  kb_map[ 74 ].row = 0;
  kb_map[ 74 ].col = 12;
  kb_map[ 74 ].down = false;
  kb_map[ 74 ].keycode = XKeysymToKeycode( xzoe_display, XK_Alt_R );
  kb_map[ 74 ].value = "Esc";

  kb_map[ 75 ].row = 1;
  kb_map[ 75 ].col = 9;
  kb_map[ 75 ].down = false;
  kb_map[ 75 ].keycode = XKeysymToKeycode( xzoe_display, XK_Control_R );
  kb_map[ 75 ].value = "Del";

  kb_map[ 76 ].row = 0;
  kb_map[ 76 ].col = 15;
  kb_map[ 76 ].down = false;
  kb_map[ 76 ].keycode = XKeysymToKeycode( xzoe_display, XK_Up );
  kb_map[ 76 ].value = "Up";

  kb_map[ 77 ].row = 0;
  kb_map[ 77 ].col = 13;
  kb_map[ 77 ].down = false;
  kb_map[ 77 ].keycode = XKeysymToKeycode( xzoe_display, XK_Left );
  kb_map[ 77 ].value = "Left";

  kb_map[ 78 ].row = 6;
  kb_map[ 78 ].col = 13;
  kb_map[ 78 ].down = false;
  kb_map[ 78 ].keycode = XKeysymToKeycode( xzoe_display, XK_Down );
  kb_map[ 78 ].value = "Down";

  kb_map[ 79 ].row = 0;
  kb_map[ 79 ].col = 14;
  kb_map[ 79 ].down = false;
  kb_map[ 79 ].keycode = XKeysymToKeycode( xzoe_display, XK_Right );
  kb_map[ 79 ].value = "Right";

  return 0;
}

static bool xzoe_validate_kb_buffer( const pru_kb_buffer* buffer, xzoe_kb_map_entry kb_map[ XZOE_NUM_KEYS ] )
{
  bool down_map[ PRU_KB_NUM_ROWS ][ PRU_KB_NUM_COLS ];
  int i, j, k, l;

  for( j = 0; j < PRU_KB_NUM_ROWS; j++ )
  {
    for( i = 0; i < PRU_KB_NUM_COLS; i++ )
    {
      down_map[ j ][ i ] = false;
    }
  }

  for( i = 0; i < XZOE_NUM_KEYS; i++ )
  {
    if( buffer->data[ kb_map[ i ].row ][ kb_map[ i ].col ] == PRU_KB_DOWN )
    {
      down_map[ kb_map[ i ].row ][ kb_map[ i ].col ] = true;
    }
  }

  for( j = 0; j < PRU_KB_NUM_ROWS; j++ )
  {
    for( i = 0; i < PRU_KB_NUM_COLS; i++ )
    {
      if( down_map[ j ][ i ] )
      {
        for( l = j - 1; l >= 0; l-- )
        {
          if( down_map[ l ][ i ] )
          {
            for( k = i - 1; k >= 0; k-- )
            {
              if( down_map[ j ][ k ] && down_map[ l ][ k ] )
              {
                return false;
              }
            }
          }
        }
      }
    }
  }

  return true;
}

