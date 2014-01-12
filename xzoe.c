#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xdamage.h>

#include "pru1_lcd.h"

static const int xzoe_width = 560;
static const int xzoe_height = 160;

int main( void )
{
  Display* display;

  int xdamage_event_base;
  int xdamage_error_base;

  Window root_window;

  if( pru1_lcd_init( ) )
  {
    fprintf( stderr, "pru1_lcd_init failed\n" );
    return -1;
  }

  display = XOpenDisplay( NULL );
  if( display == NULL )
  {
    fprintf( stderr, "XOpenDisplay failed\n" );
    return -1;
  }

  if( !XDamageQueryExtension( display, &xdamage_event_base, &xdamage_error_base ) )
  {
    fprintf( stderr, "XDamageQueryExtension failed\n" );
    return -1;
  }

  root_window = XDefaultRootWindow( display );

  XDamageCreate( display, root_window, XDamageReportRawRectangles );

  do
  {
    bool damaged = false;

    while( XPending( display ) > 0 )
    {
      XEvent event;

      XNextEvent( display, &event );

      if( event.type == xdamage_event_base + XDamageNotify )
      {
        damaged = true;
      }
    }

    if( damaged )
    {
      XImage* image;
      uint8_t buffer[ pru1_lcd_buffer_len ];
      int i, j, k;

      image = XGetImage( display, root_window, 0, 0, xzoe_width, xzoe_height, AllPlanes, ZPixmap );

      for( j = 0, k = 0; j < xzoe_height; j++ )
      {
        for( i = 0; i < xzoe_width; i += 8, k++ )
        {
          buffer[ k ] = ((image->data[ xzoe_width * j + i + 0 ] != 1) << 0) |
                        ((image->data[ xzoe_width * j + i + 1 ] != 1) << 1) |
                        ((image->data[ xzoe_width * j + i + 2 ] != 1) << 2) |
                        ((image->data[ xzoe_width * j + i + 3 ] != 1) << 3) |
                        ((image->data[ xzoe_width * j + i + 4 ] != 1) << 4) |
                        ((image->data[ xzoe_width * j + i + 5 ] != 1) << 5) |
                        ((image->data[ xzoe_width * j + i + 6 ] != 1) << 6) |
                        ((image->data[ xzoe_width * j + i + 7 ] != 1) << 7);
        }
      }

      XDestroyImage( image );

      if( pru1_lcd_send( buffer ) )
      {
        fprintf( stderr, "pru1_lcd_send failed\n" );
        return -1;
      }
    }

    usleep( 20000 );
  }
  while(1);

  return 0;
}

