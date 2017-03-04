.include "p24fj64gb202.inc"
.global __reset

config __CONFIG1, FWDTEN_OFF & JTAGEN_OFF & ICS_PGx1
config __CONFIG2, POSCMD_NONE & OSCIOFCN_OFF & FCKSM_CSDCMD & FNOSC_FRCPLL & PLLDIV_NODIV
config __CONFIG3, SOSCSEL_OFF

.bss
    i: .space 2
    j: .space 2

.text
__reset:
    mov #__SP_init, W15
    mov #__SPLIM_init, W0
    mov W0,SPLIM

    clr i
    clr j

    mov #0x90FF, W0 ; RB[11:8] - data line, RB[13] - reg select, RB[14] - enable
    mov W0, TRISB

    call delay15ms

    mov #0x3, W0
    ; W1 - temporary out register
    ; W2 - holder of data bits in correct posiiton
    and W0, #0x10, W1	; get the Register Select bit from W0[4]
    sl W1, #9, W1	; move RS bit from W1[4] to W1[13]
    mov W1, LATB	; set/reset the RS bit Rb[13]
    bset LATB, #14	; set E bit Rb[14]
    bset W1, #14	; set E bit of temp out reg
    and W0, #0xf, W2	; get the data bits W0[3:0]
    sl W2, #8, W2	; shift data bits from W2[3:0] to W2[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB	; set DB[7:4] bits Rb[11:8]
    clr LATB

    call delay4_1ms

    ; W1 - temporary out register
    ; W2 - holder of data bits in correct posiiton
    and W0, #0x10, W1	; get the Register Select bit from W0[4]
    sl W1, #9, W1	; move RS bit from W1[4] to W1[13]
    mov W1, LATB	; set/reset the RS bit Rb[13]
    bset LATB, #14	; set E bit Rb[14]
    bset W1, #14	; set E bit of temp out reg
    and W0, #0xf, W2	; get the data bits W0[3:0]
    sl W2, #8, W2	; shift data bits from W2[3:0] to W2[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB	; set DB[7:4] bits Rb[11:8]
    clr LATB

    call delay100us

    ; W1 - temporary out register
    ; W2 - holder of data bits in correct posiiton
    and W0, #0x10, W1	; get the Register Select bit from W0[4]
    sl W1, #9, W1	; move RS bit from W1[4] to W1[13]
    mov W1, LATB	; set/reset the RS bit Rb[13]
    bset LATB, #14	; set E bit Rb[14]
    bset W1, #14	; set E bit of temp out reg
    and W0, #0xf, W2	; get the data bits W0[3:0]
    sl W2, #8, W2	; shift data bits from W2[3:0] to W2[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB	; set DB[7:4] bits Rb[11:8]
    clr LATB


    mov #0x2, W0
    ; W1 - temporary out register
    ; W2 - holder of data bits in correct posiiton
    and W0, #0x10, W1	; get the Register Select bit from W0[4]
    sl W1, #9, W1	; move RS bit from W1[4] to W1[13]
    mov W1, LATB	; set/reset the RS bit Rb[13]
    bset LATB, #14	; set E bit Rb[14]
    bset W1, #14	; set E bit of temp out reg
    and W0, #0xf, W2	; get the data bits W0[3:0]
    sl W2, #8, W2	; shift data bits from W2[3:0] to W2[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB	; set DB[7:4] bits Rb[11:8]
    clr LATB


    mov #0x28, W0   ; function set (Interface = 4 bit, no. of lines = 2, character font = 5 x 7)
    ; W1 - temporary output register
    ; W2 - container for data bits
    ; W3 - used for masks
    ; W4 - holds RS bit
    mov #0x0100, W3	; mask for getting the RS bit from W0
    and W0, W3, W4	; get the Register Select (RS) bit from W0[8]
    sl W4, #5, W4	; move RS bit from W1[8] TO W1[13]
    mov W4, LATB	; set/reset the RS bit Rb[13]
    mov W4, W1
    bset LATB, #14	; set E bit Rb[14]
    bset W1, #14	; set E bit of temp out reg
    bset W4, #14
    mov #0x00f0, W3	; mask for getting the upper 4 bits
    and W0, W3, W2	; get the upper 4 bits
    sl W2, #4, W2	; shift data bits from W2[7:4] to W2[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB	; set DB[7:4] bits Rb[11:8]
    clr LATB		; clr E bit Rb[14]

    nop
    mov W4, LATB
    and W0, #0xf, W2	; get the lower 4 bits
    sl W2, #8, W2	; shift data bits from W2[3:0] to W2[11:8]
    mov #0xf0ff, W3	; mask for clearing old data bits in W1
    and W1, W3, W1	; clear W1[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB
    clr LATB

    call delay40us


    mov #0x08, W0     ; Display Off
    ; W1 - temporary output register
    ; W2 - container for data bits
    ; W3 - used for masks
    ; W4 - holds RS bit
    mov #0x0100, W3	; mask for getting the RS bit from W0
    and W0, W3, W4	; get the Register Select (RS) bit from W0[8]
    sl W4, #5, W4	; move RS bit from W1[8] TO W1[13]
    mov W4, LATB	; set/reset the RS bit Rb[13]
    mov W4, W1
    bset LATB, #14	; set E bit Rb[14]
    bset W1, #14	; set E bit of temp out reg
    bset W4, #14
    mov #0x00f0, W3	; mask for getting the upper 4 bits
    and W0, W3, W2	; get the upper 4 bits
    sl W2, #4, W2	; shift data bits from W2[7:4] to W2[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB	; set DB[7:4] bits Rb[11:8]
    clr LATB		; clr E bit Rb[14]

    nop
    mov W4, LATB
    and W0, #0xf, W2	; get the lower 4 bits
    sl W2, #8, W2	; shift data bits from W2[3:0] to W2[11:8]
    mov #0xf0ff, W3	; mask for clearing old data bits in W1
    and W1, W3, W1	; clear W1[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB
    clr LATB

    call delay40us


    mov #0x01, W0   ; Clear Display
    ; W1 - temporary output register
    ; W2 - container for data bits
    ; W3 - used for masks
    ; W4 - holds RS bit
    mov #0x0100, W3	; mask for getting the RS bit from W0
    and W0, W3, W4	; get the Register Select (RS) bit from W0[8]
    sl W4, #5, W4	; move RS bit from W1[8] TO W1[13]
    mov W4, LATB	; set/reset the RS bit Rb[13]
    mov W4, W1
    bset LATB, #14	; set E bit Rb[14]
    bset W1, #14	; set E bit of temp out reg
    bset W4, #14
    mov #0x00f0, W3	; mask for getting the upper 4 bits
    and W0, W3, W2	; get the upper 4 bits
    sl W2, #4, W2	; shift data bits from W2[7:4] to W2[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB	; set DB[7:4] bits Rb[11:8]
    clr LATB		; clr E bit Rb[14]

    nop
    mov W4, LATB
    and W0, #0xf, W2	; get the lower 4 bits
    sl W2, #8, W2	; shift data bits from W2[3:0] to W2[11:8]
    mov #0xf0ff, W3	; mask for clearing old data bits in W1
    and W1, W3, W1	; clear W1[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB
    clr LATB

    call delay40us


    mov #0x06, W0   ; Entry mode set
    ; W1 - temporary output register
    ; W2 - container for data bits
    ; W3 - used for masks
    ; W4 - holds RS bit
    mov #0x0100, W3	; mask for getting the RS bit from W0
    and W0, W3, W4	; get the Register Select (RS) bit from W0[8]
    sl W4, #5, W4	; move RS bit from W1[8] TO W1[13]
    mov W4, LATB	; set/reset the RS bit Rb[13]
    mov W4, W1
    bset LATB, #14	; set E bit Rb[14]
    bset W1, #14	; set E bit of temp out reg
    bset W4, #14
    mov #0x00f0, W3	; mask for getting the upper 4 bits
    and W0, W3, W2	; get the upper 4 bits
    sl W2, #4, W2	; shift data bits from W2[7:4] to W2[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB	; set DB[7:4] bits Rb[11:8]
    clr LATB		; clr E bit Rb[14]

    nop
    mov W4, LATB
    and W0, #0xf, W2	; get the lower 4 bits
    sl W2, #8, W2	; shift data bits from W2[3:0] to W2[11:8]
    mov #0xf0ff, W3	; mask for clearing old data bits in W1
    and W1, W3, W1	; clear W1[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB
    clr LATB

    call delay40us

    mov #0x0F, W0   ; Display on, Cursor blink on
    ; W1 - temporary output register
    ; W2 - container for data bits
    ; W3 - used for masks
    ; W4 - holds RS bit
    mov #0x0100, W3	; mask for getting the RS bit from W0
    and W0, W3, W4	; get the Register Select (RS) bit from W0[8]
    sl W4, #5, W4	; move RS bit from W1[8] TO W1[13]
    mov W4, LATB	; set/reset the RS bit Rb[13]
    mov W4, W1
    bset LATB, #14	; set E bit Rb[14]
    bset W1, #14	; set E bit of temp out reg
    bset W4, #14
    mov #0x00f0, W3	; mask for getting the upper 4 bits
    and W0, W3, W2	; get the upper 4 bits
    sl W2, #4, W2	; shift data bits from W2[7:4] to W2[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB	; set DB[7:4] bits Rb[11:8]
    clr LATB		; clr E bit Rb[14]

    nop
    mov W4, LATB
    and W0, #0xf, W2	; get the lower 4 bits
    sl W2, #8, W2	; shift data bits from W2[3:0] to W2[11:8]
    mov #0xf0ff, W3	; mask for clearing old data bits in W1
    and W1, W3, W1	; clear W1[11:8]
    ior W1, W2, W1	; move data bits to temp out reg
    mov W1, LATB
    clr LATB

    ; mov #0x014D, W0  ; send "M"
    ; ; W1 - temporary output register
    ; ; W2 - container for data bits
    ; ; W3 - used for masks
    ; ; W4 - holds RS bit
    ; mov #0x0100, W3	; mask for getting the RS bit from W0
    ; and W0, W3, W4	; get the Register Select (RS) bit from W0[8]
    ; sl W4, #5, W4	; move RS bit from W1[8] TO W1[13]
    ; mov W4, LATB	; set/reset the RS bit Rb[13]
    ; mov W4, W1
    ; bset LATB, #14	; set E bit Rb[14]
    ; bset W1, #14	; set E bit of temp out reg
    ; bset W4, #14
    ; mov #0x00f0, W3	; mask for getting the upper 4 bits
    ; and W0, W3, W2	; get the upper 4 bits
    ; sl W2, #4, W2	; shift data bits from W2[7:4] to W2[11:8]
    ; ior W1, W2, W1	; move data bits to temp out reg
    ; mov W1, LATB	; set DB[7:4] bits Rb[11:8]
    ; clr LATB		; clr E bit Rb[14]
    ;
    ; nop
    ; mov W4, LATB
    ; and W0, #0xf, W2	; get the lower 4 bits
    ; sl W2, #8, W2	; shift data bits from W2[3:0] to W2[11:8]
    ; mov #0xf0ff, W3	; mask for clearing old data bits in W1
    ; and W1, W3, W1	; clear W1[11:8]
    ; ior W1, W2, W1	; move data bits to temp out reg
    ; mov W1, LATB
    ; clr LATB

    call end
; loop:
    ; clr LATB
    ; call loop

delay15ms:
    ; instruction cycle freq = 2Mhz
    ; instruction cycle period = 0.5us
    ; clock cycle freq = 4Mhz
    ; to find the counter value use this formula
    ; (provided that the loop counter is less than 16 bits)
    ; (3 * j) + 2 + 4 = inst_count
    ; where j is the loop counter
    ; inst_count = 15ms / 0.5us = 30000 inst
    ; j = 9998 = 0x270E
    mov #0x270F, W14
    mov W14, j

loop15ms:
    dec j
    bra nz, loop15ms
    return

delay4_1ms:
    ; inst_count = 4.1ms / 0.5us = 8200 inst
    ; add 1 nop for j to become an integer
    ; (3 * j) + 2 + 4 + 1 = 8200
    ; j = 2731 = 0x0AAB
    mov #0x0AAC, W14
    mov W14, j

loop4_1ms:
    dec j
    bra nz, loop4_1ms
    return

delay100us:
    ; inst_count = 100us / 0.5us = 200 inst
    ; add 2 nop for j to become an integer
    ; (3 * j) + 2 + 4 + 2 = 200
    ; j = 64 = 0x40
    mov #0x0040, W14
    mov W14, j

loop100us:
    dec j
    bra nz, loop100us
    return

delay40us:
    mov #0x0018, W14
    mov W14, j

loop40us:
    dec j
    bra nz, loop40us
    return

end:
.end
