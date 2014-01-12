#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "pru1_lcd.h"
#include "prussdrv.h"

int pru1_lcd_init( void )
{
  if( prussdrv_init( ) )
  {
    printf( "prussdrv_init failed\n" );
    return -1;
  }

  if( prussdrv_open( 0 ) )
  {
    printf( "prussdrv_open failed\n" );
    return -1;
  }

  if( prussdrv_exec_program( 1, "./pru1_lcd.bin" ) )
  {
    printf( "prussdrv_exec_program failed\n" );
    return -1;
  }

  return 0;
}

int pru1_lcd_send( void* buffer )
{
  if( prussdrv_pru_write_memory(PRUSS0_SHARED_DATARAM, 0, buffer, PRU1_LCD_BUF_LEN ) != PRU1_LCD_BUF_LEN / 4 )
  {
    printf( "prussdrv_pru_write_memory failed\n" );
    return -1;
  }

  return 0;
}

