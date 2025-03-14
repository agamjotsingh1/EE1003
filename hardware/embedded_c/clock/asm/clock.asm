.include "m328Pdef.inc"

ldi r16, 0xFF
out DDRD, r16

ldi r16, 0xFF
out DDRB, r16

; ----
; inputs -> r17, r18, r19, r20 (H1, H2, M1, M2)
; ----
display_time:
    ldi r30, 4

    out PORTD, r17
    sbi PORTB, 3

    ldi r29, 255
    call delay
    cbi PORTB, 3

    out PORTD, r18
    sbi PORTB, 2

    ldi r29, 255
    call delay
    cbi PORTB, 2

    out PORTD, r19
    sbi PORTB, 1

    ldi r29, 255
    call delay
    cbi PORTB, 1

    out PORTD, r20
    sbi PORTB, 0

    ldi r29, 255
    call delay
    cbi PORTB, 0

; INCREMENTING TIME MODULE
; ----
; inputs -> r17, r18, r19, r20 (H1, H2, M1, M2)
; ----
increment_time:
    cpi r20, 9
    brne increment_m2

    inc r19
    ldi r20, 0

    cpi r19, 6
    brne end_routine

    ldi r19, 0
    inc r18

    cpi r18, 5
    brne check_h2

    cpi r17, 2
    brne end_routine

    ldi r17, 0
    ldi r18, 0

end_routine:
    ret

increment_m2:
    inc r20
    ret

check_h2:
    cpi r18, 10
    brlo end_routine

    ldi r18, 0
    inc r17

; -----
; inputs -> r29, r30
; clobbers -> r31
; ----
; accurate delay -> [ 1023*(r16) - 1 ] cycles
; -----
delay:
    outer_loop:
        mov r31, r30 
        inner_loop:
            nop
            dec r31
            brne inner_loop
        
        dec r29
        brne outer_loop
    ret

clr r16 ; time register

; registers for clock display
clr r17
clr r18
clr r19
clr r20

increment_and_reset_time:
    call increment_time
    clr r16
    ret

loop:
    ;cpi r16, 200
    ;brsh increment_and_reset_time

    ;call display_time

    out PORTD, r17
    sbi PORTB, 3

    ;ldi r29, 255
    ;call delay
    ;cbi PORTB, 3

    ;inc r16
    rjmp loop
