//
// Created by ádám on 2025. 11. 24..
//

#ifndef HASON_DYNARR_H
#define HASON_DYNARR_H

//
// Created by ádám on 2025. 11. 17..
//



#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/* DAL == Dynamically Allocated ArrayList */
#ifndef ARRLEN
#define ARRLEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

#if defined(__GNUC__) || defined(__clang__)
#define is_same(T, U) __builtin_types_compatible_p(T, U)
#define is_same_t(T, U) __builtin_types_compatible_p(T, U)
#else //fallback to _Generic (probably doesnt work properly, switch to normal compilers lol)
#define is_same(T, U) _Generic((T), __typeof__(U): 1, default: 0) /* works only on expressions, left-to-right */
    #define is_same_t(T, U) _Generic((T){0}, U: 1, default: 0) /* works only on types, left-to-right */
#endif

#if defined(__GNUC__) || defined(__clang__)
#define d_t(arr) __typeof__(*(arr).data)
#elif
#error "Either GCC or Clang is required for compilation."
#endif

#define DAL_INIT_CAP 32

#define d_tassert(arr, value, msg) _Static_assert(is_same(d_t(arr), __typeof__(value)), msg)

#define dchkcap(arr) \
do { \
        if ((arr).length >= (arr).capacity) { \
            if((arr).capacity == 0) (arr).capacity = DAL_INIT_CAP;\
            (arr).capacity = (arr).capacity ? (arr).capacity * 2 : 1; \
            (arr).data = realloc((arr).data, sizeof(*(arr).data) * (arr).capacity); \
        } \
    } while(0)

#define darr(type,...) \
    struct {                          \
        type *data;              \
        size_t length;                    \
        size_t capacity;                  \
    } __VA_ARGS__

#define ddnnew(arr) \
arr = {\
        .data = malloc(sizeof((arr).data) * DAL_INIT_CAP), \
        .length = 0, \
        .capacity = DAL_INIT_CAP \
}


#define dstnew(arr) \
arr = {\
        .data = NULL, \
        .length = 0, \
        .capacity = DAL_INIT_CAP \
}

#define dnewrsv(type, name, cap) \
    struct {                          \
        type *data;              \
        size_t length;                    \
        size_t capacity;                  \
    } name = {                       \
        malloc(sizeof(type) * (cap)),\
        0,                                \
        (cap)                    \
    }
#define FIRST_ARG(first, ...) first


#define dstinit(...) \
    do { \
        __typeof__(FIRST_ARG(__VA_ARGS__)) __internal_DSTINITBUF[] = {__VA_ARGS__}; \
        size_t __internal_DSTINITN = ARRLEN(__internal_DSTINITBUF); \
        for (size_t _D_i = 0; _D_i < __internal_DSTINITN; ++_D_i) { \
            __internal_DSTINITBUF[_D_i]->data = NULL; \
            __internal_DSTINITBUF[_D_i]->length = 0; \
            __internal_DSTINITBUF[_D_i]->capacity = DAL_INIT_CAP; \
        } \
    } while(0)

#define ddninit(...) \
    do { \
        __typeof__(FIRST_ARG(__VA_ARGS__)) __internal_DINITBUF[] = {__VA_ARGS__}; \
        size_t __internal_DINITN = ARRLEN(__internal_DINITBUF); \
        for (size_t __i = 0; __i < __internal_DINITN; ++__i) { \
            __internal_DINITBUF[__i]->data = malloc(sizeof(d_t(*__internal_DINITBUF[__i])) * DAL_INIT_CAP); \
            __internal_DINITBUF[__i]->length = 0; \
            __internal_DINITBUF[__i]->capacity = DAL_INIT_CAP; \
        } \
    } while(0)



#define dfrom(arr, ...) \
    do {                                                                        \
        d_t(arr) __internal_BUF[] = { __VA_ARGS__ };                          \
        size_t __internal_N = ARRLEN(__internal_BUF);                           \
        for (size_t __i = 0; __i < __internal_N; ++__i) {                       \
            dpush(arr, __internal_BUF[__i]);                                  \
        }                                                                       \
    } while (0)

#define dfree(arr) do { \
    free((arr).data); \
    (arr).data = NULL; \
    (arr).length = 0; \
    (arr).capacity = 0; \
} while(0)

#define dpush(arr, value) do { \
    d_tassert(arr, value, "Type mismatch in da_push"); \
    dchkcap(arr); \
    (arr).data[(arr).length++] = (value); \
} while(0)

#define drsv(arr, new_cap) do { \
    if ((new_cap) > (arr).capacity) { \
        (arr).capacity = (new_cap); \
        (arr).data = realloc((arr).data, sizeof(*(arr).data) * (arr).capacity); \
    } \
} while(0)

#define dpop(arr) do { if ((arr).length > 0) --(arr).length; } while(0)
#define dpopg(arr) ((arr).length > 0 ? (arr).data[--(arr).length] : 0)
#define dat(arr, pos) ((arr).length >= pos ? (arr).data[pos] : 0)




/***********************************************************************************
 * Macros for generating specialized macros for a specific GLOBAL arraylist
 * Doesnt work for local arrays because it defines functions
 * (except GCC probably which has nested functions and all of that nuanced stuff)
 ***********************************************************************************
 * Usage:
 *
 * dal_new(IntArr, int, 16); //creating a new arraylist of integers, with an initial capacity of 16
 * DAL_IMPLFOR(IntArr) //implementing the functions for IntArr
 *
 * int main() {
 *      //now they can be called like this:
 *      IntArr_push(30);
 *      int x = IntArr_pop_get(); // x = 30
 * }
 ***********************************************************************************/
#if 0 //unimplemented for now
#define DAL_IMPLFOR(arr) \
    inline void arr##_free(void) { \
        dal_free(arr); \
    } \
    inline void arr##_push(dal_t(arr) value) { \
        dal_push(arr, value); \
    } \
    inline void arr##_reserve(size_t new_cap) { \
        dal_reserve(arr, new_cap); \
    } \
    inline void arr##_pop(void) { \
        dal_pop(arr); \
    } \
    inline dal_t(arr) arr##_pop_get(void) { \
        return dal_pop_get(arr); \
    } \
    inline dal_t(arr) arr##_at(size_t pos) { \
        return dal_at(arr, pos); \
    }
#endif //implement_fns

#ifdef DYNARR_ADD_SUFFIXES
    #define DA_d_t d_t
    #define DA_d_tassert d_tassert
    #define DA_dchkcap dchkcap
    #define DA_dnew dnew
    #define DA_dnewcap dnewcap
    #define DA_dfrom dfrom
    #define DA_dfree dfree
    #define DA_dpush dpush
    #define DA_drsv drsv
    #define DA_dpop dpop
    #define DA_dpopg dpopg
    #define DA_dat dat
#endif //DYNARR_ADD_SUFFIXES

#endif //HASON_DYNARR_H
