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

int main()
{
    long xgcd_run_time = 0UL, start_time, stop_time;
    int sampleSize = 100000, divCtr = 0, s, t, rNum1, rNum2;
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

        start_time = get_nanos();
        s = 1;
        t = 0;
        int newX = 0;
        int newY = 1;
        int new_r = rNum2;
        int r = rNum1;
        int quotient, oldR;
        while (new_r)
        {
            quotient = r / new_r;

            oldR = r;
            r = new_r;
            new_r = oldR - quotient * new_r;

            oldR = s;
            s = newX;
            newX = oldR - quotient * newX;

            oldR = t;
            t = newY;
            newY = oldR - quotient * newY;
        }
        stop_time = get_nanos();
        xgcd_run_time += stop_time - start_time;
        //printf("\nrGCD: %d\nNum1: %d\nrNum2: %d\ns: %d\nt: %d", r,rNum1, rNum2, s, t);
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
 *  gcd(a,b)=as+bt s.t. a>b

static inline int xgcd(int a, int b, int s, int t)
{
    s = 1;
    t = 0;
    int newX = 0;
    int newY = 1;
    int new_r = b;
    int r = a;
    int quotient, oldR;
    while (new_r)
    {
        quotient = r / new_r;

        oldR = r;
        r = new_r;
        new_r = oldR - quotient * new_r;

        oldR = s;
        s = newX;
        newX = oldR - quotient * newX;

        oldR = t;
        t = newY;
        newY = oldR - quotient * newY;
    }


    return r;
}
 */