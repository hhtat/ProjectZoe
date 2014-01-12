#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "pru1_lcd.h"
#include "prussdrv.h"

int pru1_lcd_init( void )
{
  void* mapped_extmem;
  unsigned int extmem_address;

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

  if( prussdrv_map_extmem( &mapped_extmem ) )
  {
    printf( "prussdrv_map_extmem failed\n" );
    return -1;
  }

  extmem_address = prussdrv_get_phys_addr( mapped_extmem );

  if( prussdrv_pru_write_memory( PRUSS0_PRU1_DATARAM, 0, &extmem_address, 4 ) != 1 )
  {
    printf( "prussdrv_pru_write_memory failed\n" );
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
  void* mapped_extmem;

  if( prussdrv_map_extmem( &mapped_extmem ) )
  {
    printf( "prussdrv_map_extmem failed\n" );
    return -1;
  }

  memcpy( mapped_extmem, buffer, pru1_lcd_buffer_len );

  return 0;
}

