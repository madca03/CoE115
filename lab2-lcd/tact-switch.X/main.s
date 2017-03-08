.include "p24fj64gb202.inc"
.global __reset
     
config __CONFIG1, FWDTEN_OFF & JTAGEN_OFF & ICS_PGx1
config __CONFIG2, POSCMD_NONE & OSCIOFCN_ON & FCKSM_CSDCMD & FNOSC_FRCPLL
config __CONFIG3, SOSCSEL_OFF
;config __CONFIG4, PLLDIV_NODIV
    
.bss
    
.text
__reset:
    mov #__SP_init, W15
    mov #__SPLIM_init, W0
    mov W0, SPLIM
    
    mov #0xFFFE, W0
    mov W0, TRISA
    mov #0x0001, W0
    mov W0, ANSA
    
main:
    
waitdown:
    bset LATA, #0
    btsc PORTA, #4
    goto waitdown
    
waitup:
    bclr LATA, #0
    btss PORTA, #4
    goto waitup
    
    goto main
    
.end
    