
%ifdef _WITH_C_

%macro Save_C 0
    PUSH RBX
    PUSH R10
    PUSH R11
%endmacro

%macro Restore_C 0
     POP R11
     POP R10
     POP RBX
%endmacro

%endif


section .text
    extern malloc

    global bigadd, genbig, bigmul, bigi, bigsub, list2big, str2big

;
;   bignum_write_stack  Stack space read only.
;


bignum_write_stack:
    POP     RBX                     ; Get call
    MOV     RCX ,      [RSP]        ; Get counter
    XOR     RAX ,       RAX
bignum_write_stack_remove_zeroes:
    DEC     RCX
    ADD     RSP ,         8
   TEST     RCX ,       0xFFFFFFFF
     JZ                 bignum_write_stack_remove_zeroes_finished
    CMP    [RSP],       RAX
     JZ                 bignum_write_stack_remove_zeroes
bignum_write_stack_remove_zeroes_finished:

    INC     RCX
   PUSH     RCX
   PUSH     RBX


    MOV     RDI ,      [RSP+8]        ; Get the counter
    INC     RDI                     ; Prepare the actual size.
    SHL     RDI ,         3
   CALL                 malloc      ; Create the space.

    POP     RBX                     ; Save the CALL stack element.

    
    POP     RCX                     ; Get the counter again, and remove
                                    ; it from the stack.
    MOV    [RAX],       RCX         ; Write the counter to new space.

    SHL     RCX ,         3         ; Move to the last element.
    ADD     RAX ,       RCX
    SHR     RCX ,         3

bignum_write_stack_loop:
    POP     RDX                     ; Get high order 
    MOV    [RAX],       RDX         ; Write to new space
    SUB     RAX ,         8         ; Move to low order element.
   LOOP                 bignum_write_stack_loop
   PUSH     RBX                     ; Reset CALL stack element.
    RET

;
;   int bigtest( void *, void *)
;
;   RAX = bigtest( RDI, RSI )
;
;
;   Returns 0x0000   A == B         AH == AL
;           0x0001   A  < B         AH  < AL
;           0x0100   A >  B         AH >  AL
;
;
;   Registers set for assembly calls.
;
;   R9  =  |B|
;   

bigtest:                
    MOV     RCX ,       [RDI]
    MOV      R9 ,       [RSI]

    MOV      AX ,        0x0100             ; Init  A > B
    CMP     RCX ,         R9
     JG                  bigtest_ret        ; A >  B return
     JL                  bigtest_lessthan   ; A  < B

                                            ; A == B ?
;    Since R9 == RCX, leave RCX alone

    SHL      R9 ,          3                ; Move to high orders
    ADD     RDI ,         R9                ; of A and B
    ADD     RSI ,         R9
    SHR      R9 ,          3                

bigtest_loop:
    MOV     RDX ,       [RDI]               ; Get A
    CMP     RDX ,       [RSI]               ; A == B ?
     JG                 bigtest_ret
     JL                 bigtest_lessthan
    SUB     RDI ,          8
    SUB     RSI ,          8
   LOOP                 bigtest_loop
    MOV      AX ,        0x0000             ; A == B
    JMP                  bigtest_ret
bigtest_lessthan:
	SHL      AX ,          8
;    MOV      AX ,        0x0001
bigtest_ret:
    RET

; ==============================================================================
;
;           str2big rewrite
;
; ==============================================================================
;
;   void * str2big( char * )
;
;   RAX =  str2big( RDI    )
;
;
;   RBP : RSP       RBP low order, RSI high order.
;
;   RCX             Empty register for PUSH
;
;   RBX : BL        Char to ul 
;
;    R8             looping register

str2big:
   Save_C
    PUSH    RBP                             ; Save RBP

     MOV    RBP ,       RSP
   
     XOR    RAX ,       RAX                 ; Empty RAX on entry
     MOV    RCX ,        10                 ; Set RCX to multiplier
     XOR    RBX ,       RBX

    PUSH    RAX                             ; Put RAX immediately onto stack.

str2big_loop:
     
    MOV          BL ,      [RDI]             ; Get the byte first.                            
   TEST          BL ,       0xFF            
     JZ                     str2big_ret 
    SUB          BL ,       0x30
    ADD         RDI ,          1             ; Move to next byte.

    MOV          R8 ,       RBP             ; init low order 
    SUB          R8 ,         8             
   
str2big_do_mul:
    MOV         RAX ,      [ R8]            ; RAX = low order  
    MUL         RCX
    ADD         RAX ,       RBX             ; Add low order.
    ADC         RDX ,         0
    MOV         RBX ,       RDX             ; RBX = high order
    MOV        [ R8],       RAX             ; Write RAX 
    SUB          R8 ,         8
    CMP          R8 ,       RSP
    JGE                     str2big_do_mul
    CMP         RDX ,         0
    JNZ                     str2big_push_high
    ADD          R8 ,         8             ; Reset R8
    JMP                     str2big_loop
str2big_push_high:
   PUSH         RDX
    JMP                     str2big_loop

str2big_ret:
    MOV         RCX ,       RBP
    SUB         RCX ,       RSP
    SHR         RCX ,         3
   PUSH         RCX
   CALL                     bignum_write_stack
    MOV         RSP ,       RBP
    POP         RBP
   Restore_C
    RET 
    
;                               BEGIN ARITHMETIC FUNCTIONS
;--------------------------------------------------------------------------------------------
;           
;
;   C = void * bigadd(void * A, void * B )
;
;  RAX = bigadd( RDI, RSI )
;
;

bigadd:

   Save_C

    MOV      R8 ,       [RDI]           ; Prepare counters
    MOV      R9 ,       [RSI]
    
    ADD     RDI ,          8            ; Move to low order element.
    ADD     RSI ,          8      

    XOR     RAX ,        RAX            ; Empty RAX
    XOR     RCX ,        RCX            ; Empty counter

bigadd_loop:
    XOR     RDX ,        RDX            ; Empty high order

                     ; Work on A      ----------------------------------------

   TEST      R8 ,        0xFFFFFFFF     ; Are there more elements for A
     JZ                  bigadd_no_more_A
    ADD     RAX ,       [RDI]

    ADD     RDI ,          8            ; Move to next element.
    DEC      R8                         ; Dec A's counter

                    ; Work on B       ----------------------------------------
bigadd_no_more_A:
   TEST      R9 ,        0xFFFFFFFF     ; Are there more elements for B
     JZ                  bigadd_no_more_B
    ADD     RAX ,       [RSI]           ; Add the two integers.
    ADC     RDX ,          0            ; Add the carry to high order.
    
    ADD     RSI ,          8            ; Move to next B's element.
    DEC      R9                         ; Dec B's counter

bigadd_no_more_B:
    INC     RCX                         ; Inc C's counter
   PUSH     RAX                         ; Move low order to stack
    MOV     RAX ,       RDX             ; Move high order to RAX

    ADD      R8 ,        R9 
   TEST      R8 ,       0xFFFFFFFF 
     JZ                 bigadd_exit

    SUB      R8 ,        R9
    JMP                 bigadd_loop
bigadd_exit:
    CMP     RAX ,         0             ; Is there a high order element left over?
     JZ                 bigadd_ret 

   PUSH     RAX                         ; Move high order to stack.
    INC     RCX

bigadd_ret:
   PUSH     RCX                         ; Push RCX onto the stack.
   CALL                 bignum_write_stack
   Restore_C
    RET

;
;
;   C = void *bigmul( void *, void * )
;
;   RAX = bigmul ( RDI, RSI )
;
;   Registers...
;
;   R8,  R9 = A and B's counters.
;  R10, R11 = A and B's counter backups shifted left by 3 bits.
;
;  RBX       End of the new variable.
;  RBP       Start of the new variable.


bigmul:
   Save_C
   PUSH           RBP                         ; Save RBP
    MOV           RBP ,       RSP             ; RBP now is the beginning of the 
                                             ; newly created variable.
               
    MOV           R8 ,      [RDI]            ; Setup counters.
    MOV           R9 ,      [RSI]
               
    MOV          R10 ,       RDI             ; Backup A,B
    MOV          R11 ,       RSI
               
    XOR          RAX ,       RAX             ; Write 0's to stack.
               
    MOV          RCX ,        R8
    ADD          RCX ,        R9
               
    INC          RCX                         ; Extra space for ADC
bigmul_init_stack:                       ; initialize the stack space.
   PUSH          RAX
   LOOP                      bigmul_init_stack
    

    MOV          RBX ,       RSP             ; Backup stack position.

    ADD          RSI ,         8             ; A[1]
    ADD          RDI ,         8             ; B[1]
                 
bigmul_loop:

    MOV          RAX ,      [RDI]            ; Setup A for multiplication.
    MUL QWORD   [RSI]
             
    ADD           R8 ,        R9             ; Setup position of current element.
    SHL           R8 ,         3
    ADD          RSP ,        R8
    SHR           R8 ,         3
    SUB           R8 ,        R9

    ADD         [RSP],       RAX
    ADC QWORD    RDX ,         0

    MOV          RAX ,         1            ; Set RAX to 1 for adc.
    SUB          RSP ,         8
    ADD         [RSP],       RDX
    JNC                      bigmul_no_adc

bigmul_adc:
    SUB          RSP ,         8
    ADD         [RSP],       RAX
     JC                      bigmul_adc 
bigmul_no_adc:

    MOV          RSP ,       RBX            ; Reset stack position.

    DEC           R8
    ADD          RDI ,         8
   TEST           R8 ,       0xFFFFFFFF
    JNZ                      bigmul_loop

    DEC           R9
    ADD          RSI ,         8
   TEST           R9 ,       0xFFFFFFFF
     JZ                      bigmul_exit

    MOV          RDI ,       R10
    MOV           R8 ,      [RDI]
    ADD          RDI ,         8
    JMP                      bigmul_loop

bigmul_exit:
    MOV          RCX ,       RBP            ; Get the length of the number.
    SUB          RCX ,       RSP
    SHR          RCX ,         3

    XOR          RDX ,       RDX
    NOT          RDX
bigmul_remove_0s:
    POP          RAX                        ; Remove 0's.
    DEC          RCX
   TEST          RAX ,       RDX
    JNZ                      bigmul_ret
   TEST          RCX ,       0xFFFFFFFF
    JNZ                      bigmul_remove_0s

bigmul_ret:
   PUSH          RAX                        ; Ensure 0 is written back
    INC          RCX                        ; incase RDI*RSI = 0.
   PUSH          RCX

   CALL                      bignum_write_stack 
    MOV          RSP ,       RBP
    POP          RBP
   Restore_C
    RET
;   
;
;   C = void * bigsub( void * A, void * B )
;
;   RAX = bigsub ( RDI, RSI )
;
;


bigsub:
    Save_C

    MOV    R10 ,        RDI             ; Backup A, B
    MOV    R11 ,        RSI

   CALL                 bigtest         ; Test A, B

    MOV    RDI ,        R10             ; Restore A
    MOV    RSI ,        R11             ; Restore B 
    MOV     R8 ,       [RDI]            ; Setup counters
    MOV     R9 ,       [R11]
                                        ; |B| was setup in bigtest
    XOR    RDX ,        RDX
    XOR    RCX ,        RCX
    
    ADD    RDI ,          8             ; Move to low order
    ADD    RSI ,          8
 
    CMP     AH ,         AL
    JNL                 bigsub_loop 

    MOV    RDI ,        R10             ; Restore A
    MOV    RCX ,       [RDI]            ; Put A on the stack.
    ADD    RDI ,          8
bigsub_init_stack: 
    MOV    RAX ,       [RDI]
    ADD    RDI ,          8
   PUSH    RAX
   LOOP                 bigsub_init_stack ;  RDI = end of A
    MOV    RCX ,       [R10]            ; Move counter to RCX
    JMP                 bigsub_ret      ; Create and copy A

bigsub_loop:
    MOV    RAX ,        RDX             ; Set RAX = -1 or 0
    XOR    RDX ,        RDX
;                      Work on A      ----------------------------------------
   TEST     R8 ,         0xFFFFFFFF     ; Are there any more elements to A?
     JZ                 bigsub_ret
    DEC     R8
    ADD    RAX ,       [RDI]
    ADD    RDI ,          8

;                      Work on B      ----------------------------------------
   TEST     R9 ,         0xFFFFFFFF
     JZ                 bigsub_push_rax

    SUB    RAX ,       [RSI]
    JNC                 bigsub_inc_B
    
    MOV    RDX ,           0            ; Set high order to -1.
    DEC    RDX
bigsub_inc_B:
    DEC     R9
    ADD    RSI ,           8
bigsub_push_rax:
   PUSH    RAX

    INC    RCX                          ; Increment C's counter
    JMP                 bigsub_loop

    
bigsub_ret:
   PUSH   RCX
   CALL                 bignum_write_stack
   Restore_C
    RET 


