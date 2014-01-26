#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "prussdrv.h"

#include "pru.h"

#define EVTOUT0_SYSEVT          16
#define EVTOUT0_CHAN            2
#define EVTOUT0_HOSTINT         2
#define EVTOUT0_HOSTINT_MASK    ( 1 << EVTOUT0_HOSTINT )

int pru_init_kb( void );
int pru_init_lcd( void );

int pru_init( void )
{
  tpruss_intc_initdata intc_initdata =
    {
      .sysevts_enabled =
        {
          EVTOUT0_SYSEVT,
          -1
        },
      .sysevt_to_channel_map =
        {
          {
            .sysevt = EVTOUT0_SYSEVT,
            .channel = EVTOUT0_CHAN
          },
          {
            .sysevt = -1,
            .channel = -1
          }
        },
      .channel_to_host_map =
        {
          {
            .channel = EVTOUT0_CHAN,
            .host = EVTOUT0_HOSTINT
          },
          {
            .channel = -1,
            .host = -1
          }
        },
      .host_enable_bitmask =
        (
          EVTOUT0_HOSTINT_MASK
        )
    };

  if( prussdrv_init( ) )
  {
    printf( "prussdrv_init failed\n" );
    return -1;
  }

  if( prussdrv_open( PRU_EVTOUT_0 ) )
  {
    printf( "prussdrv_open failed\n" );
    return -1;
  }

  if( prussdrv_pruintc_init( &intc_initdata ) )
  {
    printf( "prussdrv_pruintc_init failed\n" );
    return -1;
  }

  if( pru_init_kb( ) )
  {
    return -1;
  }

  if( pru_init_lcd( ) )
  {
    return -1;
  }

  return 0;
}

int pru_init_kb( void )
{
  pru_kb_buffer buffer;

  memset( &buffer.data, 0, sizeof( buffer.data ) );

  if( prussdrv_pru_write_memory( PRUSS0_PRU0_DATARAM, 0, ( unsigned int* ) &buffer.data, sizeof( buffer.data ) ) != sizeof( buffer.data ) / 4 )
  {
    printf( "prussdrv_pru_write_memory failed\n" );
    return -1;
  }

  if( prussdrv_exec_program( 0, "./pru0_kb.bin" ) )
  {
    printf( "prussdrv_exec_program failed\n" );
    return -1;
  }

  return 0;
}

int pru_init_lcd( void )
{
  void* mapped_extmem;
  unsigned int extmem_address;

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

int pru_kb_wait( void )
{
  if( prussdrv_pru_wait_event( PRU_EVTOUT_0 ) <= 0 )
  {
    fprintf( stderr, "prussdrv_pru_wait_event failed\n" );
    return -1;
  }

  if( prussdrv_pru_clear_event( PRU_EVTOUT_0, EVTOUT0_SYSEVT ) )
  {
    fprintf( stderr, "prussdrv_pru_clear_event failed\n" );
    return -1;
  }

  return 0;
}

int pru_kb_get( pru_kb_buffer* buffer )
{
  void* mapped_dataram0;

  if( prussdrv_map_prumem( PRUSS0_PRU0_DATARAM, &mapped_dataram0 ) )
  {
    fprintf( stderr, "prussdrv_map_prumem failed\n" );
    return -1;
  }

  memcpy( buffer->data, mapped_dataram0, sizeof( buffer->data ) );

  return 0;
}

int pru_lcd_put( const pru_lcd_buffer* buffer )
{
  void* mapped_extmem;

  if( prussdrv_map_extmem( &mapped_extmem ) )
  {
    printf( "prussdrv_map_extmem failed\n" );
    return -1;
  }

  memcpy( mapped_extmem, buffer->data, sizeof( buffer->data ) );

  return 0;
}

