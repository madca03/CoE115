.include "p24FJ64GB202.inc"
.global __reset

config __CONFIG1, FWDTEN_OFF & JTAGEN_OFF & ICS_PGx1
config __CONFIG2, POSCMD_NONE & OSCIOFCN_OFF & FCKSM_CSDCMD & FNOSC_FRCPLL
config __CONFIG3, SOSCSEL_OFF
config __CONFIG4, PLLDIV_NODIV
    
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

    mov #0x0000, W0
    mov W0, TRISA
    mov W0, TRISB
    mov #0xFFFF, W1

blinky:
    clr PORTA
    clr PORTB
    call delay
    mov W1, PORTA
    mov W1, PORTB
    call delay
    bra blinky

delay:
    mov #0x000b, W0
    mov W0, i
    mov #0x2C23, W0
    mov W0, j

loop:
    dec j
    bra nz, loop
    dec i
    bra nz, loop
    return
.end
