#include <avr/io.h> 
#include <avr/interrupt.h>
#include <util/delay.h> 
#include <stdlib.h> 

typedef uint8_t byte;
#define LOW_BIT(X, Y) X &= ~(_BV(Y))
#define HIGH_BIT(X, Y) X |= (_BV(Y))
#define SET_BIT(X, Y, VAL) (VAL) ? (HIGH_BIT(X, Y)): (LOW_BIT(X, Y))

#define NOT(X) ((~X) & 1)

// POWER (seven segment) pins
#define H1 5
#define H2 4
#define M1 3
#define M2 2
#define S1 1
#define S2 0

// BCD pins
#define BCD_A 2  
#define BCD_B 3  
#define BCD_C 4  
#define BCD_D 5  

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

void send_to_bcd(int n){
    SET_BIT(PORTD, BCD_A, n&1);
    SET_BIT(PORTD, BCD_B, n&2);
    SET_BIT(PORTD, BCD_C, n&4);
    SET_BIT(PORTD, BCD_D, n&8);
}

void display_time(int h1, int h2, int m1, int m2, int s1, int s2){
    send_to_bcd(h1);
    HIGH_BIT(PORTB, H1);
    _delay_ms(0.25);
    LOW_BIT(PORTB, H1);

    send_to_bcd(h2);
    HIGH_BIT(PORTB, H2);
    _delay_ms(0.25);
    LOW_BIT(PORTB, H2);

    send_to_bcd(m1);
    HIGH_BIT(PORTB, M1);
    _delay_ms(0.25);
    LOW_BIT(PORTB, M1);

    send_to_bcd(m2);
    HIGH_BIT(PORTB, M2);
    _delay_ms(0.25);
    LOW_BIT(PORTB, M2);

    send_to_bcd(s1);
    HIGH_BIT(PORTB, S1);
    _delay_ms(0.25);
    LOW_BIT(PORTB, S1);

    send_to_bcd(s2);
    HIGH_BIT(PORTB, S2);
    _delay_ms(0.25);
    LOW_BIT(PORTB, S2);
}

int main(void){
    // Disable ADC (Convert Analog -> Digital)
    ADCSRA &= ~(1 << ADEN);

    DDRC = 0x00;
    PORTC = 0xFF; // Setting input pull up

    DDRB = 0xFF;
    DDRD = 0xFF;

    int h1 = 0;
    int h2 = 1;
    int m1 = 2;
    int m2 = 9;
    int s1 = 5;
    int s2 = 6;

    init_timer();

    while(1){
        if(flag){
            s2 = s2 + 1;
            if(s2 == 10) { s2 = 0; s1 = s1 + 1; }
            if(s1 == 6)  { s1 = 0; m2 = m2 + 1; }
            if(m2 == 10) { m2 = 0; m1 = m1 + 1; }
            if(m1 == 6)  { m1 = 0; h2 = h2 + 1; }
            if(h1 == 2 && h2 == 5) { h1 = 0; h2 = 0; m1 = 0; m2 = 0; s1 = 0; s2 = 0; }
            else if(h2 == 10) { h2 = 0; h1 = h1 + 1; }
            flag = 0;
        }
        

        display_time(h1, h2, m1, m2, s1, s2);
    }
}
