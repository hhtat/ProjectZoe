#define FR      r4
#define ROW     r5
#define INDEX   r6
#define _INDEX  r7
#define SLICE   r8
#define EXTMEM  r9

#define BUF_0   r10
#define BUF_1   r11
#define BUF_2   r12
#define BUF_3   r13
#define BUF_4   r14
#define BUF_5   r15
#define BUF_6   r16
#define BUF_7   r17
#define BUF_8   r18
#define BUF_9   r19
#define BUF_10  r20
#define BUF_11  r21
#define BUF_12  r22
#define BUF_13  r23
#define BUF_14  r24
#define BUF_15  r25
#define BUF_16  r26
#define BUF_17  r27

#define CALLREG r29.w0

#define H_DATA      r30.b0
#define V_EIO1      r30.t8
#define VH_LP       r30.t9
#define H_XCK       r30.t10
#define VH_FR       r30.t11

#define C_PRUCFG      C4

#define R_PRU1_DRAM1  0x0

#define NUM_SLICES    16
#define NUM_ROWS      160
#define NUM_ROWBYTES  70

.macro pxl_out
.mparam reg1, reg2, op

  LSR reg1, reg2, op
  MOV H_DATA, reg1.b0


  SET H_XCK
  MOV reg1, reg1
  CLR H_XCK
  MOV reg1, reg1

.endm

.macro delay
.mparam reg, n

  MOV reg, 0
  _loop:

  ADD reg, reg, 1
  QBNE _loop, reg, n

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


          LBBO BUF_0, EXTMEM, INDEX, NUM_ROWBYTES
          ADD INDEX, INDEX, NUM_ROWBYTES


          JMP _row_out
          _row_out_ret:


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


          delay r0, 200

        ADD ROW, ROW, 1
        QBNE _next_row, ROW, NUM_ROWS

      ADD FR, FR, 1
      QBNE _next_fr, FR, 2

    ADD SLICE, SLICE, 1
    QBNE _next_slice, SLICE, NUM_SLICES
  
  JMP _next_frame




_row_out:

  pxl_out r0, BUF_0, 0
  pxl_out r0, BUF_0, 4
  pxl_out r0, BUF_0, 8
  pxl_out r0, BUF_0, 12
  pxl_out r0, BUF_0, 16
  pxl_out r0, BUF_0, 20
  pxl_out r0, BUF_0, 24
  pxl_out r0, BUF_0, 28


  pxl_out r0, BUF_1, 0
  pxl_out r0, BUF_1, 4
  pxl_out r0, BUF_1, 8
  pxl_out r0, BUF_1, 12
  pxl_out r0, BUF_1, 16
  pxl_out r0, BUF_1, 20
  pxl_out r0, BUF_1, 24
  pxl_out r0, BUF_1, 28


  pxl_out r0, BUF_2, 0
  pxl_out r0, BUF_2, 4
  pxl_out r0, BUF_2, 8
  pxl_out r0, BUF_2, 12
  pxl_out r0, BUF_2, 16
  pxl_out r0, BUF_2, 20
  pxl_out r0, BUF_2, 24
  pxl_out r0, BUF_2, 28


  pxl_out r0, BUF_3, 0
  pxl_out r0, BUF_3, 4
  pxl_out r0, BUF_3, 8
  pxl_out r0, BUF_3, 12
  pxl_out r0, BUF_3, 16
  pxl_out r0, BUF_3, 20
  pxl_out r0, BUF_3, 24
  pxl_out r0, BUF_3, 28


  pxl_out r0, BUF_4, 0
  pxl_out r0, BUF_4, 4
  pxl_out r0, BUF_4, 8
  pxl_out r0, BUF_4, 12
  pxl_out r0, BUF_4, 16
  pxl_out r0, BUF_4, 20
  pxl_out r0, BUF_4, 24
  pxl_out r0, BUF_4, 28


  pxl_out r0, BUF_5, 0
  pxl_out r0, BUF_5, 4
  pxl_out r0, BUF_5, 8
  pxl_out r0, BUF_5, 12
  pxl_out r0, BUF_5, 16
  pxl_out r0, BUF_5, 20
  pxl_out r0, BUF_5, 24
  pxl_out r0, BUF_5, 28


  pxl_out r0, BUF_6, 0
  pxl_out r0, BUF_6, 4
  pxl_out r0, BUF_6, 8
  pxl_out r0, BUF_6, 12
  pxl_out r0, BUF_6, 16
  pxl_out r0, BUF_6, 20
  pxl_out r0, BUF_6, 24
  pxl_out r0, BUF_6, 28


  pxl_out r0, BUF_7, 0
  pxl_out r0, BUF_7, 4
  pxl_out r0, BUF_7, 8
  pxl_out r0, BUF_7, 12
  pxl_out r0, BUF_7, 16
  pxl_out r0, BUF_7, 20
  pxl_out r0, BUF_7, 24
  pxl_out r0, BUF_7, 28


  pxl_out r0, BUF_8, 0
  pxl_out r0, BUF_8, 4
  pxl_out r0, BUF_8, 8
  pxl_out r0, BUF_8, 12
  pxl_out r0, BUF_8, 16
  pxl_out r0, BUF_8, 20
  pxl_out r0, BUF_8, 24
  pxl_out r0, BUF_8, 28


  pxl_out r0, BUF_9, 0
  pxl_out r0, BUF_9, 4
  pxl_out r0, BUF_9, 8
  pxl_out r0, BUF_9, 12
  pxl_out r0, BUF_9, 16
  pxl_out r0, BUF_9, 20
  pxl_out r0, BUF_9, 24
  pxl_out r0, BUF_9, 28


  pxl_out r0, BUF_10, 0
  pxl_out r0, BUF_10, 4
  pxl_out r0, BUF_10, 8
  pxl_out r0, BUF_10, 12
  pxl_out r0, BUF_10, 16
  pxl_out r0, BUF_10, 20
  pxl_out r0, BUF_10, 24
  pxl_out r0, BUF_10, 28


  pxl_out r0, BUF_11, 0
  pxl_out r0, BUF_11, 4
  pxl_out r0, BUF_11, 8
  pxl_out r0, BUF_11, 12
  pxl_out r0, BUF_11, 16
  pxl_out r0, BUF_11, 20
  pxl_out r0, BUF_11, 24
  pxl_out r0, BUF_11, 28


  pxl_out r0, BUF_12, 0
  pxl_out r0, BUF_12, 4
  pxl_out r0, BUF_12, 8
  pxl_out r0, BUF_12, 12
  pxl_out r0, BUF_12, 16
  pxl_out r0, BUF_12, 20
  pxl_out r0, BUF_12, 24
  pxl_out r0, BUF_12, 28


  pxl_out r0, BUF_13, 0
  pxl_out r0, BUF_13, 4
  pxl_out r0, BUF_13, 8
  pxl_out r0, BUF_13, 12
  pxl_out r0, BUF_13, 16
  pxl_out r0, BUF_13, 20
  pxl_out r0, BUF_13, 24
  pxl_out r0, BUF_13, 28


  pxl_out r0, BUF_14, 0
  pxl_out r0, BUF_14, 4
  pxl_out r0, BUF_14, 8
  pxl_out r0, BUF_14, 12
  pxl_out r0, BUF_14, 16
  pxl_out r0, BUF_14, 20
  pxl_out r0, BUF_14, 24
  pxl_out r0, BUF_14, 28


  pxl_out r0, BUF_15, 0
  pxl_out r0, BUF_15, 4
  pxl_out r0, BUF_15, 8
  pxl_out r0, BUF_15, 12
  pxl_out r0, BUF_15, 16
  pxl_out r0, BUF_15, 20
  pxl_out r0, BUF_15, 24
  pxl_out r0, BUF_15, 28


  pxl_out r0, BUF_16, 0
  pxl_out r0, BUF_16, 4
  pxl_out r0, BUF_16, 8
  pxl_out r0, BUF_16, 12
  pxl_out r0, BUF_16, 16
  pxl_out r0, BUF_16, 20
  pxl_out r0, BUF_16, 24
  pxl_out r0, BUF_16, 28


  pxl_out r0, BUF_17, 0
  pxl_out r0, BUF_17, 4
  pxl_out r0, BUF_17, 8
  pxl_out r0, BUF_17, 12
  pxl_out r0, BUF_17, 28

JMP _row_out_ret
