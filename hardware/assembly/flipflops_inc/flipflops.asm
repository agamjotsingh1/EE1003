.include "m328Pdef.inc"

ldi r16, 0xFF ; identifying all pins as ouput in register D
out DDRD, r16 ; declaring pins as output in register D

ldi r16, 0b00100000 ; identifying all pins (except 13 which is clock) as input in register B
out DDRB, r16       ; declaring pins as input and output in register B

ldi r16, 0b00000101 ; the last 3 bits define the prescaler, 101 => division by 1024
out TCCR0B, r16     ; prescalar used = 1024. So new freq. of clock cycle = (16 MHz / 1024) = 16 kHz

clr r28 ;output bits. we are only interested in bit 6 from the right.

.def W = r15
.def X = r16
.def Y = r17
.def Z = r18

.def A = r19
.def B = r20
.def C = r21
.def D = r22

.def ONE = r30
.def TMP = r23
.def TMP2 = r24

ldi ONE, 0b00000001

loop:
  in TMP, PINB

  mov W, TMP
  and W, ONE

  lsr TMP
  mov X, TMP
  and X, ONE

  lsr TMP
  mov Z, TMP
  and Z, ONE

  lsr TMP
  mov Y, TMP
  and Y, ONE
  
; A
  mov TMP, W
  eor TMP, ONE
  mov A, TMP

; B
  ; !W & X & !Z
  mov TMP, Z
  eor TMP, ONE
  and TMP, X
  and TMP, A ; A = !W
  mov B, TMP

  ; W & !X & !Z
  mov TMP, X
  eor TMP, ONE
  and TMP, W
  mov TMP2, Z
  eor TMP2, ONE
  and TMP, TMP2

  or B, TMP

; C
  ; !W & Y & !Z
  mov TMP, Z
  eor TMP, ONE
  and TMP, Y
  and TMP, A ; A = !W
  mov C, TMP

  ; !X & Y & !Z
  mov TMP, X
  eor TMP, ONE
  and TMP, Y
  mov TMP2, Z
  eor TMP2, ONE
  and TMP, TMP2
  or C, TMP

  ; W & X & !Y & !Z
  mov TMP, Y
  eor TMP, ONE
  and TMP, W
  and TMP, X
  mov TMP2, Z
  eor TMP2, ONE
  and TMP, TMP2
  or C, TMP

; D
  ; W & X & Y & !Z
  mov TMP, Z
  eor TMP, ONE
  and TMP, X
  and TMP, Y
  and TMP, W
  mov D, TMP

  ; !W & !X & !Y & Z
  mov TMP, X
  eor TMP, ONE
  and TMP, A ; A = !W
  and TMP, Z
  mov TMP2, Y
  eor TMP2, ONE
  and TMP, TMP2
  or D, TMP

  lsl B

  lsl C
  lsl C

  lsl D
  lsl D
  lsl D

  mov TMP, A
  add TMP, B
  add TMP, C
  add TMP, D
  lsl TMP
  lsl TMP

	out PORTD, TMP

  ldi TMP, 0b00100000	;initializing
	eor r28, TMP ;change the output of LED
	out PORTB, r28 

	ldi r29, 0b01000000 ;times to run the loop = 64 for 1 second delay
	rcall PAUSE 		;call the PAUSE label
	rjmp loop
	
PAUSE:	;this is delay (function)
lp2:	;loop runs 64 times
		IN r16, TIFR0 ;tifr is timer interupt flag (8 bit timer runs 256 times)
		ldi r17, 0b00000010
		AND r16, r17 ;need second bit
		BREQ PAUSE 
		OUT TIFR0, r17	;set tifr flag high
	dec r29
	brne lp2
	ret

Start:
  rjmp Start

