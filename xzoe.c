#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

#include <X11/Xlib.h>
#include <X11/extensions/XTest.h>
#include <X11/extensions/Xdamage.h>

#include "xzoe.h"
#include "pru.h"

Display* xzoe_display;
Screen* xzoe_screen;
Window xzoe_root_window;

static int xzoe_xtest_event_base;
static int xzoe_xtest_error_base;
static int xzoe_xtest_major_version;
static int xzoe_xtest_minor_version;

static int xzoe_xdamage_event_base;
static int xzoe_xdamage_error_base;

static void* xzoe_xevent_thread_start( void* arg );

int main( void )
{
  pthread_t xevent_thread;
  pthread_t kb_thread;

  if( pru_init( ) )
  {
    fprintf( stderr, "pru_init failed\n" );
    return -1;
  }

  if( !XInitThreads( ) )
  {
    fprintf( stderr, "XInitThreads failed\n" );
  }

  xzoe_display = XOpenDisplay( NULL );
  if( xzoe_display == NULL )
  {
    fprintf( stderr, "XOpenDisplay failed\n" );
    return -1;
  }

  xzoe_screen = XDefaultScreenOfDisplay( xzoe_display );
  if( xzoe_screen == NULL )
  {
    fprintf( stderr, "XDefaultScreenOfDisplay failed\n" );
    return -1;
  }

  xzoe_root_window = XDefaultRootWindow( xzoe_display );

  if( !XTestQueryExtension( xzoe_display, &xzoe_xtest_event_base, &xzoe_xtest_error_base, &xzoe_xtest_major_version, &xzoe_xtest_minor_version ) )
  {
    fprintf( stderr, "XTestQueryExtension failed\n" );
  }

  if( !XDamageQueryExtension( xzoe_display, &xzoe_xdamage_event_base, &xzoe_xdamage_error_base ) )
  {
    fprintf( stderr, "XDamageQueryExtension failed\n" );
    return -1;
  }

  XDamageCreate( xzoe_display, xzoe_root_window, XDamageReportRawRectangles );

  if( pthread_create( &kb_thread, NULL, xzoe_kb_thread_start, NULL ) )
  {
    fprintf( stderr, "pthread_create failed\n" );
    return -1;
  }

  if( pthread_create( &xevent_thread, NULL, xzoe_xevent_thread_start, NULL ) )
  {
    fprintf( stderr, "pthread_create failed\n" );
    return -1;
  }

  pthread_exit( NULL );

  return 0;
}

static void* xzoe_xevent_thread_start( void* arg )
{
  bool damaged;
  XEvent event;

  do
  {
    damaged = false;

    XLockDisplay( xzoe_display );

    while( XPending( xzoe_display ) > 0 )
    {
      XNextEvent( xzoe_display, &event );

      if( event.type == xzoe_xdamage_event_base + XDamageNotify )
      {
        damaged = true;
      }
    }

    if( damaged )
    {
      if( xzoe_update_pru_lcd( ) )
      {
        fprintf( stderr, "xzoe_update_pru_lcd failed\n" );
        return NULL;
      }
    }

    XUnlockDisplay( xzoe_display );

    usleep( 20000 );
  }
  while( true );

  return NULL;
}

