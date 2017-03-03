.include "p24FJ64GB202.inc"
.global __reset
    
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
    
    mov #0xFFFC, W0
    mov W0, TRISA
    mov #0x0003, W1
    
blinky:
    clr PORTA
    call delay
    mov W1, PORTA
    call delay
    bra blinky
    
delay:
    mov #0x0003, W0
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


