#define PRU1_LCD_NUM_ROWS 160
#define PRU1_LCD_NUM_COLS 560

#define PRU1_LCD_BUF_LEN ( PRU1_LCD_NUM_ROWS * PRU1_LCD_NUM_COLS / 8 )

int pru1_lcd_init( void );
int pru1_lcd_send( void* buffer );

