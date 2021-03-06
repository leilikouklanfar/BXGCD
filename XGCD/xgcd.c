#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
// A faster version of the algorithm defined in the paper(same idea, less instansations). The algorithm defined in the paper is xgcd_paper.c.
static inline long get_nanos()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

long xgcd(int a, int b)
{
    long start_time, stop_time;
    int last_s, last_t;
    start_time = get_nanos();
    last_s = 1;
    last_t = 0;
    int s = 0;
    int t = 1;
    int r = b;      // b
    int last_r = a; // a
    int quotient, temp;
    while (r)
    {
        quotient = last_r / r;

        temp = last_r;
        last_r = r;
        r = temp - quotient * r;

        temp = last_s;
        last_s = s;
        s = temp - quotient * s;

        temp = last_t;
        last_t = t;
        t = temp - quotient * t;
    }
    stop_time = get_nanos();
    // printf("\nrGCD: %d\nNum1: %d\nrNum2: %d\nlast_s: %d\nlast_t: %d", last_r, a, b, last_s, last_t);
    return stop_time - start_time;
}

int main()
{
    long xgcd_run_time = 0UL;
    int sampleSize = 100000, divCtr = 0, rNum1, rNum2;
    char *temp;
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
        mpz_rrandomb(rNumMpz1, state, nBits);
        // GET THE FIRST RANDOM NUMBER
        temp = mpz_get_str(NULL, 10, rNumMpz1);
        rNum1 = atoi(temp);
        mpz_rrandomb(rNumMpz2, state, nBits);
        free(temp);
        // GET THE SECOND RANDOM NUMBER
        temp = mpz_get_str(NULL, 10, rNumMpz2);
        rNum2 = atoi(temp);
        free(temp);
        if (rNum1 < rNum2)
        {
            int smallerNum = rNum1;
            rNum1 = rNum2;
            rNum2 = smallerNum;
        }

        xgcd_run_time += xgcd(rNum1, rNum2);
    }

    printf("\n****************************************************************************************************************************\n");
    printf("XGCD RUNTIME FOR COMPUTING THE XGCD OF TWO 31-BIT NUMBERS\n[SAMPLE SIZE: 10000 iterations per divisor]");
    printf("\n****************************************************************************************************************************\n");
    printf("(Approximatly around the ~87000000ns-88000000ns range)\n");
    printf("Total Run Time: %lu nanosecond\n", xgcd_run_time);
    printf("****************************************************************************************************************************\n");

    mpz_clear(rNumMpz1);
    mpz_clear(rNumMpz2);
    return 0;
}