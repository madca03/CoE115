;;
;; C Run-time startup module for MPLAB XC16 C compiler.
;; (c) Copyright 2015 - 2016 Microchip Technology, All rights reserved
;;
;; Data initialization support.
;; The linker loads this version when the --data-init
;; option is selected.
;;

	.section .init,code
        .global __data_init_duo
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
;;  int  page:9;      /* destination page     */
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
;;  __memcpypd3crt
;; 
;; for 16-bit Microchip devices with dual cores
;;

        .equ __data_init, __data_init_duo
        .global __data_init
	.global __data_init_duo
__data_init_duo:

        ; for all format codes, hi-bit set means there is a DST page at
        ; data[0a
        ;                           ;  format codes
        .equiv   FMT_CLEAR,0        ;  clear len bytes
        .equiv   FMT_COPY2,1        ;  copy len bytes (not packed)
        .equiv   FMT_COPY3,2        ;  copy len bytes (packed)
        .equiv   FMT_CALL, 3
                                    ;  this space intentionally left blank
                                    ;  begin subordinate codes
        .equiv   FMT_DUO_COPY3,0x1F ;  copy len instructions to slave

#define ZERO      w0
#define TBLOFFSET w9
	; DSTOFFSET, LEN must be paired for call.l
#define DSTOFFSET w10
#define LEN       w11
#define UPPERBYTE w12
#define FORMAT    w13
#define PAGE      w14

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
        mov      PAGE,_DSWPAG                    ; set DSWPAG

        cp.b     FORMAT,#FMT_CALL
        bra nz,  2f

        call.l   DSTOFFSET
        clr      ZERO                            ; calling a fn may clobber this
        bra      4f

2:      cp.b     FORMAT,#FMT_CLEAR
        bra nz,  2f

        ;; FMT_CLEAR - clear destination memory
9:      clr.b    [DSTOFFSET]
        inc      DSTOFFSET,DSTOFFSET
        bra nc,  8f
        inc      _DSWPAG
        bset     DSTOFFSET,#15
8:      dec      LEN,LEN
        bra gtu, 9b                              ; loop if not done 
        bra      4f

        ;; FMT_COPY2, FMT_COPY3
2:      cp       FORMAT,#FMT_COPY2
        bra z,   3f

        setm     UPPERBYTE

        ;; special memcpyd3
3:      rcall __memcpyd3extended

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

__memcpyd3extended:
;;
;; Copy data from program memory to data memory using
;;  Inputs, ZERO and w5
;; 
;; not user callable
;;
;; Inputs:
;;  TBLOFFSET:_TBLPAG source addres (24-bits)
;;  DSTOFFSET:_DSWPAG destination address (24-bits)
;;  LEN number of bytes (even or odd)
;;  UPPERBYTE upper byte flag   (0 = false)
;;
;; Outputs:
;;  TBLOFFSET:_TBLPAG  next source address (24 bits)
;;
;; This function does not follow the standard C calling conventions
;;

1:      
        btst.c   UPPERBYTE, #13        ; set carry if UPPERBYTE is set      
        addc     DSTOFFSET, #2, [w15]  
        bra nc,  2f                    ; this will not cross

        ; the destination offset will cross a page, so check each time
        mov      TBLOFFSET,w5          ; w5 =   tbloffset(src)
        tblrdl.b [w5++],[DSTOFFSET]    ; dst = lo byte
        inc      DSTOFFSET,DSTOFFSET
        bra nc,  3f
	inc      _DSWPAG               ; increment write page
	bset     DSTOFFSET,#15         ; set bit 15
3:       
        dec      LEN,LEN               ; num--
        bra z,   4f                    ; br if done

        tblrdl.b [w5--],[DSTOFFSET]    ; dst++ = hi byte
        inc      DSTOFFSET,DSTOFFSET
        bra nc,  3f
        inc      _DSWPAG
        bset     DSTOFFSET,#15
3:
        dec      LEN,LEN               ; num--
        bra z,   4f                    ; br if done

        cp0      UPPERBYTE             ; test upper flag
        bra z,   4f                    ; br if false

        tblrdh.b [w5],[DSTOFFSET]      ; dst++ = upper byte
        inc      DSTOFFSET,DSTOFFSET
        bra nc,  3f
        inc      _DSWPAG
        bset     DSTOFFSET,#15
3:       
        dec      LEN,LEN               ; num--
4:      
        inc2     TBLOFFSET,TBLOFFSET
        addc     _TBLPAG               ; ZERO must be tied to 0
        cp0      LEN
        bra nz,   1b
        return                         ; exit

2:      ; the destination offset will not cross this time, be quicker
        ;   still need to write byte at a time in case there is a funky offset
        tblrdl  [TBLOFFSET],w5
        mov.b   w5,[DSTOFFSET++]
        dec     LEN,LEN
        bra z,  4b

        lsr     w5,#8,w5
        mov.b   w5,[DSTOFFSET++]
        dec     LEN,LEN
        bra z,  4b

        cp0     UPPERBYTE
        bra z,  4b

        tblrdh.b [TBLOFFSET],[DSTOFFSET++]
        bra     3b


#include "subordinate_errors.h"

        ; extern int _program_slave(int slave_number, int verify, 
        ;                           __eds__ unsigned char *image);
        ; uses same format as the .dinit table
        ; this function currently supports 1 slave - slave 1
	.global __program_slave
__program_slave:

        cp       w0, #1
        bra z,   good_slave
#if ESLV_INVALID < 1024
        retlw    #ESLV_INVALID,w0
#else
        mov      #ESLV_INVALID,w0
        return
#endif
good_slave:      
        push     DSRPAG
        push     DSWPAG
        mov.d    w0, w4                 ; makes things easier later
        mov      w3, DSRPAG
next_record:
        mov.d    [w2++], w0             ; w0 <- dest address, w1 <- len
        cp0      w1
        bra eq,  done                    ; all finished, no error
        mov      [w2++], w6             ; w6 <- page:format
        lsr      w6,#7,w7
        mov      w7, DSWPAG             ; write page
        and      #0x7f,w6                 
        cp       w6,#FMT_DUO_COPY3
        bra eq,  copy_slave
        mov      #ESLV_BAD_IMAGE,w0     ; set error code and return

done:   pop      DSWPAG
        pop      DSRPAG
        return

        ; ready to copy len (w1) instructions 
        ; starting at w2, storing at w0
      
copy_slave:      
        dec      w1,w1
        repeat   w1
        ldslv    [w2++],[w0++],#1
        bra      next_record                     ; read the next record


	.global __start_slave
__start_slave:
	mov 	#0x55, w0
	mov 	w0, MSI1KEY
	mov 	#0xAA, w0
	mov 	w0, MSI1KEY
	bset 	MSI1CON, #15
	return


.include "null_signature.s"
