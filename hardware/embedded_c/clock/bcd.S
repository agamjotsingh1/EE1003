#define __SFR_OFFSET 0          // Use 0 for the I/O register offset
#include <avr/io.h>             // Defines I/O port aliases

.global send_to_bcd
  
.section .text                  ; Defines a code section

send_to_bcd:
    out PORTD, r24
    ret
