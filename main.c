#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "dynarr.h"
#include "hason_fns.h"



void init_words() {
    ddninit(&words);
    dfrom(words,
          { "+", HSN_add, 2 },
          { "-", HSN_sub, 2 },
          { "*", HSN_mul, 2 },
          { "/", HSN_div, 2 }
          //{ "def", HSN_def }
    );
}

int is_word(char *token) { // a helyét is visszaadja, hogy ne kelljen kétszer hívni
    for(int i = 0; i < words.length; i++){
        if (strcmp(token, words.data[i].word) == 0) return i;
    }
    return -1; //nulladik eleme is van a listának, nemde? nem lehet 0
}
#define INPUTBUF_SIZE 64
#ifndef ARRLEN
#define ARRLEN(arr) sizeof(arr) / sizeof(arr[0])
#endif

/*typedef struct {
    double *data;
    size_t length;
    size_t capacity;
} STACK_T; */

int eval(int place, struct {double *data; size_t length; size_t capacity;} *stack){
    if (place < 0) return -1; //double-check
    double params[64] = {0};
    for(int i = 0; i < words.data[place].word_fn.paramc; i++){
        params[i] = dpopg(*stack);
    }
    double result = words.data[place].word_fn.fptr(words.data[place].word_fn.paramc, params);
    dpush(*stack, result);
}

int main(){
    init_words();
    dnewrsv(double, stack, 1000 * 1000);

    char input[INPUTBUF_SIZE];
    input[strcspn(input, "\n")] = '\0'; //nullterminálja a bekapott stringet

    char *token = strtok(input, " \t");
    while (token) {
        char *end;
        int wordplace;
        double maybe_val = strtod(token, &end);

        if (end != token && *end == '\0') { //szám
            dpush(stack, maybe_val);
        } else if ((wordplace = is_word(token)) != -1) { //szó
            eval(wordplace, &stack);
        } else { //anyád
        }

        token = strtok(NULL, " \t");
    }
}

#if 0

#include <iostream>
#include <stack>
#include <cstdarg>
#include <windows.h>

std::stack<double> szamok;
static const std::string operators = "*/-+";

double eval(double a, double b, char c){
    switch(c){
        case '*':
            return a*b;
        case '/':
            return a/b;
        case '+':
            return a+b;
        case '-':
            return a-b;
    }

}

double parser() {
    std::string input;
    do {
        std::cin >> input;
        if(std::isdigit(input[0])) szamok.push(std::stod(input));
        else if(operators.find(input) != std::string::npos && input.size() == 1) { //operátor-e
            double a = szamok.top();
            szamok.pop();
            double b = szamok.top();
            szamok.pop();
            szamok.push(eval(b, a, input[0]));
        }
        else if (input != "do"){
            std::cerr << "Invalid character" << '\n'; exit(1);
        }
    } while (input != "do");
    return szamok.top();
}

int main(){
    std::string input;
    std::cin >> input;
    if(input != "quit") std::cout << parser() << '\n';
}
#endif