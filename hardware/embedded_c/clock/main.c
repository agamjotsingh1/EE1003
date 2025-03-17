#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h> 
#include <stdlib.h> 

typedef uint8_t byte;
#define LOW_BIT(X, Y) X &= ~_BV(Y)
#define HIGH_BIT(X, Y) X |= _BV(Y)
#define SET_BIT(X, Y, VAL) (VAL) ? (HIGH_BIT(X, Y)): (LOW_BIT(X, Y))

// POWER (seven segment) pins
#define P1 3
#define P2 2
#define P3 1
#define P4 0

int flag = 0;

void init_timer() {
    // Set CTC mode (WGM12 = 1)
    TCCR1B |= (1 << WGM12);
    
    // Set prescaler to 1024: CS12 = 1, CS10 = 1
    TCCR1B |= (1 << CS12) | (1 << CS10);

    // Set OCR1A for 1s delay at 16MHz (16,000,000 / 1024 = 15625 ticks per second)
    OCR1A = 15624;

    // Enable Timer1 compare match interrupt
    TIMSK1 |= (1 << OCIE1A);
    
    // Enable global interrupts
    sei();
}

// Timer1 compare match ISR
ISR(TIMER1_COMPA_vect) {
    flag = 1;
}

// instruction time -> 
void send_to_bcd(int n){
    for(int i = 0; i < 4; i++){
        SET_BIT(PORTD, i + 2, (n&1));
        n = (n >> 1);
    }
}

void display_time(int h1, int h2, int m1, int m2){
    send_to_bcd(h1);
    HIGH_BIT(PORTB, P1);
    _delay_ms(1);
    LOW_BIT(PORTB, P1);

    send_to_bcd(h2);
    HIGH_BIT(PORTB, P2);
    _delay_ms(1);
    LOW_BIT(PORTB, P2);

    send_to_bcd(m1);
    HIGH_BIT(PORTB, P3);
    _delay_ms(1);
    LOW_BIT(PORTB, P3);

    send_to_bcd(m2);
    HIGH_BIT(PORTB, P4);
    _delay_ms(1);
    LOW_BIT(PORTB, P4);
}

void increment_time(int* h1, int* h2, int* m1, int* m2){
    if(*m2 == 9){
        *m1 = *m1 + 1;
        *m2 = 0;

        if(*m1 == 6){
            *m1 = 0;
            *h2 = *h2 + 1;

            if(*h2 == 5 && *h1 == 2){
                *h1 = 0; *h2 = 0;
            }
            else if(*h2 >= 10){
                *h2 = 0;
                *h1 = *h1 + 1;
            }
        }
    }
    else {
        *m2 = *m2 + 1;
    }
}

int main(void){
    DDRB = 0xFF;
    DDRD = 0xFF;

    int h1 = 0;
    int h2 = 0;
    int m1 = 0;
    int m2 = 0;

    init_timer();

    while(1){
        if(flag){
            increment_time(&h1, &h2, &m1, &m2);
            flag = 0;
        }

        display_time(h1, h2, m1, m2);
    }
}
