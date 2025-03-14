#include <avr/io.h> 
#include <util/delay.h> 
#include <stdlib.h> 

// ------------------
// TYPEDEFS
typedef uint8_t byte;

// ------------------
// MACROS
// CLR_BIT -> Clears bit number 'Y' on register 'X'
#define CLR_BIT(X, Y) X &= ~_BV(Y)
// SET_BIT -> Sets bit number 'Y' on register 'X'
#define SET_BIT(X, Y) X |= _BV(Y)
// LCD_RS -> R/S (Register Select)
#define LCD_RS 0 // PB0
// LCD_E -> LCD Enable pin
#define LCD_E 1 // PB1
// DATA PINS
#define DB4 2 // pin for DB4
#define DB5 3 // pin for DB5
#define DB6 4 // pin for DB6
#define DB7 5 // pin for DB7
// CLEAR_DISPLAY -> Instruction for clearing display
#define CLEAR_DISPLAY 0x01

void pulse_enable_line(){
    SET_BIT(PORTB, LCD_E); // take LCD enable line high
    _delay_us(40); // wait 40 microseconds
    CLR_BIT(PORTB, LCD_E); // take LCD enable line low
}

void send_nibble(byte data){
    PORTB &= 0xC3; // 1100.0011 = clear 4 data lines
    if (data & _BV(4)) SET_BIT(PORTB, DB4);
    if (data & _BV(5)) SET_BIT(PORTB, DB5);
    if (data & _BV(6)) SET_BIT(PORTB, DB6);
    if (data & _BV(7)) SET_BIT(PORTB, DB7);
    pulse_enable_line(); // clock 4 bits into controller
}

void send_byte(byte data){
    send_nibble(data); // send upper 4 bits
    send_nibble(data << 4); // send lower 4 bits
    CLR_BIT(PORTB, 5); // turn off PIN 13 (Clk i guess)
}

void lcd_cmd(byte cmd){
    CLR_BIT(PORTB, LCD_RS); // R/S line 0 = command data
    send_byte(cmd); // send it
}

void lcd_char(byte ch){
    SET_BIT(PORTB, LCD_RS); // R/S line 1 = character data
    send_byte(ch); // send it
}

void lcd_init(){
    lcd_cmd(0x33); // initialize controller
    lcd_cmd(0x32); // set to 4-bit input mode
    lcd_cmd(0x28); // 2 line, 5x7 matrix
    lcd_cmd(0x0E); // turn cursor off (0x0E to enable)
    lcd_cmd(0x06); // cursor direction = right
    lcd_cmd(0x01); // start with clear display
    _delay_ms(3); // wait for LCD to initialize
}

void lcd_clear() {
    lcd_cmd(CLEAR_DISPLAY);
    _delay_ms(3); // wait for LCD to process command
}

void lcd_msg(const char *text){
    while (*text) 
        lcd_char(*text++); // send char & update char pointer
}

void lcd_int(int data){
    char st[8] = ""; // save enough space for result
    itoa(data,st,10); // convert integer to ascii 
    lcd_msg(st); // display in on LCD
}

void clear_button_states(int n, int button_states[n], int i){
    for(int j = 0; j < n; j++){
            if(i == j) continue;
            button_states[j] = 0;
    }
}

int which_button(int n, int button_states[n], int *time_passed){
    for(int i = 0; i < n; i++){
        if(!(PIND & (1 << 7 - i)) && !(button_states[i])){
            button_states[i] = 1;
            clear_button_states(n, button_states, i);
            *time_passed = 0;
            return i;
        }
    }

    return -1;
}

void switch_off_idle_buttons(int n, int button_states[n], int time_passed){
    if(time_passed <= 1) return;
    clear_button_states(n, button_states, -1);
}

float eval(char str[64]){
    char buf[64];
    int op;
    while(*str){
        if(*str == '+'){
            op = 0;
        }
        *str += 1;
    }

}

int main(void){
    // use PortB for LCD interface
    DDRB = 0xFF; // 1111.1111; set PB0-PB7 as outputs	 
    DDRD = 0x00; // 0000.0000; set PD0-PD7 as inputs
    PORTD = 0xFF;

    lcd_init(); // initialize LCD controller
    int n = 2;
    int button_states[2] = {0};
    int time_passed = 0;
    char str[64] = {'\0'};
    int pos = 0;

    while(1){
        lcd_clear();

        int button = which_button(n, button_states, &time_passed);

        for(int i = 0; i < n; i++){
            if(button_states[i] == 1 && time_passed == 0){
                //lcd_clear();
                //lcd_int(i + 1);
                str[pos] = (char) (i + '0');
                pos += 1;
                //break;
            }
        }
    

        lcd_clear();

        if(pos > 15) lcd_msg(str + pos - 15);
        else lcd_msg(str);

        switch_off_idle_buttons(n, button_states, time_passed);

        time_passed += 1;
        _delay_ms(50);     // set animation speed
    }
}
