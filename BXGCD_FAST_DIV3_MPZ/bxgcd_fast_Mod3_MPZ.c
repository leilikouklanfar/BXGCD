#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include <time.h>
#include <math.h>
#include <inttypes.h>

long bxgcd(int x, int y);
long bxgcd_div3_after_removing_power2(int x, int y);
long bxgcd_div3_in_removing_power2(int x, int y);
long bxgcd_div3_in_and_after_removing_power2(int x, int y);
long bxgcdDiv3_replace_while_with_if(int x, int y);
const uint32_t d3 = 3; // your divisor > 0
// c = ceil( (1<<64) / d ) ; we take L = N
const uint64_t c3 = UINT64_C(0xFFFFFFFFFFFFFFFF) / d3 + 1;

static inline uint32_t fast_mod3(uint32_t num)
{
    uint64_t lowbits = c3 * num;
    return ((__uint128_t)lowbits * d3) >> 64;
}

static inline long get_nanos()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

// Idea for code for the BXGCD is from the HOAC
int main()
{
    long bxgcd_run_time = 0UL;
    long bxgcdDiv3_after_removing2_run_time = 0UL;
    long bxgcdDiv3_in_removing_power2_run_time = 0UL;
    long bxgcdDiv3_in_and_after_2loop_run_time = 0UL;
    long bxgcdDiv3_replace_while_with_if_run_time = 0UL;
    int sampleSize = 1000000;
    int divCtr = 0;

    mp_bitcnt_t nBits = 31;
    unsigned long int seed = 0UL;
    gmp_randstate_t state;
    // Initialize state with default algorithm
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);

    // Initialize mp numbers
    mpz_t rNumMpz1;
    mpz_init(rNumMpz1);
    mpz_t rNumMpz2;
    mpz_init(rNumMpz2);

    for (int i = 0; i < sampleSize; i++)
    {
        // GET THE FIRST RANDOM NUMBER
        mpz_rrandomb(rNumMpz1, state, nBits);

        int rNum1 = (int)mpz_get_ui(rNumMpz1);

        // free(temp);
        //  GET THE SECOND RANDOM NUMBER
        mpz_rrandomb(rNumMpz2, state, nBits);
        int rNum2 = (int)mpz_get_ui(rNumMpz2);
        // free(temp);

        int smallerNum = rNum1;
        if (rNum1 < rNum2)
        {
            rNum1 = rNum2;
            rNum2 = smallerNum;
        }

        bxgcd_run_time += bxgcd(rNum1, rNum2);
        bxgcdDiv3_after_removing2_run_time += bxgcd_div3_after_removing_power2(rNum1, rNum2);
        bxgcdDiv3_in_removing_power2_run_time += bxgcd_div3_in_removing_power2(rNum1, rNum2);
        bxgcdDiv3_in_and_after_2loop_run_time += bxgcd_div3_in_and_after_removing_power2(rNum1, rNum2);
        bxgcdDiv3_replace_while_with_if_run_time += bxgcdDiv3_replace_while_with_if(rNum1, rNum2);
        //  r_x = r_y;
    }
    // printf("\nrGCD: %d\nNum1: %d\nrNum2: %d\ns: %d\nt: %d", r,rNum1, rNum2, s2, t2);

    printf("\n****************************************************************************************************************************\n");
    printf("EXTENDED BXGCD RUNTIME FOR COMPUTING THE BXGCD OF TWO 31-BIT NUMBERS\n[SAMPLE SIZE: 10000 iterations per divisor]");
    printf("\n****************************************************************************************************************************\n");
    printf("Total Run Time\n\nBXGCD:                                        %lu nanosecond\n\n", bxgcd_run_time);
    printf("(1) BXGCD Diviving by 3 after loop to rm pow2:    %lu nanosecond\n", bxgcdDiv3_after_removing2_run_time);
    printf("(2) BXGCD Diviving by 3 in loop to rm pow2:       %lu nanosecond (<- FASTER)\n", bxgcdDiv3_in_removing_power2_run_time);
    printf("(3) BXGCD Diviving by 3 in&after loop to rm pow2: %lu nanosecond (<- SOMETIMES FASTER)\n", bxgcdDiv3_in_and_after_2loop_run_time);
    printf("(4) BXGCD Diviving by 3 replace while with if:    %lu nanosecond (<- LESS OFTEN FASTER)\n", bxgcdDiv3_replace_while_with_if_run_time);
    printf("****************************************************************************************************************************\n");

    mpz_clear(rNumMpz1);
    mpz_clear(rNumMpz2);
    return 0;
}
/**
 * ************************************************************
 */
long bxgcd(int x, int y)
{
    long start_time, stop_time;
    int g, u, s1, t1, s2, t2, v;

    // x = 693;
    // y = 609;
    //  printf("\na: %d\nb: %d", a, b);
    start_time = get_nanos();
    g = 1;
    // For both to be even, isEven will return
    while ((x % 2 == 0) && (y % 2 == 0))
    {
        x = x * (0.5);
        y = y * (0.5);
        g = 2 * g;
    }
    u = x;
    v = y;
    s1 = 1;
    t1 = 0;
    s2 = 0;
    t2 = 1;
    while (u != 0)
    {
        if ((u % 2) == 0)
        {
            u = u * (0.5);
            if ((s1 % 2 == 0) && (t1 % 2 == 0))
            {
                s1 = s1 * (0.5);
                t1 = t1 * (0.5);
            }
            else
            {
                s1 = (s1 + y) * (0.5);
                t1 = (t1 - x) * (0.5);
            }
        }
        else if ((v % 2) == 0)
        {
            v = v * (0.5);
            if ((s2 % 2 == 0) && (t2 % 2 == 0))
            {
                s2 = s2 * (0.5);
                t2 = t2 * (0.5);
            }
            else
            {
                s2 = (s2 + y) * (0.5);
                t2 = (t2 - x) * (0.5);
            }
        }

        if (u >= v)
        {
            u = u - v;
            s1 = s1 - s2;
            t1 = t1 - t2;
        }
        else
        {
            v = v - u;
            s2 = s2 - s1;
            t2 = t2 - t1;
        }
    }

    v = v * g;
    stop_time = get_nanos();
    // printf("\ns: %d\nt: %d\ngcd: %d", s2, t2, v);
    return stop_time - start_time;
}
/**
 * ************************************************************
 */
long bxgcd_div3_after_removing_power2(int x, int y)
{
    long start_time, stop_time;
    int g, u, s1, t1, s2, t2, v;

    start_time = get_nanos();
    g = 1;
    while ((x % 2 == 0) && (y % 2 == 0))
    {
        x = x * (0.5);
        y = y * (0.5);
        g = 2 * g;
    }
    while ((fast_mod3(x) == 0) && (fast_mod3(y) == 0))
    {
        x = x / 3;
        y = y / 3;
        g = g * 3;
    }
    u = x;
    v = y;
    s1 = 1;
    t1 = 0;
    s2 = 0;
    t2 = 1;
    while (u != 0)
    {
        if ((u % 2) == 0)
        {
            u = u * (0.5);
            if ((s1 % 2 == 0) && (t1 % 2 == 0))
            {
                s1 = s1 * (0.5);
                t1 = t1 * (0.5);
            }
            else
            {
                s1 = (s1 + y) * (0.5);
                t1 = (t1 - x) * (0.5);
            }
        }
        else if ((v % 2) == 0)
        {
            v = v * (0.5);
            if ((s2 % 2 == 0) && (t2 % 2 == 0))
            {
                s2 = s2 * (0.5);
                t2 = t2 * (0.5);
            }
            else
            {
                s2 = (s2 + y) * (0.5);
                t2 = (t2 - x) * (0.5);
            }
        }

        if (u >= v)
        {
            u = u - v;
            s1 = s1 - s2;
            t1 = t1 - t2;
        }
        else
        {
            v = v - u;
            s2 = s2 - s1;
            t2 = t2 - t1;
        }
    }
    v = v * g;
    stop_time = get_nanos();
    return stop_time - start_time;
}
/**
 * ************************************************************
 */
long bxgcd_div3_in_removing_power2(int x, int y)
{
    long start_time, stop_time;
    int g, u, s1, t1, s2, t2, v;

    start_time = get_nanos();
    g = 1;
    while ((x % 2 == 0) && (y % 2 == 0))
    {
        x = x * (0.5);
        y = y * (0.5);
        g = 2 * g;

        while ((fast_mod3(x) == 0) && (fast_mod3(y) == 0))
        {
            x = x / 3;
            y = y / 3;
            g = g * 3;
        }
    }
    u = x;
    v = y;
    s1 = 1;
    t1 = 0;
    s2 = 0;
    t2 = 1;
    while (u != 0)
    {
        if ((u % 2) == 0)
        {
            u = u * (0.5);
            if ((s1 % 2 == 0) && (t1 % 2 == 0))
            {
                s1 = s1 * (0.5);
                t1 = t1 * (0.5);
            }
            else
            {
                s1 = (s1 + y) * (0.5);
                t1 = (t1 - x) * (0.5);
            }
        }
        else if ((v % 2) == 0)
        {
            v = v * (0.5);
            if ((s2 % 2 == 0) && (t2 % 2 == 0))
            {
                s2 = s2 * (0.5);
                t2 = t2 * (0.5);
            }
            else
            {
                s2 = (s2 + y) * (0.5);
                t2 = (t2 - x) * (0.5);
            }
        }

        if (u >= v)
        {
            u = u - v;
            s1 = s1 - s2;
            t1 = t1 - t2;
        }
        else
        {
            v = v - u;
            s2 = s2 - s1;
            t2 = t2 - t1;
        }
    }

    v = v * g;
    stop_time = get_nanos();
    // printf("\ns: %d\nt: %d\ngcd: %d", s2, t2, v);
    return stop_time - start_time;
}
/**
 * ************************************************************
 */
long bxgcd_div3_in_and_after_removing_power2(int x, int y)
{
    long start_time, stop_time;
    int g, u, s1, t1, s2, t2, v;

    start_time = get_nanos();
    g = 1;
    while ((x % 2 == 0) && (y % 2 == 0))
    {
        x = x * (0.5);
        y = y * (0.5);
        g = 2 * g;

        while ((fast_mod3(x) == 0) && (fast_mod3(y) == 0))
        {
            x = x / 3;
            y = y / 3;
            g *= 3;
        }
    }

    while ((fast_mod3(x) == 0) && (fast_mod3(y) == 0))
    {
        x = x / 3;
        y = y / 3;
        g = g * 3;
    }
    u = x;
    v = y;
    s1 = 1;
    t1 = 0;
    s2 = 0;
    t2 = 1;
    while (u != 0)
    {
        if ((u % 2) == 0)
        {
            u = u * (0.5);
            if ((s1 % 2 == 0) && (t1 % 2 == 0))
            {
                s1 = s1 * (0.5);
                t1 = t1 * (0.5);
            }
            else
            {
                s1 = (s1 + y) * (0.5);
                t1 = (t1 - x) * (0.5);
            }
        }
        else if ((v % 2) == 0)
        {
            v = v * (0.5);
            if ((s2 % 2 == 0) && (t2 % 2 == 0))
            {
                s2 = s2 * (0.5);
                t2 = t2 * (0.5);
            }
            else
            {
                s2 = (s2 + y) * (0.5);
                t2 = (t2 - x) * (0.5);
            }
        }

        if (u >= v)
        {
            u = u - v;
            s1 = s1 - s2;
            t1 = t1 - t2;
        }
        else
        {
            v = v - u;
            s2 = s2 - s1;
            t2 = t2 - t1;
        }
    }

    v = v * g;
    stop_time = get_nanos();
    // printf("\ns: %d\nt: %d\ngcd: %d", s2, t2, v);
    return stop_time - start_time;
}
/**
 * ************************************************************
 */
long bxgcdDiv3_replace_while_with_if(int x, int y)
{
    long start_time, stop_time;
    int g, u, s1, t1, s2, t2, v;

    start_time = get_nanos();
    g = 1;
    while (1)
    {
        if ((x % 2 == 0) && (y % 2 == 0))
        {
            x = x * (0.5);
            y = y * (0.5);
            g = 2 * g;
        }
        else if ((fast_mod3(x) == 0) && (fast_mod3(y) == 0))
        {
            x = x / 3;
            y = y / 3;
            g *= 3;
        }
        else
        {
            break;
        }
    }
    u = x;
    v = y;
    s1 = 1;
    t1 = 0;
    s2 = 0;
    t2 = 1;
    while (u != 0)
    {
        if ((u % 2) == 0)
        {
            u = u * (0.5);
            if ((s1 % 2 == 0) && (t1 % 2 == 0))
            {
                s1 = s1 * (0.5);
                t1 = t1 * (0.5);
            }
            else
            {
                s1 = (s1 + y) * (0.5);
                t1 = (t1 - x) * (0.5);
            }
        }
        else if ((v % 2) == 0)
        {
            v = v * (0.5);
            if ((s2 % 2 == 0) && (t2 % 2 == 0))
            {
                s2 = s2 * (0.5);
                t2 = t2 * (0.5);
            }
            else
            {
                s2 = (s2 + y) * (0.5);
                t2 = (t2 - x) * (0.5);
            }
        }

        if (u >= v)
        {
            u = u - v;
            s1 = s1 - s2;
            t1 = t1 - t2;
        }
        else
        {
            v = v - u;
            s2 = s2 - s1;
            t2 = t2 - t1;
        }
    }

    v = v * g;
    stop_time = get_nanos();
    // printf("\ns: %d\nt: %d\ngcd: %d", s2, t2, v);
    return stop_time - start_time;
}

/*
long bxgcd_div3_after_removing_power2(int x, int y)
{
    long start_time, stop_time;
    int g, u, s1, t1, s2, t2, v;

    start_time = get_nanos();
    g = 1;
    while ((x % 2 == 0) && (y % 2 == 0))
    {
        x = x * (0.5);
        y = y * (0.5);
        g = 2 * g;
    }
    while ((fast_mod3(x) == 0) && (fast_mod3(y) == 0))
    {
        x = x / 3;
        y = y / 3;
        g = g*3;
    }
    u = x;
    v = y;
    s1 = 1;
    t1 = 0;
    s2 = 0;
    t2 = 1;
    while (u != 0)
    {
        if ((u % 2) == 0)
        {
            u = u * (0.5);
            if ((s1 % 2 == 0) && (t1 % 2 == 0))
            {
                s1 = s1 * (0.5);
                t1 = t1 * (0.5);
            }
            else
            {
                s1 = (s1 + y) * (0.5);
                t1 = (t1 - x) * (0.5);
            }
        }
        else if (fast_mod3(u) == 0)
        {
            u = u / 3;
            if ((fast_mod3(s1) == 0) && (fast_mod3(t1) == 0))
            {
                s1 = s1 / 3;
                t1 = t1 / 3;
            }
            else
            {
                s1 = (s1 + y) / 3;
                t1 = (t1 - x) / 3;
            }
        }

        else if ((v % 2) == 0)
        {
            v = v * (0.5);
            if ((s2 % 2 == 0) && (t2 % 2 == 0))
            {
                s2 = s2 * (0.5);
                t2 = t2 * (0.5);
            }
            else
            {
                s2 = (s2 + y) * (0.5);
                t2 = (t2 - x) * (0.5);
            }
        }

        if (u >= v)
        {
            u = u - v;
            s1 = s1 - s2;
            t1 = t1 - t2;
        }
        else
        {
            v = v - u;
            s2 = s2 - s1;
            t2 = t2 - t1;
        }
    }
    v = v * g;
    stop_time = get_nanos();
    return stop_time - start_time;
}

long bxgcd_div3_in_removing_power2(int x, int y)
{
    long start_time, stop_time;
    int g, u, s1, t1, s2, t2, v;

    start_time = get_nanos();
    g = 1;
    while ((x % 2 == 0) && (y % 2 == 0))
    {
        x = x * (0.5);
        y = y * (0.5);
        g = 2 * g;

        while ((fast_mod3(x) == 0) && (fast_mod3(y) == 0))
        {
            x = x / 3;
            y = y / 3;
            g = g*3;
        }
    }
    u = x;
    v = y;
    s1 = 1;
    t1 = 0;
    s2 = 0;
    t2 = 1;
    while (u != 0)
    {
        if ((u % 2) == 0)
        {
            u = u * (0.5);
            if ((s1 % 2 == 0) && (t1 % 2 == 0))
            {
                s1 = s1 * (0.5);
                t1 = t1 * (0.5);
            }
            else
            {
                s1 = (s1 + y) * (0.5);
                t1 = (t1 - x) * (0.5);
            }
        }
        else if (fast_mod3(u) == 0)
        {
            u = u / 3;
            if ((fast_mod3(s1) == 0) && (fast_mod3(t1) == 0))
            {
                s1 = s1 / 3;
                t1 = t1 / 3;
            }
            else
            {
                s1 = (s1 + y) / 3;
                t1 = (t1 - x) / 3;
            }
        }

        else if ((v % 2) == 0)
        {
            v = v * (0.5);
            if ((s2 % 2 == 0) && (t2 % 2 == 0))
            {
                s2 = s2 * (0.5);
                t2 = t2 * (0.5);
            }
            else
            {
                s2 = (s2 + y) * (0.5);
                t2 = (t2 - x) * (0.5);
            }
        }

        if (u >= v)
        {
            u = u - v;
            s1 = s1 - s2;
            t1 = t1 - t2;
        }
        else
        {
            v = v - u;
            s2 = s2 - s1;
            t2 = t2 - t1;
        }
    }

    v = v * g;
    stop_time = get_nanos();
    // printf("\ns: %d\nt: %d\ngcd: %d", s2, t2, v);
    return stop_time - start_time;
}
long bxgcd_div3_in_and_after_removing_power2(int x, int y)
{
    long start_time, stop_time;
    int g, u, s1, t1, s2, t2, v;

    start_time = get_nanos();
    g = 1;
    while ((x % 2 == 0) && (y % 2 == 0))
    {
        x = x * (0.5);
        y = y * (0.5);
        g = 2 * g;

        while ((fast_mod3(x) == 0) && (fast_mod3(y) == 0))
        {
            x = x / 3;
            y = y / 3;
            g = g*3;
        }
    }

    while ((fast_mod3(x) == 0) && (fast_mod3(y) == 0))
    {
        x = x / 3;
        y = y / 3;
        g = g*3;
    }
    u = x;
    v = y;
    s1 = 1;
    t1 = 0;
    s2 = 0;
    t2 = 1;
    while (u != 0)
    {
        if ((u % 2) == 0)
        {
            u = u * (0.5);
            if ((s1 % 2 == 0) && (t1 % 2 == 0))
            {
                s1 = s1 * (0.5);
                t1 = t1 * (0.5);
            }
            else
            {
                s1 = (s1 + y) * (0.5);
                t1 = (t1 - x) * (0.5);
            }
        }
        else if (fast_mod3(u) == 0)
        {
            u = u / 3;
            if ((fast_mod3(s1) == 0) && (fast_mod3(t1) == 0))
            {
                s1 = s1 / 3;
                t1 = t1 / 3;
            }
            else
            {
                s1 = (s1 + y) / 3;
                t1 = (t1 - x) / 3;
            }
        }

        else if ((v % 2) == 0)
        {
            v = v * (0.5);
            if ((s2 % 2 == 0) && (t2 % 2 == 0))
            {
                s2 = s2 * (0.5);
                t2 = t2 * (0.5);
            }
            else
            {
                s2 = (s2 + y) * (0.5);
                t2 = (t2 - x) * (0.5);
            }
        }

        if (u >= v)
        {
            u = u - v;
            s1 = s1 - s2;
            t1 = t1 - t2;
        }
        else
        {
            v = v - u;
            s2 = s2 - s1;
            t2 = t2 - t1;
        }
    }

    v = v * g;
    stop_time = get_nanos();
    // printf("\ns: %d\nt: %d\ngcd: %d", s2, t2, v);
    return stop_time - start_time;
}

long bxgcdDiv3_replace_while_with_if(int x, int y)
{
    long start_time, stop_time;
    int g, u, s1, t1, s2, t2, v;

    start_time = get_nanos();
    g = 1;
    while (1)
    {
        if ((x % 2 == 0) && (y % 2 == 0))
        {
            x = x * (0.5);
            y = y * (0.5);
            g = 2 * g;
        }
        else if ((fast_mod3(x) == 0) && (fast_mod3(y) == 0))
        {
            x = x / 3;
            y = y / 3;
            g = g*3;
        }
        else
        {
            break;
        }
    }

    while ((fast_mod3(x) == 0) && (fast_mod3(y) == 0))
    {
        x = x / 3;
        y = y / 3;
        g = g*3;
    }
    u = x;
    v = y;
    s1 = 1;
    t1 = 0;
    s2 = 0;
    t2 = 1;
    while (u != 0)
    {
        if ((u % 2) == 0)
        {
            u = u * (0.5);
            if ((s1 % 2 == 0) && (t1 % 2 == 0))
            {
                s1 = s1 * (0.5);
                t1 = t1 * (0.5);
            }
            else
            {
                s1 = (s1 + y) * (0.5);
                t1 = (t1 - x) * (0.5);
            }
        }
        else if (fast_mod3(u) == 0)
        {
            u = u / 3;
            if ((fast_mod3(s1) == 0) && (fast_mod3(t1) == 0))
            {
                s1 = s1 / 3;
                t1 = t1 / 3;
            }
            else
            {
                s1 = (s1 + y) / 3;
                t1 = (t1 - x) / 3;
            }
        }

        else if ((v % 2) == 0)
        {
            v = v * (0.5);
            if ((s2 % 2 == 0) && (t2 % 2 == 0))
            {
                s2 = s2 * (0.5);
                t2 = t2 * (0.5);
            }
            else
            {
                s2 = (s2 + y) * (0.5);
                t2 = (t2 - x) * (0.5);
            }
        }

        if (u >= v)
        {
            u = u - v;
            s1 = s1 - s2;
            t1 = t1 - t2;
        }
        else
        {
            v = v - u;
            s2 = s2 - s1;
            t2 = t2 - t1;
        }
    }

    v = v * g;
    stop_time = get_nanos();
    // printf("\ns: %d\nt: %d\ngcd: %d", s2, t2, v);
    return stop_time - start_time;
}
*/