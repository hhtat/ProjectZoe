#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "xzoe.h"
#include "pru.h"

static int xzoe_init_lcd_slice_map( int slice_map[ PRU_LCD_NUM_SLICES ][ XZOE_NUM_COLORS ] );

int xzoe_update_pru_lcd( void )
{
  int slice_map[ PRU_LCD_NUM_SLICES ][ XZOE_NUM_COLORS ];
  XImage* image;
  pru_lcd_buffer buffer;
  int i, j;

  if( xzoe_init_lcd_slice_map( slice_map ) )
  {
    fprintf( stderr, "xzoe_init_lcd_slice_map failed\n" );
    return -1;
  }

  image = XGetImage( xzoe_display, xzoe_root_window, 0, 0, PRU_LCD_NUM_COLS, PRU_LCD_NUM_ROWS, AllPlanes, ZPixmap );
  if( image == NULL )
  {
    fprintf( stderr, "XGetImage failed\n" );
    return -1;
  }

  for( i = 0; i < PRU_LCD_NUM_COLS * PRU_LCD_NUM_ROWS; i += 8 )
  {
    for( j = 0; j < PRU_LCD_NUM_SLICES; j++ )
    {
      buffer.data[ j ][ i / 8 ] = ( slice_map[ j ][ ( size_t ) image->data[ i + 0 ] ] << 0 ) |
                                  ( slice_map[ j ][ ( size_t ) image->data[ i + 1 ] ] << 1 ) |
                                  ( slice_map[ j ][ ( size_t ) image->data[ i + 2 ] ] << 2 ) |
                                  ( slice_map[ j ][ ( size_t ) image->data[ i + 3 ] ] << 3 ) |
                                  ( slice_map[ j ][ ( size_t ) image->data[ i + 4 ] ] << 4 ) |
                                  ( slice_map[ j ][ ( size_t ) image->data[ i + 5 ] ] << 5 ) |
                                  ( slice_map[ j ][ ( size_t ) image->data[ i + 6 ] ] << 6 ) |
                                  ( slice_map[ j ][ ( size_t ) image->data[ i + 7 ] ] << 7 );
    }
  }

  XDestroyImage( image );

  if( pru_lcd_put( &buffer ) )
  {
    fprintf( stderr, "pru_lcd_put failed\n" );
    return -1;
  }

  return 0;
}

int xzoe_init_lcd_slice_map( int slice_map[ PRU_LCD_NUM_SLICES ][ XZOE_NUM_COLORS ] )
{
  Colormap colormap;
  XColor colors[ XZOE_NUM_COLORS ];
  int i;

  colormap = XDefaultColormapOfScreen( xzoe_screen );

  for( i = 0; i < XZOE_NUM_COLORS; i++ )
  {
    colors[ i ].pixel = i;
  }

  if( XQueryColors( xzoe_display, colormap, colors, XZOE_NUM_COLORS ) != 1 )
  {
    fprintf( stderr, "XQueryColors failed\n" );
    return -1;
  }

  for( i = 0; i < XZOE_NUM_COLORS; i++ )
  {
    switch( ( ( colors[ i ].red + colors[ i ].green + colors[ i ].blue ) / 3 ) >> 12 )
    {
      case 15:
        slice_map[ 0 ][ i ] = 0;
        slice_map[ 1 ][ i ] = 0;
        slice_map[ 2 ][ i ] = 0;
        slice_map[ 3 ][ i ] = 0;
        slice_map[ 4 ][ i ] = 0;
        slice_map[ 5 ][ i ] = 0;
        slice_map[ 6 ][ i ] = 0;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 0;
        slice_map[ 9 ][ i ] = 0;
        slice_map[ 10 ][ i ] = 0;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 0;
        slice_map[ 13 ][ i ] = 0;
        slice_map[ 14 ][ i ] = 0;
        slice_map[ 15 ][ i ] = 0;
        break;
//      case 14:
//        slice_map[ 0 ][ i ] = 1;
//        slice_map[ 1 ][ i ] = 0;
//        slice_map[ 2 ][ i ] = 0;
//        slice_map[ 3 ][ i ] = 0;
//        slice_map[ 4 ][ i ] = 0;
//        slice_map[ 5 ][ i ] = 0;
//        slice_map[ 6 ][ i ] = 0;
//        slice_map[ 7 ][ i ] = 0;
//        slice_map[ 8 ][ i ] = 0;
//        slice_map[ 9 ][ i ] = 0;
//        slice_map[ 10 ][ i ] = 0;
//        slice_map[ 11 ][ i ] = 0;
//        slice_map[ 12 ][ i ] = 0;
//        slice_map[ 13 ][ i ] = 0;
//        slice_map[ 14 ][ i ] = 0;
//        slice_map[ 15 ][ i ] = 0;
//        break;
      case 14:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 0;
        slice_map[ 2 ][ i ] = 0;
        slice_map[ 3 ][ i ] = 0;
        slice_map[ 4 ][ i ] = 0;
        slice_map[ 5 ][ i ] = 0;
        slice_map[ 6 ][ i ] = 0;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 0;
        slice_map[ 10 ][ i ] = 0;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 0;
        slice_map[ 13 ][ i ] = 0;
        slice_map[ 14 ][ i ] = 0;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 13:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 0;
        slice_map[ 2 ][ i ] = 0;
        slice_map[ 3 ][ i ] = 0;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 0;
        slice_map[ 6 ][ i ] = 0;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 0;
        slice_map[ 10 ][ i ] = 0;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 0;
        slice_map[ 13 ][ i ] = 0;
        slice_map[ 14 ][ i ] = 0;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 12:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 0;
        slice_map[ 2 ][ i ] = 0;
        slice_map[ 3 ][ i ] = 0;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 0;
        slice_map[ 6 ][ i ] = 0;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 0;
        slice_map[ 10 ][ i ] = 0;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 0;
        slice_map[ 14 ][ i ] = 0;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 11:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 0;
        slice_map[ 2 ][ i ] = 1;
        slice_map[ 3 ][ i ] = 0;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 0;
        slice_map[ 6 ][ i ] = 0;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 0;
        slice_map[ 10 ][ i ] = 0;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 0;
        slice_map[ 14 ][ i ] = 0;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 10:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 0;
        slice_map[ 2 ][ i ] = 1;
        slice_map[ 3 ][ i ] = 0;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 0;
        slice_map[ 6 ][ i ] = 0;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 0;
        slice_map[ 10 ][ i ] = 1;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 0;
        slice_map[ 14 ][ i ] = 0;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 9:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 0;
        slice_map[ 2 ][ i ] = 1;
        slice_map[ 3 ][ i ] = 0;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 0;
        slice_map[ 6 ][ i ] = 1;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 0;
        slice_map[ 10 ][ i ] = 1;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 0;
        slice_map[ 14 ][ i ] = 0;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 8:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 0;
        slice_map[ 2 ][ i ] = 1;
        slice_map[ 3 ][ i ] = 0;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 0;
        slice_map[ 6 ][ i ] = 1;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 0;
        slice_map[ 10 ][ i ] = 1;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 0;
        slice_map[ 14 ][ i ] = 1;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 7:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 1;
        slice_map[ 2 ][ i ] = 1;
        slice_map[ 3 ][ i ] = 0;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 0;
        slice_map[ 6 ][ i ] = 1;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 0;
        slice_map[ 10 ][ i ] = 1;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 0;
        slice_map[ 14 ][ i ] = 1;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 6:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 1;
        slice_map[ 2 ][ i ] = 1;
        slice_map[ 3 ][ i ] = 0;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 0;
        slice_map[ 6 ][ i ] = 1;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 1;
        slice_map[ 10 ][ i ] = 1;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 0;
        slice_map[ 14 ][ i ] = 1;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 5:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 1;
        slice_map[ 2 ][ i ] = 1;
        slice_map[ 3 ][ i ] = 0;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 1;
        slice_map[ 6 ][ i ] = 1;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 1;
        slice_map[ 10 ][ i ] = 1;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 0;
        slice_map[ 14 ][ i ] = 1;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 4:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 1;
        slice_map[ 2 ][ i ] = 1;
        slice_map[ 3 ][ i ] = 0;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 1;
        slice_map[ 6 ][ i ] = 1;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 1;
        slice_map[ 10 ][ i ] = 1;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 1;
        slice_map[ 14 ][ i ] = 1;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 3:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 1;
        slice_map[ 2 ][ i ] = 1;
        slice_map[ 3 ][ i ] = 1;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 1;
        slice_map[ 6 ][ i ] = 1;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 1;
        slice_map[ 10 ][ i ] = 1;
        slice_map[ 11 ][ i ] = 0;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 1;
        slice_map[ 14 ][ i ] = 1;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 2:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 1;
        slice_map[ 2 ][ i ] = 1;
        slice_map[ 3 ][ i ] = 1;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 1;
        slice_map[ 6 ][ i ] = 1;
        slice_map[ 7 ][ i ] = 0;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 1;
        slice_map[ 10 ][ i ] = 1;
        slice_map[ 11 ][ i ] = 1;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 1;
        slice_map[ 14 ][ i ] = 1;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 1:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 1;
        slice_map[ 2 ][ i ] = 1;
        slice_map[ 3 ][ i ] = 1;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 1;
        slice_map[ 6 ][ i ] = 1;
        slice_map[ 7 ][ i ] = 1;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 1;
        slice_map[ 10 ][ i ] = 1;
        slice_map[ 11 ][ i ] = 1;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 1;
        slice_map[ 14 ][ i ] = 1;
        slice_map[ 15 ][ i ] = 0;
        break;
      case 0:
        slice_map[ 0 ][ i ] = 1;
        slice_map[ 1 ][ i ] = 1;
        slice_map[ 2 ][ i ] = 1;
        slice_map[ 3 ][ i ] = 1;
        slice_map[ 4 ][ i ] = 1;
        slice_map[ 5 ][ i ] = 1;
        slice_map[ 6 ][ i ] = 1;
        slice_map[ 7 ][ i ] = 1;
        slice_map[ 8 ][ i ] = 1;
        slice_map[ 9 ][ i ] = 1;
        slice_map[ 10 ][ i ] = 1;
        slice_map[ 11 ][ i ] = 1;
        slice_map[ 12 ][ i ] = 1;
        slice_map[ 13 ][ i ] = 1;
        slice_map[ 14 ][ i ] = 1;
        slice_map[ 15 ][ i ] = 1;
        break;
    }
  }

  return 0;
}

