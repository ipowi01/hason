//
// Created by ádám on 2025. 11. 24..
//
#include <stdio.h>
#include "dynarr.h"
int main(){
    darr(int) apple, pear;
    dnewrsv(int, x, 100);
    ddninit(&apple, &pear);
    dfrom(apple, 7, 8, 17, 231, 12231);
    dfrom(pear, 382747, 3, 21, 3);

    printf("%d %d %d", dat(apple, 0), dat(pear, 0), dat(pear, 2));

}