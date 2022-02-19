#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <inttypes.h>
#include <math.h>
#include <gmp.h>
//gcc ./FAST_REM_VS_MOD_OPERATOR/fastVsSlowMod.c -o ./FAST_REM_VS_MOD_OPERATOR/fastVsSlowMod -lgmp
//gcc ./FAST_REM_VS_MOD_OPERATOR/fastVsSlowMod.c -o ./FAST_REM_VS_MOD_OPERATOR/fastVsSlowMod -lgmp -O3

const uint32_t d2 = 2; // your divisor > 0
// c = ceil( (1<<64) / d ) ; we take L = N
const uint64_t c2 = UINT64_C(0xFFFFFFFFFFFFFFFF) / d2 + 1;

const uint32_t d3 = 3; // your divisor > 0
// c = ceil( (1<<64) / d ) ; we take L = N
const uint64_t c3 = UINT64_C(0xFFFFFFFFFFFFFFFF) / d3 + 1;

const uint32_t d5 = 5; // your divisor > 0
// c = ceil( (1<<64) / d ) ; we take L = N
static const uint64_t c5 = UINT64_C(0xFFFFFFFFFFFFFFFF) / d5 + 1;

const uint32_t d7 = 7; // your divisor > 0
// c = ceil( (1<<64) / d ) ; we take L = N
const uint64_t c7 = UINT64_C(0xFFFFFFFFFFFFFFFF) / d7 + 1;

const uint32_t d11 = 11; // your divisor > 0
// c = ceil( (1<<64) / d ) ; we take L = N
const uint64_t c11 = UINT64_C(0xFFFFFFFFFFFFFFFF) / d11 + 1;

const uint32_t d17 = 17; // your divisor > 0
// c = ceil( (1<<64) / d ) ; we take L = N
const uint64_t c17 = UINT64_C(0xFFFFFFFFFFFFFFFF) / d17 + 1;


static inline uint32_t slow_mod2(uint32_t num)
{
    return num % d2;
}

static inline uint32_t slow_mod3(uint32_t num)
{
    return num % d3;
}

static inline uint32_t slow_mod5(uint32_t num)
{
    return num % d5;
}

static inline uint32_t slow_mod7(uint32_t num)
{
    return num % d7;
}

static inline uint32_t slow_mod11(uint32_t num)
{
    return num % d11;
}

static inline uint32_t slow_mod17(uint32_t num)
{
    return num % d17;
}

// num mod 3
//  must be precomputed
static inline uint32_t fast_mod2(uint32_t num)
{
    uint64_t lowbits = c2 * num;
    return ((__uint128_t)lowbits * d2) >> 64;
}

static inline uint32_t fast_mod3(uint32_t num)
{
    uint64_t lowbits = c3 * num;
    return ((__uint128_t)lowbits * d3) >> 64;
}

static inline uint32_t fast_mod5(uint32_t num)
{
    uint64_t lowbits = c5 * num;
    return ((__uint128_t)lowbits * d5) >> 64;
}

static inline uint32_t fast_mod7(uint32_t num)
{
    uint64_t lowbits = c7 * num;
    return ((__uint128_t)lowbits * d7) >> 64;
}

static inline uint32_t fast_mod11(uint32_t num)
{
    uint64_t lowbits = c11 * num;
    return ((__uint128_t)lowbits * d11) >> 64;
}

static inline uint32_t fast_mod17(uint32_t num)
{
    uint64_t lowbits = c17 * num;
    return ((__uint128_t)lowbits * d17) >> 64;
}

static inline long get_nanos()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

int main(void)
{
    // Initialize seed
    unsigned long int seed = 0UL;
    // Initialize state with default algorithm
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);
    mp_bitcnt_t nBits = 31;
    mpz_t rNumMpz;
    mpz_init(rNumMpz);

    long time_mod2_operator = 0UL;
    long time_fast2_rem = 0UL;
    long time_mod3_operator = 0UL;
    long time_fast3_rem = 0UL;

    long time_mod5_operator = 0UL;
    long time_fast5_rem = 0UL;

    long time_mod7_operator = 0UL;
    long time_fast7_rem = 0UL;

    long time_mod11_operator = 0UL;
    long time_fast11_rem = 0UL;

    long time_mod17_operator = 0UL;
    long time_fast17_rem = 0UL;

    long stop_time;
    long start_time;

    uint32_t rNum;
    uint32_t isDiv;

    for (int i = 0; i < 10000; i++)
    {
        rNum = 0U;
        mpz_rrandomb(rNumMpz, state, nBits);
        char *temp = mpz_get_str(NULL, 10, rNumMpz);
        rNum = strtoul(temp, NULL, 10);
        free(temp);

        start_time = get_nanos();
        // Use operator
        isDiv = slow_mod2(rNum);
        stop_time = get_nanos();
        time_mod2_operator += stop_time - start_time;

        start_time = get_nanos();
        isDiv = fast_mod2(rNum);
        stop_time = get_nanos();
        time_fast2_rem += stop_time - start_time;
    }

    for (int i = 0; i < 10000; i++)
    {
        rNum = 0U;
        // printf("\nROUND #%d\n", i);
        mpz_rrandomb(rNumMpz, state, nBits);
        char *temp = mpz_get_str(NULL, 10, rNumMpz);
        rNum = strtoul(temp, NULL, 10);
        free(temp);
        // printf("For ui31bit: %" PRIu32 "\n", rNum);

        start_time = get_nanos();
        // Use operator
        isDiv = slow_mod3(rNum);
        stop_time = get_nanos();
        time_mod3_operator += stop_time - start_time;
        // printf("Operator: %ld\n", time_mod3_operator);

        start_time = get_nanos();
        isDiv = fast_mod3(rNum);
        stop_time = get_nanos();
        time_fast3_rem += stop_time - start_time;
        // printf("FastRem: %ld\n", time_fast3_rem);
    }

    for (int i = 0; i < 10000; i++)
    {
        rNum = 0U;
        mpz_rrandomb(rNumMpz, state, nBits);
        char *temp = mpz_get_str(NULL, 10, rNumMpz);
        rNum = strtoul(temp, NULL, 10);
        free(temp);

        start_time = get_nanos();
        // Use operator
        isDiv = slow_mod5(rNum);
        stop_time = get_nanos();
        time_mod5_operator += stop_time - start_time;

        start_time = get_nanos();
        isDiv = fast_mod5(rNum);
        stop_time = get_nanos();
        time_fast5_rem += stop_time - start_time;
    }

    for (int i = 0; i < 10000; i++)
    {
        rNum = 0U;
        mpz_rrandomb(rNumMpz, state, nBits);
        char *temp = mpz_get_str(NULL, 10, rNumMpz);
        rNum = strtoul(temp, NULL, 10);
        free(temp);

        start_time = get_nanos();
        // Use operator
        isDiv = slow_mod7(rNum);
        stop_time = get_nanos();
        time_mod7_operator += stop_time - start_time;

        start_time = get_nanos();
        isDiv = fast_mod7(rNum);
        stop_time = get_nanos();
        time_fast7_rem += stop_time - start_time;
    }

    for (int i = 0; i < 10000; i++)
    {
        rNum = 0U;
        mpz_rrandomb(rNumMpz, state, nBits);
        char *temp = mpz_get_str(NULL, 10, rNumMpz);
        rNum = strtoul(temp, NULL, 10);
        free(temp);

        start_time = get_nanos();
        // Use operator
        isDiv = slow_mod11(rNum);
        stop_time = get_nanos();
        time_mod11_operator += stop_time - start_time;

        start_time = get_nanos();
        isDiv = fast_mod11(rNum);
        stop_time = get_nanos();
        time_fast11_rem += stop_time - start_time;
    }

    for (int i = 0; i < 10000; i++)
    {
        rNum = 0U;
        mpz_rrandomb(rNumMpz, state, nBits);
        char *temp = mpz_get_str(NULL, 10, rNumMpz);
        rNum = strtoul(temp, NULL, 10);
        free(temp);

        start_time = get_nanos();
        // Use operator
        isDiv = slow_mod17(rNum);
        stop_time = get_nanos();
        time_mod17_operator += stop_time - start_time;

        start_time = get_nanos();
        isDiv = fast_mod17(rNum);
        stop_time = get_nanos();
        time_fast17_rem += stop_time - start_time;
    }

    printf("\n****************************************************************************************************************************\n");
    printf("COMPARING THE COMPUTAIONAL TIME OF 31BIT RANDOM NUMBERS\n[SAMPLE SIZE: 10000 iterations per divisor]\n\t\t>Fast division uses the remainder method\n\t\t>Slow division uses the built-in mod operator");
    printf("\n****************************************************************************************************************************\n");
    printf("[METHOD OF DIV]\t\t[FAST DIVISON TIME(ns)]\t\t[SLOW DIVISION TIME(ns)]\n");
    printf("      DIVISOR 2\t\t%ld\t\t\t\t%ld\n", time_fast3_rem, time_mod3_operator);
    printf("      DIVISOR 5\t\t%ld\t\t\t\t%ld\n", time_fast5_rem, time_mod5_operator);
    printf("      DIVISOR 7\t\t%ld\t\t\t\t%ld\n", time_fast7_rem, time_mod7_operator);
    printf("     DIVISOR 11\t\t%ld\t\t\t\t%ld\n", time_fast11_rem, time_mod11_operator);
    printf("     DIVISOR 17\t\t%ld\t\t\t\t%ld\n", time_fast17_rem, time_mod17_operator);
    printf("****************************************************************************************************************************\n");

    mpz_clear(rNumMpz);
    return 0;
}
