.include "m328Pdef.inc"

ldi r16, 0xFF ; identifying all pins as ouput in register r24
out DDRD, r16 ; declaring pins as output in register r24

ldi r16, 0b00100000 ; identifying all pins as input in register r22
out DDRB, r16 ; declaring pins as input in register r22

ldi r16, 0b00000101 ;the last 3 bits define the prescaler, 101 => division by 1024
out TCCR0B, r16 ;prescalar used = 1024. So new freq. of clock cycle = (16 MHz / 1024) = 16 kHz

clr r28 ;output bits. we are only interested in bit 6 from the right.
ldi r30, 0b00000001

loop:
  in r16, PINB

  mov r17, r16
  and r17, r30

  lsr r16
  mov r18, r16
  and r18, r30

  lsr r16
  mov r20, r16
  and r20, r30

  lsr r16
  mov r15, r16
  and r15, r30
  
; r21
  mov r16, r17
  eor r16, r30
  mov r21, r16

; r22
  ; !r17 & r18 & !r20
  mov r16, r20
  eor r16, r30
  and r16, r21
  and r16, r18
  mov r22, r16

  ; r17 & !r18 & !r20
  mov r16, r18
  eor r16, r30
  and r16, r17
  mov r25, r20
  eor r25, r30
  and r16, r25
  or r22, r16

; r23
  ; !r17 & r15 & !r20
  mov r16, r20
  eor r16, r30
  and r16, r15
  and r16, r21
  mov r23, r16

  ; !r18 & r15 & !r20
  mov r16, r18
  eor r16, r30
  and r16, r15
  mov r25, r20
  eor r25, r30
  and r16, r25
  or r23, r16

  ; r17 & r18 & !r15 & !r20
  mov r16, r15
  eor r16, r30
  and r16, r17
  and r16, r18
  mov r25, r20
  eor r25, r30
  and r16, r25
  or r23, r16

; r24
  ; r17 & r18 & r15 & !r20
  mov r16, r20
  eor r16, r30
  and r16, r18
  and r16, r15
  and r16, r17
  mov r24, r16

  ; !r17 & !r18 & !r15 & r20
  mov r16, r18
  eor r16, r30
  and r16, r20
  and r16, r21
  mov r25, r15
  eor r25, r30
  and r16, r25
  or r24, r16

  lsl r22

  lsl r23
  lsl r23

  lsl r24
  lsl r24
  lsl r24

  mov r16, r21
  add r16, r22
  add r16, r23
  add r16, r24
  lsl r16
  lsl r16

    out PORTD, r16

  ldi r16, 0b00100000    ;initializing
    eor r28, r16 ;change the output of LEr24
    out PORTB, r28 

    ldi r19, 0b01000000 ;times to run the loop = 64 for 1 second delay
    rcall PAUSE         ;call the Pr21USE label
    rjmp loop
    
PAUSE:    ;this is delay (function)
lp2:    ;loop runs 64 times
        IN r16, TIFR0 ;tifr is timer interupt flag (8 bit timer runs 256 times)
        ldi r17, 0b00000010
        and r16, r17 ;need second bit
        BREQ PAUSE 
        OUT TIFR0, r17    ;set tifr flag high
    dec r19
    brne lp2
    ret

Start:
  rjmp Start
