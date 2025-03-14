#include <avr/io.h> 
#include <util/delay.h> 
#include <stdlib.h> 

// ------------------
// TYPEDEFS
typedef uint8_t byte;

// ------------------
// MACROS
// LOW_BIT -> Sets bit number 'Y' on register 'X' to 0
#define LOW_BIT(X, Y) X &= ~_BV(Y)
// HIGH_BIT -> Sets bit number 'Y' on register 'X' to 1
#define HIGH_BIT(X, Y) X |= _BV(Y)

// SET_BIT -> Sets bit number 'Y' on register 'X' to VAL
#define SET_BIT(X, Y, VAL) (VAL) ? (HIGH_BIT(X, Y)): (LOW_BIT(X, Y))

// POWER (seven segment) pins
#define P1 3
#define P2 2
#define P3 1
#define P4 0

// DATA (BCD) pins
#define DA 2
#define DB 3
#define DC 4
#define DD 5


//extern void send_to_bcd(byte val);

// instruction time -> 
void send_to_bcd(int n){
    for(int i = 0; i < 4; i++){
        SET_BIT(PORTD, i + 2, (n&1));
        n = (n >> 1);
    }
}

void show_time(int h1, int h2, int m1, int m2){
    send_to_bcd(h1);
    HIGH_BIT(PORTB, P1);
    _delay_ms(1.25);
    LOW_BIT(PORTB, P1);

    send_to_bcd(h2);
    HIGH_BIT(PORTB, P2);
    _delay_ms(1.25);
    LOW_BIT(PORTB, P2);

    send_to_bcd(m1);
    HIGH_BIT(PORTB, P3);
    _delay_ms(1.25);
    LOW_BIT(PORTB, P3);

    send_to_bcd(m2);
    HIGH_BIT(PORTB, P4);
    _delay_ms(1.25);
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
    // use PortB for LCD interface
    DDRB = 0xFF; // 1111.1111; set PB0-PB7 as outputs	 
    DDRD = 0xFF; // 1111.1111; set PD0-PD7 as outputs	 

    int t = 0;

    int h1 = 0;
    int h2 = 0;
    int m1 = 0;
    int m2 = 0;

    while(1){
        if(t > 200){
            increment_time(&h1, &h2, &m1, &m2);
            t = 0;
        }

        show_time(h1, h2, m1, m2);
        t++;
    }
}
