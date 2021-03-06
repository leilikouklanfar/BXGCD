#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include <time.h>
#include <math.h>

static inline long get_nanos()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

long bxgcd(int x, int y)
{
    long start_time, stop_time;
    int g, u, s1, t1, s2, t2, v;

    // x = 693;
    // y = 609;
    // printf("\na: %d\nb: %d", a, b);
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

// Idea for code for the BXGCD is from the HOAC
int main()
{
    long bxgcd_run_time = 0UL;
    int sampleSize = 100000;
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
        //  r_x = r_y;
    }
    // printf("\nrGCD: %d\nNum1: %d\nrNum2: %d\ns: %d\nt: %d", r,rNum1, rNum2, s2, t2);

    printf("\n****************************************************************************************************************************\n");
    printf("BXGCD RUNTIME FOR COMPUTING THE BXGCD OF TWO 31-BIT NUMBERS\n[SAMPLE SIZE: 10000 iterations per divisor]");
    printf("\n****************************************************************************************************************************\n");
    printf("(Approximatly around the ~78700000-80000000ns range)\n");
    printf("Total Run Time: %lu nanosecond\n", bxgcd_run_time);
    printf("****************************************************************************************************************************\n");

    mpz_clear(rNumMpz1);
    mpz_clear(rNumMpz2);
    return 0;
}