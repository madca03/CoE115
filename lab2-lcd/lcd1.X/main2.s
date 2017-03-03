
    
    mov #0x01, W0
    call send8ToLCD
    call delay40us
    
    mov #0x06, W0
    call send8ToLCD
    call delay40us
    
    mov #0x0F, W0
    call send8ToLCD
    call delay40us
    
    ; Initialization Complete
    ; Display Ready
    
    mov #0x014D, W0
    call send8ToLCD
    call delay40us
    
    mov #0x141, W0
    call send8ToLCD
    call delay40us


