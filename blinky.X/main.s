.include "p24fj64gb002.inc"
.global __reset
    
config __CONFIG1, FWDTEN_OFF & JTAGEN_OFF & ICS_PGx2
config __CONFIG2, POSCMOD_NONE & OSCIOFNC_OFF & FCKSM_CSDCMD & FNOSC_FRCPLL & PLL96MHZ_OFF & PLLDIV_NODIV

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


