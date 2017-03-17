;; C Run-time startup module for MPLAB XC16 C compiler.
;; (c) Copyright 2016 Microchip Technology, All rights reserved

;
;   Default __crt_start_mode function
;
;	modify to your taste
;
;   This function return 0 to indicate that a full startup is required
;

	.weak __crt_start_mode
__crt_start_mode:
	btsc NVMCON,#0xB
        retlw #1,w0
        retlw #0,w0

