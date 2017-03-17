;;
;; C Run-time startup module for MPLAB XC16 C compiler.
;; (c) Copyright 2009 - 2016 Microchip Technology, All rights reserved
;;
;; Data initialization support.
;; The linker loads this version when the --data-init
;; option is selected.
;;

	.section .init,code
        .global __data_init_standard
;; 
;; Process data init template
;;
;; The template is null-terminated, with records
;; in the following format:
;;
;; struct data_record {
;;  char *dst;        /* destination address  */
;;  int  len;         /* length in bytes      */
;;  int  format:7;    /* format code          */
;;  int  page:9;      /* page                 */
;;  char dat[0];      /* variable length data */
;; };
;; 
;; Registers used:  w0 w1 w2 w3 w4 w5
;; 
;; Inputs:
;;  w0 <- tbloffset of initialization template
;;  w1 <- tblpage of initialization template
;; 
;; Outputs:
;;  (none)
;;
;; Calls:
;;  __memcpypd3
;; 
;;
;; standard version
;;
;;

#define ZERO      w0
#define TBLOFFSET w9
#define DSTOFFSET w10
#define LEN       w11
#define UPPERBYTE w12
#define FORMAT    w13
#define PAGE      w14


__long_indirect_call:
        push     DSTOFFSET                       ; this device has no
        push     LEN                             ; long call
        return

	.equ __data_init, __data_init_standard
	.global __data_init

__data_init_standard:

        ; for all format codes, hi-bit set means there is a DST page at
        ; data[0]
        .equiv   FMT_CLEAR,0    ;  format codes
        .equiv   FMT_COPY2,1    ; 
        .equiv   FMT_COPY3,2    ; 
	.equiv   FMT_CALL, 3
        .equiv   FMT_DUO_COPY3,0x1F ;  copy len instructions to slave

        mov       w1,_TBLPAG
	mov       w0,TBLOFFSET
        clr       ZERO
        bra      4f

1:      add      TBLOFFSET,#2,TBLOFFSET
        addc     _TBLPAG                         ; ZERO must be tied to 0

        tblrdl.w [TBLOFFSET],LEN
        add      TBLOFFSET,#2,TBLOFFSET
        addc     _TBLPAG                         ; ZERO must be tied to 0

        tblrdl.w [TBLOFFSET],FORMAT
        add      TBLOFFSET,#2,TBLOFFSET
        addc     _TBLPAG                         ; ZERO must be tied to 0

        clr      UPPERBYTE
        lsr      FORMAT,#7,PAGE                  ; PAGE <- stored DSWPAG
        and      #0x7F,FORMAT                    ; zero out upper bits

	cp.b     FORMAT,#FMT_CALL
	bra nz,  2f

        rcall    __long_indirect_call
        clr      ZERO                            ; calling a fn may clobber this
        bra      4f

2:	cp.b     FORMAT,#FMT_CLEAR
        bra nz,  2f

        ;; FMT_CLEAR - clear destination memory
9:      clr.b    [DSTOFFSET++]
        dec      LEN,LEN
        bra gtu, 9b                              ; loop if not done 
        bra      4f

        ;; FMT_COPY2, FMT_COPY3
2:      cp       FORMAT,#FMT_COPY2
        bra z,   3f

        setm     UPPERBYTE

        ;; standard memcpyd3
3:      rcall __memcpypd3_std

4:      tblrdh.b [TBLOFFSET],PAGE               ; check for link
        tblrdl.w [TBLOFFSET],DSTOFFSET
        cp0.b    PAGE
        bra nz,  1f
        cp0      DSTOFFSET
        bra nz,  1b
        return

1:      and      #0x7F,PAGE
        mov      PAGE, _TBLPAG                  ; link
        mov      DSTOFFSET, TBLOFFSET
        bra      4b

__memcpypd3_std:
;;
;; Copy data from program memory to data memory
;;
;; Not C callable
;;

1:
        tblrdl.b [TBLOFFSET++],[DSTOFFSET++]  ; dst++ = lo byte
        dec      LEN,LEN          ; num--
        bra      z,2f           ; br if done         ( add one )

        tblrdl.b [TBLOFFSET--],[DSTOFFSET++]  ; dst++ = hi byte
        dec      LEN,LEN          ; num--
        bra      z,4f           ; br if done         ( add two )

        cp0      UPPERBYTE             ; test upper flag
        bra      nz,1f           ; br if false
 
3:      add      TBLOFFSET,#2,TBLOFFSET
        addc     _TBLPAG
        bra      1b

1:      tblrdh.b [TBLOFFSET],[DSTOFFSET++]    ; dst++ = upper byte
        dec      LEN,LEN        ; num--
        bra      nz,3b          ; br if not done

4:      inc      TBLOFFSET,TBLOFFSET
2:      add      TBLOFFSET,#1,TBLOFFSET
        addc     _TBLPAG
        return                  ; exit


.include "null_signature.s"

