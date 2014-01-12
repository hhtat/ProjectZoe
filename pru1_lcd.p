#define FR      r4
#define ROW     r5
#define COL     r6
#define INDEX   r7
#define EXTMEM  r8

#define CALLREG r29.w0

#define H_DATA      r30.b0
#define V_EIO1      r30.t8
#define VH_LP       r30.t9
#define H_XCK       r30.t10
#define VH_FR       r30.t11

#define C_PRUCFG      C4

#define R_PRU1_DRAM1  0x0

#define NUM_ROWS    160
#define NUM_COLS    70

.macro delay
.mparam reg, cycles
  MOV reg, cycles
  _delay:
  SUB reg, reg, 1
  QBNE _delay, reg, 0
.endm

.setcallreg CALLREG
.entrypoint _start
.origin 0

_start:

  LBCO r0, C_PRUCFG, 4, 4
  CLR r0, r0, 4
  SBCO r0, C_PRUCFG, 4, 4


  MOV r0, R_PRU1_DRAM1
  LBBO EXTMEM, r0, 0, 4


  MOV FR, 0


  _next_frame:

    MOV INDEX, 0


    MOV ROW, 0
    _next_row:

      QBNE _skip_ROW0_logic_1, ROW, 0
        SET V_EIO1
      _skip_ROW0_logic_1:


      delay r0, 500


      MOV COL, 0
      _next_col:

        LBBO r0.b0, EXTMEM, INDEX, 1
        
        
        AND r0.b1, r0.b0, 0x0f
        AND r0.b2, H_DATA, 0xf0
        OR H_DATA, r0.b2, r0.b1


        SET H_XCK
        delay r1, 10
        CLR H_XCK
        delay r1, 10


        LSR r0.b1, r0.b0, 4
        AND r0.b2, H_DATA, 0xf0
        OR H_DATA, r0.b2, r0.b1


        SET H_XCK
        delay r1, 10
        CLR H_XCK
        delay r1, 10


        ADD INDEX, INDEX, 1

      ADD COL, COL, 1
      QBNE _next_col, COL, NUM_COLS


      SET VH_LP
      delay r0, 20
      CLR VH_LP
      delay r0, 20


      QBNE _skip_ROW0_logic_2, ROW, 0
      
        CLR V_EIO1


        QBNE _FRn0_logic, FR, 0
          CLR VH_FR
        JMP _FRn0_logic_end
        _FRn0_logic:
          SET VH_FR
        _FRn0_logic_end:

      _skip_ROW0_logic_2:

    ADD ROW, ROW, 1
    QBNE _next_row, ROW, NUM_ROWS


    NOT FR, FR
  
  JMP _next_frame

JMP _start

