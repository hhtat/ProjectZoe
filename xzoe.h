#define XZOE_NUM_KEYS 80
#define XZOE_NUM_COLORS 256

typedef struct _xzoe_kb_map_entry
{
  int row;
  int col;
  bool down;
  KeyCode keycode;
  char* value;
}
xzoe_kb_map_entry;

extern Display* xzoe_display;
extern Screen* xzoe_screen;
extern Window xzoe_root_window;

void* xzoe_kb_thread_start( void* arg );
int xzoe_update_pru_lcd( void );

