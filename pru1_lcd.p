#define FR      r4
#define ROW     r5
#define COL     r6
#define INDEX   r7
#define _INDEX  r8
#define SLICE   r9
#define EXTMEM  r10

#define CALLREG r29.w0

#define H_DATA      r30.b0
#define V_EIO1      r30.t8
#define VH_LP       r30.t9
#define H_XCK       r30.t10
#define VH_FR       r30.t11

#define C_PRUCFG      C4

#define R_PRU1_DRAM1  0x0

#define NUM_SLICES  16
#define NUM_ROWS    160
#define NUM_COLS    35

.setcallreg CALLREG
.entrypoint _start
.origin 0

_start:

  LBCO r0, C_PRUCFG, 4, 4
  CLR r0, r0, 4
  SBCO r0, C_PRUCFG, 4, 4


  MOV r0, R_PRU1_DRAM1
  LBBO EXTMEM, r0, 0, 4


  _next_frame:

    MOV INDEX, 0


    MOV SLICE, 0
    _next_slice:

      MOV _INDEX, INDEX


      MOV FR, 0
      _next_fr:

        MOV INDEX, _INDEX


        MOV ROW, 0
        _next_row:

          QBNE _skip_ROW0_logic_1, ROW, 0
            SET V_EIO1
          _skip_ROW0_logic_1:


          MOV COL, 0
          _next_col:

            LBBO r0.w0, EXTMEM, INDEX, 2


            LSR r1.w0, r0.w0, 0
            MOV H_DATA, r1.b0


            SET H_XCK
            CLR H_XCK


            LSR r1.w0, r0.w0, 4
            MOV H_DATA, r1.b0


            SET H_XCK
            CLR H_XCK


            LSR r1.w0, r0.w0, 8
            MOV H_DATA, r1.b0


            SET H_XCK
            CLR H_XCK


            LSR r1.w0, r0.w0, 12
            MOV H_DATA, r1.b0


            SET H_XCK
            CLR H_XCK


            ADD INDEX, INDEX, 2

          ADD COL, COL, 1
          QBNE _next_col, COL, NUM_COLS


          SET VH_LP
          CLR VH_LP


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

      ADD FR, FR, 1
      QBNE _next_fr, FR, 2

    ADD SLICE, SLICE, 1
    QBNE _next_slice, SLICE, NUM_SLICES
  
  JMP _next_frame

JMP _start

