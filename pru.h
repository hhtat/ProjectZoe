#define PRU_KB_NUM_ROWS           9
#define PRU_KB_NUM_COLS           16
#define PRU_KB_UP                 0
#define PRU_KB_DOWN               8

#define PRU_LCD_NUM_SLICES        16
#define PRU_LCD_NUM_ROWS          160
#define PRU_LCD_NUM_COLS          560
#define PRU_LCD_BUF_SLICE_LEN     ( PRU_LCD_NUM_ROWS * PRU_LCD_NUM_COLS / 8 )

typedef struct _pru_kb_buffer
{
  uint8_t data[ PRU_KB_NUM_ROWS ][ PRU_KB_NUM_COLS ];
}
pru_kb_buffer;

typedef struct _pru_lcd_buffer
{
  uint8_t data[ PRU_LCD_NUM_SLICES ][ PRU_LCD_BUF_SLICE_LEN ];
}
pru_lcd_buffer;

int pru_init( void );
int pru_kb_wait( void );
int pru_kb_get( pru_kb_buffer* buffer );
int pru_lcd_put( const pru_lcd_buffer* buffer );

