.include "m328Pdef.inc"

ldi r16, 0xFF ; identifying all pins as ouput in register D
out DDRD, r16 ; declaring pins as output in register D

ldi r16, 0b00100000 ; identifying all pins as input in register B
out DDRB, r16 ; declaring pins as input in register B

;sbi DDRB, 5 ;set pin 13 as output pin (DDRB pin 5)
ldi r16, 0b00000101 ;the last 3 bits define the prescaler, 101 => division by 1024
out TCCR0B, r16 ;prescalar used = 1024. So new freq. of clock cycle = (16 MHz / 1024) = 16 kHz

clr r28 ;output bits. we are only interested in bit 6 from the right.

.def W = r17
.def X = r18
.def Y = r15
.def Z = r20

.def A = r21
.def B = r22
.def C = r23
.def D = r24

.def ONE = r30
.def TEMP = r16
.def TEMP2 = r25
.def TEMP3 = r26
.def TEMP4 = r27

ldi ONE, 0b00000001
clr TEMP
clr TEMP2
clr TEMP3
clr TEMP4

loop:
  in TEMP, PINB

  mov W, TEMP
  and W, ONE

  lsr TEMP
  mov X, TEMP
  and X, ONE

  lsr TEMP
  mov Z, TEMP
  and Z, ONE

  lsr TEMP
  mov Y, TEMP
  and Y, ONE
  
; A
  mov TEMP, W
  eor TEMP, ONE
  mov A, TEMP

; B
  ; !W & X & !Z
  mov TEMP, W
  eor TEMP, ONE
  and TEMP, X
  mov TEMP2, Z
  eor TEMP2, ONE
  and TEMP, TEMP2

  ; W & !X & !Z
  mov TEMP2, X
  eor TEMP2, ONE
  and TEMP2, W
  mov TEMP3, Z
  eor TEMP3, ONE
  and TEMP2, TEMP3

  or TEMP, TEMP2
  mov B, TEMP

; C
  ; !W & Y & !Z
  mov TEMP, W
  eor TEMP, ONE
  and TEMP, Y
  mov TEMP2, Z
  eor TEMP2, ONE
  and TEMP, TEMP2

  ; !X & Y & !Z
  mov TEMP2, X
  eor TEMP2, ONE
  and TEMP2, Y
  mov TEMP3, Z
  eor TEMP3, ONE
  and TEMP2, TEMP3

  ; W & X & !Y & !Z
  mov TEMP3, Y
  eor TEMP3, ONE
  and TEMP3, W
  and TEMP3, X
  mov TEMP4, Z
  eor TEMP4, ONE
  and TEMP3, TEMP4

  or TEMP, TEMP2
  or TEMP, TEMP3
  mov C, TEMP

; D
  ; W & X & Y & !Z
  mov TEMP, Z
  eor TEMP, ONE
  and TEMP, X
  and TEMP, Y
  and TEMP, W

  ; !W & !X & !Y & Z
  mov TEMP2, W
  eor TEMP2, ONE
  and TEMP2, Z
  mov TEMP3, X
  eor TEMP3, ONE
  and TEMP2, TEMP3
  mov TEMP3, Y
  eor TEMP3, ONE
  and TEMP2, TEMP3

  or TEMP, TEMP2
  mov D, TEMP

  lsl B

  lsl C
  lsl C

  lsl D
  lsl D
  lsl D

  mov TEMP, A
  add TEMP, B
  add TEMP, C
  add TEMP, D
  lsl TEMP
  lsl TEMP

	out PORTD, TEMP

  ldi TEMP, 0b00100000	;initializing
	eor r28, TEMP ;change the output of LED
	out PORTB, r28 

	ldi r19, 0b01000000 ;times to run the loop = 64 for 1 second delay
	rcall PAUSE 		;call the PAUSE label
	rjmp loop
	
PAUSE:	;this is delay (function)
lp2:	;loop runs 64 times
		IN r16, TIFR0 ;tifr is timer interupt flag (8 bit timer runs 256 times)
		ldi r17, 0b00000010
		AND r16, r17 ;need second bit
		BREQ PAUSE 
		OUT TIFR0, r17	;set tifr flag high
	dec r19
	brne lp2
	ret

Start:
  rjmp Start

