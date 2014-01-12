#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xdamage.h>

#include "pru1_lcd.h"

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
      uint8_t buffer[ PRU1_LCD_BUF_LEN ];
      int i, j, k;

      image = XGetImage( display, root_window, 0, 0, PRU1_LCD_NUM_COLS, PRU1_LCD_NUM_ROWS, AllPlanes, ZPixmap );

      for( j = 0, k = 0; j < PRU1_LCD_NUM_ROWS; j++ )
      {
        for( i = 0; i < PRU1_LCD_NUM_COLS; i += 8, k++ )
        {
          buffer[ k ] = ((image->data[ PRU1_LCD_NUM_COLS * j + i + 0 ] != 1) << 0) |
                        ((image->data[ PRU1_LCD_NUM_COLS * j + i + 1 ] != 1) << 1) |
                        ((image->data[ PRU1_LCD_NUM_COLS * j + i + 2 ] != 1) << 2) |
                        ((image->data[ PRU1_LCD_NUM_COLS * j + i + 3 ] != 1) << 3) |
                        ((image->data[ PRU1_LCD_NUM_COLS * j + i + 4 ] != 1) << 4) |
                        ((image->data[ PRU1_LCD_NUM_COLS * j + i + 5 ] != 1) << 5) |
                        ((image->data[ PRU1_LCD_NUM_COLS * j + i + 6 ] != 1) << 6) |
                        ((image->data[ PRU1_LCD_NUM_COLS * j + i + 7 ] != 1) << 7);
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

