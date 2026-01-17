#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcs.h"
#include <avr/eeprom.h>

#define STACK_SIZE 128
#define PI 3.14159265358979323846
#define E 2.7182818284
#define ADDRESS 0

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV,
    POW,
    FACT
} Op;

typedef enum {
    SIN,
    COS,
    TAN,
    LN,
    ARCSIN,
    ARCCOS,
    ARCTAN,
    LOG
} Func;

typedef enum {
    NUM,
    OP,
    LBRAK,
    RBRAK,
    FUNC
} TokenType;

typedef union {
    double num;
    Op op;
    Func func;
    unsigned present: 1;
} TokenVal;

typedef struct Token {
    TokenType type;
    TokenVal val;
} Token;

int precedence(Op op){
    if(op == ADD || op == SUB) return 1;
    else if(op == MUL || op == DIV) return 2;
    else if(op == POW) return 3;
    return -1;
}

void append(Token token_stream[STACK_SIZE], short* size, Token token){
    token_stream[*size] = token;
    *size = *size + 1; }

Token pop(Token token_stream[STACK_SIZE], short* size){
    Token popped_token = token_stream[*size - 1];
    *size = *size - 1;
    return popped_token;
}

double eval(char buf[STACK_SIZE], double ans){
    Token token_stream[STACK_SIZE];
    short size = 0;

    short i = 0;

    while(i < 64 && buf[i] != '\0'){
        Token token;
        char ch = buf[i];
        int skip = 0;
        
        if(ch == '('){
            token.type = LBRAK;
            token.val.present = 0;
            skip = 1;
        }
        else if(ch == ')'){
            token.type = RBRAK;
            token.val.present = 0;
            skip = 1;
        }
        else if(ch == '+'){
            token.type = OP;
            token.val.op = ADD;
            skip = 1;
        }
        else if(ch == '-'){
            token.type = OP;
            token.val.op = SUB;
            skip = 1;
        }
        else if(ch == '*'){
            token.type = OP;
            token.val.op = MUL;
            skip = 1;
        }
        else if(ch == '/'){
            token.type = OP;
            token.val.op = DIV;
            skip = 1;
        }
        else if(ch == '^'){
            token.type = OP;
            token.val.op = POW;
            skip = 1;
        }
        else if(ch == '!'){
            token.type = OP;
            token.val.op = FACT;
            skip = 1;
        }
        else if(ch == 'p'){
            token.type = NUM;
            token.val.num = (double) PI;
            skip = 1;
        }
        else if(ch == 'e'){
            token.type = NUM;
            token.val.num = (double) E;
            skip = 1;
        }
        else if(ch == 's'){
            token.type = FUNC;
            token.val.func = SIN;
            skip = 1;
        }
        else if(ch == 'c'){
            token.type = FUNC;
            token.val.func = COS;
            skip = 1;
        }
        else if(ch == 't'){
            token.type = FUNC;
            token.val.func = TAN;
            skip = 1;
        }
        else if(ch == 'l'){
            token.type = FUNC;
            token.val.func = LN;
            skip = 1;
        }
        else if(ch == 'L'){
            token.type = FUNC;
            token.val.func = LOG;
            skip = 1;
        }
        else if(ch == '@'){
            token.type = FUNC;
            token.val.func = ARCSIN;
            skip = 1;
        }
        else if(ch == '#'){
            token.type = FUNC;
            token.val.func = ARCCOS;
            skip = 1;
        }
        else if(ch == '$'){
            token.type = FUNC;
            token.val.func = ARCTAN;
            skip = 1;
        }
        else if(ch == 'M'){
            double mem = 0;
            eeprom_read_block((void*) &mem, (const void*) ADDRESS, sizeof(double));

            token.type = NUM;
            token.val.num = mem;
            skip = 1;
        }
        else {
            double val;
            sscanf(buf + i, "%lf %n", &val, &skip);
            token.type = NUM;
            token.val.num = val;
        }

        append(token_stream, &size, token);
        i += skip;
    }

    Token output_stack[STACK_SIZE];
    short output_size = 0;

    Token operator_stack[STACK_SIZE];
    short operator_size = 0;

    short is_brak = 0;
    
    for(int i = 0; i < size; i++){
        Token token = token_stream[i];

        if(token.type == NUM){
            append(output_stack, &output_size, token);
        }
        else if(token.type == OP){
            if(operator_size > 0 && !is_brak){
                Token op_token = operator_stack[operator_size - 1];
                if(precedence(token.val.op) <= precedence(op_token.val.op)){
                    while(operator_size > 0){
                        append(output_stack, &output_size, pop(operator_stack, &operator_size));
                    }
                }
            }

            append(operator_stack, &operator_size, token);
        }
        else if(token.type == LBRAK){
            append(operator_stack, &operator_size, token);
            is_brak = 1;
        }
        else if(token.type == RBRAK){
            while(operator_stack[operator_size].type != LBRAK) {
                append(output_stack, &output_size, pop(operator_stack, &operator_size));
            }
            is_brak = 0;
        }
        else if(token.type == FUNC){
            append(operator_stack, &operator_size, token);
        }
    }

    while(operator_size > 0){
        append(output_stack, &output_size, pop(operator_stack, &operator_size));
    }

    Token res_stack[STACK_SIZE];
    short res_size = 0;
    short is_ans = 1;

    for(int i = 0; i < output_size; i++){
        Token token = output_stack[i];
        if(token.type == NUM){
            append(res_stack, &res_size, token);
        }
        else if(token.type == OP){
            Token right = pop(res_stack, &res_size);
            if(token.val.op == FACT) {
                Token res_token;
                res_token.type = NUM;
                res_token.val.num = factorial(right.val.num);
                is_ans = 0;

                append(res_stack, &res_size, res_token);
                continue;
            }

            Token left;

            if(res_size == 0 && is_ans){
                left.type = NUM;
                left.val.num = ans;
            }
            else if(res_size == 0){
                left.type = NUM;
                left.val.num = 0;
            }
            else {
                left = pop(res_stack, &res_size);
            }

            double res = left.val.num;
            if(token.val.op == ADD) res = res + right.val.num;
            else if(token.val.op == SUB) res = res - right.val.num;
            else if(token.val.op == MUL) res = res * right.val.num;
            else if(token.val.op == DIV) res = res / right.val.num;
            else if(token.val.op == POW) res = pow(res, right.val.num);

            Token res_token;
            res_token.type = NUM;
            res_token.val.num = res;
            is_ans = 0;

            append(res_stack, &res_size, res_token);
        }
        else if(token.type == FUNC){
            Token res_token = pop(res_stack, &res_size);
            if(token.val.func == SIN) res_token.val.num = sin(res_token.val.num);
            else if(token.val.func == COS) res_token.val.num = cos(res_token.val.num);
            else if(token.val.func == TAN) res_token.val.num = tan(res_token.val.num);
            else if(token.val.func == LN) res_token.val.num = ln(res_token.val.num);
            else if(token.val.func == LOG) res_token.val.num = ln(res_token.val.num)/2.30258509299; // denom is ln(10)
            else if(token.val.func == ARCSIN) res_token.val.num = arcsin(res_token.val.num);
            else if(token.val.func == ARCCOS) res_token.val.num = arccos(res_token.val.num);
            else if(token.val.func == ARCTAN) res_token.val.num = arctan(res_token.val.num);
            append(res_stack, &res_size, res_token);
        }
    }

    return res_stack[0].val.num;
}
