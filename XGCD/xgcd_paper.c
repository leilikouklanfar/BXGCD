#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

static inline long get_nanos()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

long xgcd(int a, int b)
{
    //printf("\nNum1: %d\nrNum2: %d", a, b);
    long start_time, stop_time;
    int r, s, t, s1, s2, t2, t1, quotient;
    start_time = get_nanos();
    s2 = 1;
    s1 = 0;
    t2 = 0;
    t1 = 1;
    while (b != 0)
    {
        quotient = a / b;     // a div b = floor(a/b)
        r = a - quotient * b; // a mod b = a - b * ( floor(a/b) )
        s = s2 - quotient * s1;
        t = t2 - quotient * t1;

        a = b;
        b = r;
        s2 = s1;
        s1 = s;
        t2 = t1;
        t1 = t;
    }
    //printf("\nrGCD: %d\ns: %d\nt: %d", a, s2, t2);
    stop_time = get_nanos();
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

/**
 * @brief  XGCD iterative
 *  gcd(a,b)=as+bt last_s.last_t. a>b

static inline int xgcd(int a, int b, int last_s, int last_t)
{
    last_s = 1;
    last_t = 0;
    int s = 0;
    int t = 1;
    int r = b;
    int last_r = a;
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


    return last_r;
}
 */