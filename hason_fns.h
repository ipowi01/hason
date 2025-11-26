//
// Created by ádám on 2025. 11. 23..
//

#ifndef HASON_HASON_FNS_H
#define HASON_HASON_FNS_H

#include "dynarr.h"

typedef  double (*HSN_sfptr)(int, double*);

typedef struct HSN_fptr {
    HSN_sfptr fptr;
    int paramc;
} HSN_fptr;

typedef struct wordmap {
    char *word;
    HSN_fptr word_fn;
} wordmap_t;

extern darr(wordmap_t) dstnew(words);
double HSN_add(int paramc, double *paramv){
    double sum = 0;
    for(int i = 0; i < paramc; i++){
        sum += paramv[i];
    }
    return sum;
}
double HSN_sub(int paramc, double *paramv){
    double ret = paramv[0];
    for(int i = 1; i < paramc; i++){
        ret -= paramv[i];
    }
    return ret;
}
double HSN_mul(int paramc, double *paramv){
    double ret = paramv[0];
    for(int i = 1; i < paramc; i++){
        ret *= paramv[i];
    }
    return ret;
}
double HSN_div(int paramc, double *paramv){
    double ret = paramv[0];
    for(int i = 1; i < paramc; i++){
        ret /= paramv[i];
    }
    return ret;
}
double HSN_def(char *name, int paramc, double *paramv, HSN_sfptr eval) {
 /*
  * úgy akarom hogy működjön, hogy
  *
  * def avg_three
  *     take 3     // 3 paraméter - levesz hármat a stackről (majd körbeadja és már csak 2 üveg sör marad a stacken) //
  *     apply + + 3 /   // applyolja rá a műveleteket (a takeltekből substack? talán, vagy a take csak ellenőriz) //
  *     ; //definíció vége //
  *
  * így pl:
  *     def add take 2 apply +;
  *     def sub take 2 apply -;
  *     def mul take 2 apply *;
  *     def div take 2 apply /;
  */

  //nemtudom 😭😭
}

#endif //HASON_HASON_FNS_H
