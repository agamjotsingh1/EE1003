#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    ADD,
    SUB,
    MUL,
    DIV
} Op;

typedef enum {
    NUM,
    OP,
    LBRAK,
    RBRAK
} TokenType;

typedef union {
    double num;
    Op op;
    unsigned present: 1;
} TokenVal;

typedef struct Token {
    TokenType type;
    TokenVal val;
} Token;

int precedence(Op op){
    if(op == ADD || op == SUB) return 1;
    else if(op == MUL || op == DIV) return 2;
    return -1;
}

void append(Token** token_stream, short* size, Token token){
    if(*size == 0) *token_stream = (Token *) malloc(sizeof(Token));
    else *token_stream = (Token *) realloc(*token_stream, sizeof(Token) * (*size + 1));

    (*token_stream)[*size] = token;
    *size = *size + 1;
}

Token pop(Token** token_stream, short* size){
    Token popped_token = (*token_stream)[*size - 1];
    //free(*token_stream + *size - 1);
    *token_stream = (Token *) realloc(*token_stream, sizeof(Token) * (*size - 1));
    *size = *size - 1;
    return popped_token;
}

void print_tok_stream(Token* token_stream, short size){
    for(int i = 0; i < size; i++){
        Token token = token_stream[i];
        if(token.type == NUM){
            printf("%lf", token.val.num);
        }
        else if(token.type == OP){
            if(token.val.op == ADD) printf("+");
            else if(token.val.op == SUB) printf("-");
            else if(token.val.op == MUL) printf("*");
            else if(token.val.op == DIV) printf("/");
        }
    }
}

double eval(char buf[64], double ans){
    Token* token_stream;
    short size = 0;
    short i = 0;

    while(i < 64 && buf[i] != '\0'){
        Token token;
        char ch = buf[i];
        int skip = 0;
        int is_int = 0;
        
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
        else {
            double val;
            sscanf(buf + i, "%lf %n", &val, &skip);
            token.type = NUM;
            token.val.num = val;
            is_int = 1;
        }

        if(size == 0) token_stream = (Token *) malloc(sizeof(Token));
        else token_stream = (Token *) realloc(token_stream, sizeof(Token) * (size + 1));

        token_stream[size] = token;
        size += 1; 
        i += skip;
    }

    Token* output_stack;
    short output_size = 0;

    Token* operator_stack;
    short operator_size = 0;
    
    for(int i = 0; i < size; i++){
        Token token = token_stream[i];

        if(token.type == NUM){
            append(&output_stack, &output_size, token);
        }
        else if(token.type == OP){
            if(operator_size > 0){
                Token op_token = operator_stack[operator_size - 1];
                if(precedence(token.val.op) <= precedence(op_token.val.op)){
                    while(operator_size > 0){
                        append(&output_stack, &output_size, pop(&operator_stack, &operator_size));
                    }
                }
            }
            append(&operator_stack, &operator_size, token);
        }
    }

    while(operator_size > 0){
        append(&output_stack, &output_size, pop(&operator_stack, &operator_size));
    }

    free(operator_stack);
    Token* res_stack;
    short res_size = 0;

    for(int i = 0; i < output_size; i++){
        Token token = output_stack[i];
        if(token.type == NUM){
            append(&res_stack, &res_size, token);
        }
        else if(token.type == OP){
            Token right = pop(&res_stack, &res_size);

            Token left;

            if(res_size == 0){
                left.type = NUM;
                left.val.num = ans;
            }
            else {
                left = pop(&res_stack, &res_size);
            }

            double res = left.val.num;
            if(token.val.op == ADD) res = res + right.val.num;
            else if(token.val.op == SUB) res = res - right.val.num;
            else if(token.val.op == MUL) res = res * right.val.num;
            else if(token.val.op == DIV) res = res / right.val.num;

            Token res_token;
            res_token.type = NUM;
            res_token.val.num = res;

            append(&res_stack, &res_size, res_token);
        }
    }

    return res_stack[0].val.num;
}

int main(){
    char buf[64] = {'\0'};
    strcpy(buf, "+1.8/2");
    eval(buf, 3);
    return 0;
}
