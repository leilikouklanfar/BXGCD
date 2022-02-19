
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gmp.h>
#include <time.h>
#include <math.h>

#include <errno.h>

long get_nanos()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

int isEven(int n)
{
    if (n & 1)
    {
        // odd
        return 0;
    }
    return 1;
}
// Idea for code for the BXGCD is from the HOAC
int main()
{
    long bxgcd_run_time = 0UL;
    long start_time;
    long stop_time;
    int sampleSize = 100000;
    int divCtr = 0;

    mp_bitcnt_t nBits = 16;
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

    for (int i = 0; i < 10000; i++)
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

        int factorsOf2 = 0;
        start_time = get_nanos();
        while (((rNum1 % 2) == 0) && ((rNum2 % 2) == 0))
        {
            // printf("[STEP 2-In Loop]\n");
            rNum1 = rNum1 >> 1;
            rNum2 = rNum2 >> 1;
            factorsOf2 >> 1;
        }
        int r_x = rNum1;
        int r_y = rNum2;
        int s_x = 1;
        int s_y = 0;
        int t_x = 0;
        int t_y = 1;

        while (r_x != r_y)
        {
            //printf("\nNum1: %d\nrNum2: %d", rNum1, rNum2);
            if ((r_x % 2) != 0)
            {
                if (r_x > r_y)
                {
                    r_x -= r_y;
                    s_x -= s_y;
                    t_x -= t_x;
                }
                else
                {
                    r_y -= r_x;
                    s_y -= s_x;
                    t_y -= t_x;
                }
            }

            else // ((r_x % 2) == 0)
            {
                r_x = r_x >> 1;
                if (((s_x % 2) == 0) && ((s_y % 2) == 0))
                {
                    s_x = s_x >> 1;
                    t_x = t_x >> 1;
                }
                else
                {
                    s_x += rNum2;
                    s_x = s_x >> 1;
                    t_x -= rNum1;
                    t_x = t_x >> 1;
                }
            }
            //printf("rx: %d\nry: %d",r_x,r_y);
        }

        int s = s_x;
        int t = t_x;
        int gcd = r_x;
        gcd = gcd * factorsOf2;
        stop_time = get_nanos();
        bxgcd_run_time += stop_time - start_time;
        //printf("\ns: %d\nt: %d\ngcd: %d", s, t, gcd);
        // r_x = r_y;
    }
    // printf("\nrGCD: %d\nNum1: %d\nrNum2: %d\ns: %d\nt: %d", r,rNum1, rNum2, s, t);

    printf("\n****************************************************************************************************************************\n");
    printf("BXGCD RUNTIME FOR COMPUTING THE XGCD OF TWO 31-BIT NUMBERS\n[SAMPLE SIZE: 10000 iterations per divisor]");
    printf("\n****************************************************************************************************************************\n");
    printf("(Approximatly around the ~87000000ns-88000000ns range)\n");
    printf("Total Run Time: %lu nanosecond\n", bxgcd_run_time);
    printf("****************************************************************************************************************************\n");

    mpz_clear(rNumMpz1);
    mpz_clear(rNumMpz2);
    return 0;
}
