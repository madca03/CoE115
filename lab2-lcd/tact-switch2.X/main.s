.include "p24fj64gb202.inc"
.global __reset
     
config __CONFIG1, FWDTEN_OFF & JTAGEN_OFF & ICS_PGx1
config __CONFIG2, POSCMD_NONE & OSCIOFCN_ON & FCKSM_CSDCMD & FNOSC_FRCPLL
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
    
    mov #0xFFFC, W0
    mov W0, TRISA
    clr W2
    mov #0xFFFF, W6
    mov W6, LATA
    mov #0x0001, W0
    mov W0, ANSA
    
main:

waitdown:
    btsc PORTA, #4
    goto waitdown
    xor #0x0003, W2
    mov W2, LATA
    call delay
    
waitup:
    btss PORTA, #4
    goto waitup
    call delay
    goto main
    
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


