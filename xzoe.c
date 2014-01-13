#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <unistd.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xdamage.h>

#include "pru1_lcd.h"

#define XZOE_NUM_COLORS 256

static Display* xzoe_display;
static Screen* xzoe_screen;
static Window xzoe_root_window;

static int xzoe_xdamage_event_base;
static int xzoe_xdamage_error_base;

static int pixel_to_slice_map[ PRU1_LCD_NUM_SLICES ][ XZOE_NUM_COLORS ];

int xzoe_init_pixel_to_slice_map( void );
int xzoe_init_xdamage( void );
int xzoe_update_pru1_lcd( void );

int main( void )
{
  if( pru1_lcd_init( ) )
  {
    fprintf( stderr, "pru1_lcd_init failed\n" );
    return -1;
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

  if( xzoe_init_pixel_to_slice_map( ) )
  {
    return -1;
  }

  if( xzoe_init_xdamage( ) )
  {
    return -1;
  }

  do
  {
    bool damaged = false;

    while( XPending( xzoe_display ) > 0 )
    {
      XEvent event;

      XNextEvent( xzoe_display, &event );

      if( event.type == xzoe_xdamage_event_base + XDamageNotify )
      {
        damaged = true;
      }
    }

    if( damaged )
    {
      if( xzoe_update_pru1_lcd( ) )
      {
        return -1;
      }
    }

    usleep( 20000 );
  }
  while(1);

  return 0;
}

int xzoe_init_pixel_to_slice_map( void )
{
  Colormap colormap;
  XColor colors[ XZOE_NUM_COLORS ];
  int i;

  colormap = XDefaultColormapOfScreen( xzoe_screen );

  for( i = 0; i < XZOE_NUM_COLORS; i++ )
  {
    colors[ i ].pixel = i;
  }

  XQueryColors( xzoe_display, colormap, colors, XZOE_NUM_COLORS );

  for( i = 0; i < XZOE_NUM_COLORS; i++ )
  {
    switch( ( ( colors[ i ].red + colors[ i ].green + colors[ i ].blue ) / 3 ) >> 12 )
    {
      case 15:
        pixel_to_slice_map[ 0 ][ i ] = 0;
        pixel_to_slice_map[ 1 ][ i ] = 0;
        pixel_to_slice_map[ 2 ][ i ] = 0;
        pixel_to_slice_map[ 3 ][ i ] = 0;
        pixel_to_slice_map[ 4 ][ i ] = 0;
        pixel_to_slice_map[ 5 ][ i ] = 0;
        pixel_to_slice_map[ 6 ][ i ] = 0;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 0;
        pixel_to_slice_map[ 9 ][ i ] = 0;
        pixel_to_slice_map[ 10 ][ i ] = 0;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 0;
        pixel_to_slice_map[ 13 ][ i ] = 0;
        pixel_to_slice_map[ 14 ][ i ] = 0;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
//      case 14:
//        pixel_to_slice_map[ 0 ][ i ] = 1;
//        pixel_to_slice_map[ 1 ][ i ] = 0;
//        pixel_to_slice_map[ 2 ][ i ] = 0;
//        pixel_to_slice_map[ 3 ][ i ] = 0;
//        pixel_to_slice_map[ 4 ][ i ] = 0;
//        pixel_to_slice_map[ 5 ][ i ] = 0;
//        pixel_to_slice_map[ 6 ][ i ] = 0;
//        pixel_to_slice_map[ 7 ][ i ] = 0;
//        pixel_to_slice_map[ 8 ][ i ] = 0;
//        pixel_to_slice_map[ 9 ][ i ] = 0;
//        pixel_to_slice_map[ 10 ][ i ] = 0;
//        pixel_to_slice_map[ 11 ][ i ] = 0;
//        pixel_to_slice_map[ 12 ][ i ] = 0;
//        pixel_to_slice_map[ 13 ][ i ] = 0;
//        pixel_to_slice_map[ 14 ][ i ] = 0;
//        pixel_to_slice_map[ 15 ][ i ] = 0;
//        break;
      case 14:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 0;
        pixel_to_slice_map[ 2 ][ i ] = 0;
        pixel_to_slice_map[ 3 ][ i ] = 0;
        pixel_to_slice_map[ 4 ][ i ] = 0;
        pixel_to_slice_map[ 5 ][ i ] = 0;
        pixel_to_slice_map[ 6 ][ i ] = 0;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 0;
        pixel_to_slice_map[ 10 ][ i ] = 0;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 0;
        pixel_to_slice_map[ 13 ][ i ] = 0;
        pixel_to_slice_map[ 14 ][ i ] = 0;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 13:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 0;
        pixel_to_slice_map[ 2 ][ i ] = 0;
        pixel_to_slice_map[ 3 ][ i ] = 0;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 0;
        pixel_to_slice_map[ 6 ][ i ] = 0;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 0;
        pixel_to_slice_map[ 10 ][ i ] = 0;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 0;
        pixel_to_slice_map[ 13 ][ i ] = 0;
        pixel_to_slice_map[ 14 ][ i ] = 0;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 12:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 0;
        pixel_to_slice_map[ 2 ][ i ] = 0;
        pixel_to_slice_map[ 3 ][ i ] = 0;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 0;
        pixel_to_slice_map[ 6 ][ i ] = 0;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 0;
        pixel_to_slice_map[ 10 ][ i ] = 0;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 0;
        pixel_to_slice_map[ 14 ][ i ] = 0;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 11:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 0;
        pixel_to_slice_map[ 2 ][ i ] = 1;
        pixel_to_slice_map[ 3 ][ i ] = 0;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 0;
        pixel_to_slice_map[ 6 ][ i ] = 0;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 0;
        pixel_to_slice_map[ 10 ][ i ] = 0;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 0;
        pixel_to_slice_map[ 14 ][ i ] = 0;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 10:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 0;
        pixel_to_slice_map[ 2 ][ i ] = 1;
        pixel_to_slice_map[ 3 ][ i ] = 0;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 0;
        pixel_to_slice_map[ 6 ][ i ] = 0;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 0;
        pixel_to_slice_map[ 10 ][ i ] = 1;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 0;
        pixel_to_slice_map[ 14 ][ i ] = 0;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 9:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 0;
        pixel_to_slice_map[ 2 ][ i ] = 1;
        pixel_to_slice_map[ 3 ][ i ] = 0;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 0;
        pixel_to_slice_map[ 6 ][ i ] = 1;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 0;
        pixel_to_slice_map[ 10 ][ i ] = 1;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 0;
        pixel_to_slice_map[ 14 ][ i ] = 0;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 8:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 0;
        pixel_to_slice_map[ 2 ][ i ] = 1;
        pixel_to_slice_map[ 3 ][ i ] = 0;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 0;
        pixel_to_slice_map[ 6 ][ i ] = 1;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 0;
        pixel_to_slice_map[ 10 ][ i ] = 1;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 0;
        pixel_to_slice_map[ 14 ][ i ] = 1;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 7:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 1;
        pixel_to_slice_map[ 2 ][ i ] = 1;
        pixel_to_slice_map[ 3 ][ i ] = 0;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 0;
        pixel_to_slice_map[ 6 ][ i ] = 1;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 0;
        pixel_to_slice_map[ 10 ][ i ] = 1;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 0;
        pixel_to_slice_map[ 14 ][ i ] = 1;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 6:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 1;
        pixel_to_slice_map[ 2 ][ i ] = 1;
        pixel_to_slice_map[ 3 ][ i ] = 0;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 0;
        pixel_to_slice_map[ 6 ][ i ] = 1;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 1;
        pixel_to_slice_map[ 10 ][ i ] = 1;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 0;
        pixel_to_slice_map[ 14 ][ i ] = 1;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 5:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 1;
        pixel_to_slice_map[ 2 ][ i ] = 1;
        pixel_to_slice_map[ 3 ][ i ] = 0;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 1;
        pixel_to_slice_map[ 6 ][ i ] = 1;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 1;
        pixel_to_slice_map[ 10 ][ i ] = 1;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 0;
        pixel_to_slice_map[ 14 ][ i ] = 1;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 4:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 1;
        pixel_to_slice_map[ 2 ][ i ] = 1;
        pixel_to_slice_map[ 3 ][ i ] = 0;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 1;
        pixel_to_slice_map[ 6 ][ i ] = 1;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 1;
        pixel_to_slice_map[ 10 ][ i ] = 1;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 1;
        pixel_to_slice_map[ 14 ][ i ] = 1;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 3:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 1;
        pixel_to_slice_map[ 2 ][ i ] = 1;
        pixel_to_slice_map[ 3 ][ i ] = 1;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 1;
        pixel_to_slice_map[ 6 ][ i ] = 1;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 1;
        pixel_to_slice_map[ 10 ][ i ] = 1;
        pixel_to_slice_map[ 11 ][ i ] = 0;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 1;
        pixel_to_slice_map[ 14 ][ i ] = 1;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 2:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 1;
        pixel_to_slice_map[ 2 ][ i ] = 1;
        pixel_to_slice_map[ 3 ][ i ] = 1;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 1;
        pixel_to_slice_map[ 6 ][ i ] = 1;
        pixel_to_slice_map[ 7 ][ i ] = 0;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 1;
        pixel_to_slice_map[ 10 ][ i ] = 1;
        pixel_to_slice_map[ 11 ][ i ] = 1;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 1;
        pixel_to_slice_map[ 14 ][ i ] = 1;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 1:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 1;
        pixel_to_slice_map[ 2 ][ i ] = 1;
        pixel_to_slice_map[ 3 ][ i ] = 1;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 1;
        pixel_to_slice_map[ 6 ][ i ] = 1;
        pixel_to_slice_map[ 7 ][ i ] = 1;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 1;
        pixel_to_slice_map[ 10 ][ i ] = 1;
        pixel_to_slice_map[ 11 ][ i ] = 1;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 1;
        pixel_to_slice_map[ 14 ][ i ] = 1;
        pixel_to_slice_map[ 15 ][ i ] = 0;
        break;
      case 0:
        pixel_to_slice_map[ 0 ][ i ] = 1;
        pixel_to_slice_map[ 1 ][ i ] = 1;
        pixel_to_slice_map[ 2 ][ i ] = 1;
        pixel_to_slice_map[ 3 ][ i ] = 1;
        pixel_to_slice_map[ 4 ][ i ] = 1;
        pixel_to_slice_map[ 5 ][ i ] = 1;
        pixel_to_slice_map[ 6 ][ i ] = 1;
        pixel_to_slice_map[ 7 ][ i ] = 1;
        pixel_to_slice_map[ 8 ][ i ] = 1;
        pixel_to_slice_map[ 9 ][ i ] = 1;
        pixel_to_slice_map[ 10 ][ i ] = 1;
        pixel_to_slice_map[ 11 ][ i ] = 1;
        pixel_to_slice_map[ 12 ][ i ] = 1;
        pixel_to_slice_map[ 13 ][ i ] = 1;
        pixel_to_slice_map[ 14 ][ i ] = 1;
        pixel_to_slice_map[ 15 ][ i ] = 1;
        break;
    }
  }

  return 0;
}

int xzoe_init_xdamage( void )
{
  if( !XDamageQueryExtension( xzoe_display, &xzoe_xdamage_event_base, &xzoe_xdamage_error_base ) )
  {
    fprintf( stderr, "XDamageQueryExtension failed\n" );
    return -1;
  }

  XDamageCreate( xzoe_display, xzoe_root_window, XDamageReportRawRectangles );

  return 0;
}

int xzoe_update_pru1_lcd( void )
{
  XImage* image;
  uint8_t buffer[ PRU1_LCD_BUF_LEN ];
  int i, j, k, l;

  image = XGetImage( xzoe_display, xzoe_root_window, 0, 0, PRU1_LCD_NUM_COLS, PRU1_LCD_NUM_ROWS, AllPlanes, ZPixmap );

  for( k = 0, l = 0; k < PRU1_LCD_NUM_SLICES; k++ )
  {
    for( j = 0; j < PRU1_LCD_NUM_ROWS; j++ )
    {
      for( i = 0; i < PRU1_LCD_NUM_COLS; i += 8, l++ )
      {
        buffer[ l ] = ( pixel_to_slice_map[ k ][ ( size_t ) image->data[ PRU1_LCD_NUM_COLS * j + i + 0 ] ] << 0 ) |
                      ( pixel_to_slice_map[ k ][ ( size_t ) image->data[ PRU1_LCD_NUM_COLS * j + i + 1 ] ] << 1 ) |
                      ( pixel_to_slice_map[ k ][ ( size_t ) image->data[ PRU1_LCD_NUM_COLS * j + i + 2 ] ] << 2 ) |
                      ( pixel_to_slice_map[ k ][ ( size_t ) image->data[ PRU1_LCD_NUM_COLS * j + i + 3 ] ] << 3 ) |
                      ( pixel_to_slice_map[ k ][ ( size_t ) image->data[ PRU1_LCD_NUM_COLS * j + i + 4 ] ] << 4 ) |
                      ( pixel_to_slice_map[ k ][ ( size_t ) image->data[ PRU1_LCD_NUM_COLS * j + i + 5 ] ] << 5 ) |
                      ( pixel_to_slice_map[ k ][ ( size_t ) image->data[ PRU1_LCD_NUM_COLS * j + i + 6 ] ] << 6 ) |
                      ( pixel_to_slice_map[ k ][ ( size_t ) image->data[ PRU1_LCD_NUM_COLS * j + i + 7 ] ] << 7 );
      }
    }
  }

  XDestroyImage( image );

  if( pru1_lcd_send( buffer ) )
  {
    fprintf( stderr, "pru1_lcd_send failed\n" );
    return -1;
  }

  return 0;
}
