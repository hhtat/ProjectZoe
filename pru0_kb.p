#define ROW     r4
#define COL     r5
#define INDEX   r6
#define EVENT   r7

#define CALLREG r29.w0

#define SCLK    r30.t0
#define MOSI    r30.t1
#define MISO    r31.t2
#define SS      r30.t3
#define RST     r30.t5

#define INTCOUT r31

#define R_PRU0_DRAM0          0x0

#define INTCOUT_PULSE         0x20
#define EVTOUT0_SYSEVT        16
#define EVTOUT0_INTCOUT       ( INTCOUT_PULSE | ( EVTOUT0_SYSEVT - 16 ) )

#define NUM_ROWS              9
#define NUM_COLS              16
#define SATVAL_LOW            0
#define SATVAL_HIGH           8

#define IOX_OP_BASE           0x40
#define IOX_OP_WRITE          0x00
#define IOX_OP_READ           0x01

#define IOX_REG_IODIRA        0x00
#define IOX_REG_IODIRB        0x01
#define IOX_REG_IOCON         0x0A
#define IOX_REG_GPPUA         0x0C
#define IOX_REG_GPPUB         0x0D
#define IOX_REG_GPIOA         0x12
#define IOX_REG_GPIOB         0x13
#define IOX_REG_OLATA         0x14
#define IOX_REG_OLATB         0x15

#define IOX_BIT_IOCON_HAEN    0x08

#define IOX_ROW_ADDR          0x02
#define IOX_COL_ADDR          0x04

.macro delay
.mparam reg, n

  MOV reg, 0
  _loop:

  ADD reg, reg, 1
  QBNE _loop, reg, n

.endm

.macro spi_begin
.mparam reg1

  CLR SS
  delay reg1, 10

.endm

.macro spi_transfer_bit
.mparam reg1, op, reg2

  QBBS mosi_high, reg1, op

    CLR MOSI

  QBA mosi_end
  mosi_high:

    SET MOSI

  mosi_end:
  delay reg2, 10


  SET SCLK


  QBBS miso_high, MISO

    CLR reg1, op

  QBA miso_end
  miso_high:

    SET reg1, op

  miso_end:
  delay reg2, 10


  CLR SCLK

.endm

.macro spi_end
.mparam reg1

  delay reg1, 10
  SET SS
  delay reg1, 10

.endm

.macro spi_transfer
.mparam reg1, reg2, reg3, op

  MOV reg1, op

  MOV reg2, 7
  _next_bit:

    spi_transfer_bit reg1, reg2.b0, reg3

  SUB reg2, reg2, 1
  QBBC _next_bit, reg2, 31

.endm

.setcallreg CALLREG
.entrypoint _start
.origin 0

_start:

  SET SS
  CLR SCLK


  CLR RST
  delay r0, 10
  SET RST
  delay r0, 10


  spi_begin r1
  spi_transfer r0, r1, r2, IOX_OP_BASE | IOX_OP_WRITE
  spi_transfer r0, r1, r2, IOX_REG_IOCON
  spi_transfer r0, r1, r2, IOX_BIT_IOCON_HAEN
  spi_end r1


  spi_begin r1
  spi_transfer r0, r1, r2, IOX_OP_BASE | IOX_COL_ADDR | IOX_OP_WRITE
  spi_transfer r0, r1, r2, IOX_REG_GPPUA
  spi_transfer r0, r1, r2, 0xFF
  spi_end r1


  spi_begin r1
  spi_transfer r0, r1, r2, IOX_OP_BASE | IOX_COL_ADDR | IOX_OP_WRITE
  spi_transfer r0, r1, r2, IOX_REG_GPPUB
  spi_transfer r0, r1, r2, 0xFF
  spi_end r1


  _main_loop:

    MOV INDEX, R_PRU0_DRAM0


    MOV EVENT, 0


    MOV ROW, 0
    _next_row:

      JMP xio_scan_row
      xio_scan_row_ret:


      MOV COL, 0
      _next_col:

        LBBO r1, INDEX, 0, 1


        QBEQ _down, r1, SATVAL_HIGH
        QBEQ _up, r1, SATVAL_LOW


        QBBC _add, r0, COL
        QBA _sub


        _down:
          QBBC _increment_index, r0, COL
          QBA _sub


        _up:
          QBBS _increment_index, r0, COL
          QBA _add


        _sub:
          SUB r1, r1, 1
          QBEQ _event, r1, SATVAL_LOW
          QBA _store_col


        _add:
          ADD r1, r1, 1
          QBEQ _event, r1, SATVAL_HIGH
          QBA _store_col


        _event:
          ADD EVENT, EVENT, 1
        _store_col:
          SBBO r1, INDEX, 0, 1
        _increment_index:
          ADD INDEX, INDEX, 1

      ADD COL, COL, 1
      QBNE _next_col, COL, NUM_COLS

    ADD ROW, ROW, 1
    QBNE _next_row, ROW, NUM_ROWS


    QBEQ _main_loop_end, EVENT, 0
    MOV INTCOUT, EVTOUT0_INTCOUT

  _main_loop_end:
  JMP _main_loop


xio_scan_row:

  MOV r3, 1
  LSL r3, r3, ROW
  NOT r3, r3


  spi_begin r1
  spi_transfer r0, r1, r2, IOX_OP_BASE | IOX_ROW_ADDR | IOX_OP_WRITE
  spi_transfer r0, r1, r2, IOX_REG_IODIRA
  spi_transfer r0, r1, r2, r3.b0
  spi_end r1


  spi_begin r1
  spi_transfer r0, r1, r2, IOX_OP_BASE | IOX_ROW_ADDR | IOX_OP_WRITE
  spi_transfer r0, r1, r2, IOX_REG_IODIRB
  spi_transfer r0, r1, r2, r3.b1
  spi_end r1


  delay r1, 200


  spi_begin r1
  spi_transfer r0.b0, r1, r2, IOX_OP_BASE | IOX_COL_ADDR | IOX_OP_READ
  spi_transfer r0.b0, r1, r2, IOX_REG_GPIOA
  spi_transfer r0.b0, r1, r2,0
  spi_end r1


  spi_begin r1
  spi_transfer r0.b1, r1, r2, IOX_OP_BASE | IOX_COL_ADDR | IOX_OP_READ
  spi_transfer r0.b1, r1, r2, IOX_REG_GPIOB
  spi_transfer r0.b1, r1, r2, 0
  spi_end r1


  JMP xio_scan_row_ret

