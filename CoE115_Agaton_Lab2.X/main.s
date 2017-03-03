.include "p24fj64gb002.inc"
.global __reset
    
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
    
    mov #0x90FF, W0
    mov W0, TRISB

blinky:    
    mov #0x3, W0
    call send4ToLCD
    call delay4_1ms
    
    call send4ToLCD
    call delay100us
    
    call send4ToLCD
    mov #0x2, W0
    call send4ToLCD
    call delay15ms
    
    
    
    call blinky
    
delay15ms:
    mov #0x2710, W14
    mov W14, j
    
loop15ms:
    dec j
    bra nz, loop15ms
    return

delay4_1ms:
    mov #0x0AAE, W14
    mov W14, j
    
loop4_1ms:
    dec j
    bra nz, loop4_1ms
    return
    
delay100us:
    mov #0x0042, W14
    mov W14, j
    
loop100us:
    dec j
    bra nz, loop100us
    return
    
send4ToLCD:
    clr W2
    bset W2, #14
    clr W1
    
    and W0, #0x10, W1
    sl W1, #9, W1
    ior W2, W1, W2
    clr W1
    
    and W0, #0x0F, W1
    sl W1, #8, W1
    ior W2, W1, W2
    mov W2, LATB
    bclr LATB, #14
    
    return
    
send8ToLCD:
    clr W2
    
    mov #0x0100, W3
    clr W1
    and W0, W3, W1
    sl W1, #5, W1
    ior W2, W1, W2
    mov W2, LATB
    
    bset LATB, #14
    
    clr W1
    and W0, #0x0F, W1
    sl W1, #8, W1
    ior W2, W1, W2
    mov W2, LATB
    
    bclr LATB, #14
    
    bset LATB, #14
    
    
    clr W1
    and W0, #0xF0, W1
    sl W1, #8, W1
    ior W2, W1, W2
    mov W2, LATB
    
    bclr LATB,#14
    
end:
.end


