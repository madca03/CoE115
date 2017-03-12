.include "p24fj64gb202.inc"
.global __reset

config __CONFIG1, FWDTEN_OFF & JTAGEN_OFF & ICS_PGx1
config __CONFIG2, POSCMD_NONE & OSCIOFCN_ON & FCKSM_CSDCMD & FNOSC_FRCPLL
config __CONFIG3, SOSCSEL_OFF

.bss
    i: .space 2
    j: .space 2

.text
__reset:
    mov #__SP_init, W15
    mov #__SPLIM_init, W0
    mov W0, SPLIM

    clr i
    clr j
    clr W10

    mov #0xFFFE, W0
    mov W0, TRISA
    mov #0x0001, W0
    mov W0, ANSA

    mov #0x90FF, W0 ; RB[11:8] - data line, RB[13] - reg select, RB[14] - enable
    mov W0, TRISB

    call delay15ms

    mov #0x3, W0
    call send4ToLCD
    call delay4_1ms

    call send4ToLCD
    call delay100us

    call send4ToLCD

    call delay1s
    call delay1s

    mov #0x2, W0
    call send4ToLCD

    mov #0x28, W0   ; function set (Interface = 4 bit, no. of lines = 2, character font = 5 x 7)
    call send8ToLCD
    call delay100us

    mov #0x04, W0
    call send8ToLCD
    call delay100us

    mov #0x06, W0   ; Entry mode set
    call send8ToLCD
    call delay100us

    mov #0x01, W0   ; Clear display
    call send8ToLCD
    call delay15ms

    mov #0x0F, W0   ; Display on, Cursor On, Blink On
    call send8ToLCD
    call delay100us

    ; Initialization Complete

main:
    call write_name

waitdown:
    btsc PORTA, #4
    goto waitdown
    call clear_display
    call write_name_reverse
    bset LATA, #0

waitup:
    btss PORTA, #4
    goto waitup
    call clear_display
    call write_name
    bclr LATA, #0
    goto waitdown

clear_display:
    mov #0x01, W0   ; Clear display
    call send8ToLCD
    call delay15ms
    return

write_name:
    mov #0x80, W0   ; Set address
    call send8ToLCD
    call delay100us

    mov #0x014D, W0  ; send "M"
    call send8ToLCD
    call delay100us

    mov #0x141, W0
    call send8ToLCD
    call delay100us

    mov #0x152, W0
    call send8ToLCD
    call delay100us

    mov #0x14B, W0
    call send8ToLCD
    call delay100us

    mov #0x120, W0
    call send8ToLCD
    call delay100us

    mov #0x141, W0
    call send8ToLCD
    call delay100us

    mov #0x14C, W0
    call send8ToLCD
    call delay100us

    mov #0x14C, W0
    call send8ToLCD
    call delay100us

    mov #0x145, W0
    call send8ToLCD
    call delay100us

    mov #0x14E, W0
    call send8ToLCD
    call delay100us

    mov #0xC0, W0
    call send8ToLCD
    call delay100us

    mov #0x141, W0
    call send8ToLCD
    call delay100us

    mov #0x147, W0
    call send8ToLCD
    call delay100us

    mov #0x141, W0
    call send8ToLCD
    call delay100us

    mov #0x154, W0
    call send8ToLCD
    call delay100us

    mov #0x14F, W0
    call send8ToLCD
    call delay100us

    mov #0x14E, W0
    call send8ToLCD
    call delay100us

    return

write_name_reverse:
    mov #0x80, W0
    call send8ToLCD
    call delay100us

    mov #0x141, W0
    call send8ToLCD
    call delay100us

    mov #0x147, W0
    call send8ToLCD
    call delay100us

    mov #0x141, W0
    call send8ToLCD
    call delay100us

    mov #0x154, W0
    call send8ToLCD
    call delay100us

    mov #0x14F, W0
    call send8ToLCD
    call delay100us

    mov #0x14E, W0
    call send8ToLCD
    call delay100us

    mov #0xC0, W0   ; Set address
    call send8ToLCD
    call delay100us

    mov #0x014D, W0  ; send "M"
    call send8ToLCD
    call delay100us

    mov #0x141, W0
    call send8ToLCD
    call delay100us

    mov #0x152, W0
    call send8ToLCD
    call delay100us

    mov #0x14B, W0
    call send8ToLCD
    call delay100us

    mov #0x120, W0
    call send8ToLCD
    call delay100us

    mov #0x141, W0
    call send8ToLCD
    call delay100us

    mov #0x14C, W0
    call send8ToLCD
    call delay100us

    mov #0x14C, W0
    call send8ToLCD
    call delay100us

    mov #0x145, W0
    call send8ToLCD
    call delay100us

    mov #0x14E, W0
    call send8ToLCD
    call delay100us

    return

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
    mov #0x271F, W14
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
    mov #0x0AAB, W14
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

delay1s:
    mov #0x000b, W14
    mov W14, i
    mov #0x2C23, W14
    mov W14, j

loop1s:
    dec j
    bra nz, loop1s
    dec i
    bra nz, loop1s
    return

delaybtn:
    mov #0x000b, W0
    mov W0, i
    mov #0x2C23, W0
    mov W0, j

loopbtn:
    dec j
    bra nz, loopbtn
    dec i
    bra nz, loopbtn
    return

send4ToLCD:
    ; W1 - holds RS bit
    ; W2 - holds RS bit and E bit
    ; W3 - holds RS bit, E bit, data bits
    and W0, #0x10, W1   ; get the RS bit from W0[4]
    sl W1, #9, W1       ; move RS bit from bit4 to bit13
    mov W1, W2          ; copy W1 to W2
    bset W2, #14        ; set E bit at bit14
    and W0, #0xf, W3    ; get data bits from W0[3:0]
    sl W3, #8, W3       ; move data bits from bits[3:0] to bits [11:8]
    ior W2, W3, W3      ; copy RS bit and E bit to W3
    mov W1, LATB        ; set RS bit
    mov W2, LATB        ; set E bit
    mov W3, LATB        ; set data bits
    ; bclr LATB, #14	; reset E bit
    clr LATB
    return

send8ToLCD:
  ; W1 - holds RS bit
  ; W2 - holds RS bit, E bit
  ; W3 - holds RS bit, E bit, data bits[7:4]
  ; W4 - holds RS bit, data bits[7:4]
  ; W5 - holds RS bit, E bit, data bits[3:0]
  mov #0x0100, W6     ; mask for getting the RS bit from W0[8]
  and W0, W6, W1      ; get the RS bit from W0[8]
  sl W1, #5, W1       ; move RS bit from bit8 to bit13
  mov W1, W2          ; copy W1 to W2
  bset W2, #14        ; set E bit at bit14
  mov #0x00f0, W6     ; mask for getting the upper 4 data bits[7:4]
  and W0, W6, W3      ; get the upper 4 data bits[7:4]
  sl W3, #4, W3       ; move data bits from W3[7:4] to W3[11:8]
  ior W2, W3, W3      ; copy RS bit and E bit to W3
  mov W3, W4          ; copy W3 to W4
  bclr W4, #14        ; clear E bit (bit14)
  mov #0x000f, W6     ; mask for getting the lower 4 data bits[3:0]
  and W0, W6, W5      ; get the lower 4 data bits[7:4]
  sl W5, #8, W5       ; move data bits from W4[3:0] to W4[11:8]
  ior W2, W5, W5      ; copy RS bit and E bit
  mov W1, LATB        ; set RS bit
  mov W2, LATB        ; set E bit
  mov W3, LATB        ; send DB[7:4] bits
  nop
  mov W4, LATB        ; clear E bit
  mov W2, LATB        ; set E bit
  mov W5, LATB        ; send DB[3:0] bits
  ; bclr LATB, #14      ; reset E bit
  clr LATB
  return

end:
.end
