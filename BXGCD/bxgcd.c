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

long bxgcd(int x, int y)
{
    long start_time, stop_time;
    int g, u, s1, t1, s2, t2, v;

    // x = 693;
    // y = 609;
    // printf("\nNum1: %d\nrNum2: %d", rNum1, rNum2);
    start_time = get_nanos();
    g = 1;
    // For both to be even, isEven will return
    while ((x%2==0) && (y%2==0))
    {
        x = x / 2;
        y = y / 2;
        g = 2 * g;
    }

    // printf("[STEP 3]\n");
    u = x;
    v = y;
    s1 = 1;
    t1 = 0;
    s2 = 0;
    t2 = 1;
    while (1)
    {
        while ((u%2)==0)
        {
            u = u / 2;
            if ((s1 % 2 == 0) && (t1 % 2 == 0))
            {
                s1 = s1 / 2;
                t1 = t1 / 2;
            }
            else
            {
                s1 = (s1 + y) / 2;
                t1 = (t1 - x) / 2;
            }
        }
        while ((v%2) ==0)
        {
            v = v / 2;
            if ((s2 % 2 == 0) && (t2 % 2 == 0))
            {
                s2 = s2 / 2;
                t2 = t2 / 2;
            }
            else
            {
                s2 = (s2 + y) / 2;
                t2 = (t2 - x) / 2;
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

        if (u == 0)
        {
            v = v * g;
            break;
        }
    }
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
    printf("(Approximatly around the ~87000000ns-88000000ns range)\n");
    printf("Total Run Time: %lu nanosecond\n", bxgcd_run_time);
    printf("****************************************************************************************************************************\n");

    mpz_clear(rNumMpz1);
    mpz_clear(rNumMpz2);
    return 0;
}

/**
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

        int factorsOf2 = 0;
        start_time = get_nanos();
        printf("\nNum1: %d\nrNum2: %d", rNum1, rNum2);

        int g, u, s1, t1, s2, t2, v;
        v = 0;

        g = 1;
        int x = rNum1;
        int y = rNum2;
        // int x = 693;
        // int y = 609;

        // For both to be even, isEven will return
        while ((isEven(x) + isEven(y)) == 2)
        {
            x = x / 2;
            y = y / 2;
            g = 2 * g;
        }

        // printf("[STEP 3]\n");
        u = x;
        v = y;
        s1 = 1;
        t1 = 0;
        s2 = 0;
        t2 = 1;
        while (1)
        {
            while (isEven(u))
            {
                // printf("[STEP 4]\n");
                u = u / 2;
                if ((s1 % 2 == 0) && (t1 % 2 == 0))
                {
                    // printf("[STEP 4-True]\n");
                    s1 = s1 / 2;
                    t1 = t1 / 2;
                }
                else
                {
                    // printf("[STEP 4-False]\n");
                    s1 = (s1 + y) / 2;
                    t1 = (t1 - x) / 2;
                }
            }

            while (isEven(v))
            {
                // printf("[STEP 5]\n");
                v = v / 2;
                if ((s2 % 2 == 0) && (t2 % 2 == 0))
                {
                    // printf("[STEP 5-True]\n");
                    s2 = s2 / 2;
                    t2 = t2 / 2;
                }
                else
                {
                    // printf("[STEP 5-False]\n");
                    s2 = (s2 + y) / 2;
                    t2 = (t2 - x) / 2;
                }
            }

            if (u >= v)
            {
                // printf("[STEP 6-True]\n");
                u = u - v;
                s1 = s1 - s2;
                t1 = t1 - t2;
            }
            else
            {
                // printf("[STEP 6-False]\n");
                v = v - u;
                s2 = s2 - s1;
                t2 = t2 - t1;
            }

            if (u == 0)
            {
                v = v * g;
                break;
            }
        }
        stop_time = get_nanos();
        bxgcd_run_time += stop_time - start_time;
        printf("\ns: %d\nt: %d\ngcd: %d", s2, t2, v);
        //  r_x = r_y;
    }
    // printf("\nrGCD: %d\nNum1: %d\nrNum2: %d\ns: %d\nt: %d", r,rNum1, rNum2, s2, t2);

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
 *
 */
/**
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

     for (int i = 0; i < 10; i++)
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
         printf("\nNum1: %d\nrNum2: %d", rNum1, rNum2);

         int g, vV, u, aA, bB, cC, dD, v;
         int a = 0;
         int b = 0;
         vV = 0;

         g = 1;
         int x = rNum1;
         int y = rNum2;

         // For both to be even, isEven will return
         while ((isEven(x) + isEven(y)) == 2)
         {
             x = x / 2;
             y = y / 2;
             g = 2 * g;
         }

         // printf("[STEP 3]\n");
         u = x;
         vV = y;
         aA = 1;
         bB = 0;
         cC = 0;
         dD = 1;
         while (1)
         {
             while (isEven(u))
             {
                 printf("[STEP 4]\n");
                 u = u / 2;
                 if ((aA % 2 == 0) && (bB % 2 == 0))
                 {
                     printf("[STEP 4-True]\n");
                     aA = aA / 2;
                     bB = bB / 2;
                 }
                 else
                 {
                     printf("[STEP 4-False]\n");
                     aA = (aA + y) / 2;
                     bB = (bB - x) / 2;
                 }
             }

             while (isEven(vV))
             {
                 printf("[STEP 5]\n");
                 vV = vV / 2;
                 if ((cC % 2 == 0) && (dD % 2 == 0))
                 {
                     printf("[STEP 5-True]\n");
                     cC = cC / 2;
                     dD = dD / 2;
                 }
                 else
                 {
                     printf("[STEP 5-False]\n");
                     cC = (cC + y) / 2;
                     dD = (dD - x) / 2;
                 }
             }

             if (u >= vV)
             {
                 printf("[STEP 6-True]\n");
                 u = u - vV;
                 aA = aA - cC;
                 bB = bB - dD;
             }
             else
             {
                 printf("[STEP 6-False]\n");
                 vV = vV - u;
                 cC = cC - aA;
                 dD = dD - bB;
             }

             if (u == 0)
             {
                 a = cC;
                 b = dD;
                 v = vV * g;
             }
         }
         stop_time = get_nanos();
         bxgcd_run_time += stop_time - start_time;
         printf("\ns: %d\nt: %d\ngcd: %d", a, b, v);
         //  r_x = r_y;
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

     for (int i = 0; i < 10; i++)
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
             printf("\nNum1: %d\nrNum2: %d", rNum1, rNum2);
             while ((r_x % 2) == 0)
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

             while ((r_y % 2) == 0)
             {
                 r_y = r_y >> 1;
                 if (((t_x % 2) == 0) && ((t_y % 2) == 0))
                 {
                     t_x = t_x >> 1;
                     s_x = s_x >> 1;
                 }
                 else
                 {
                     t_x += rNum2;
                     t_x = t_x >> 1;
                     s_x -= rNum1;
                     s_x = s_x >> 1;
                 }
             }

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
             //printf("rx: %d\nry: %d",r_x,r_y);
         }

         int s = s_x;
         int t = t_x;
         int gcd = r_x * factorsOf2;
         stop_time = get_nanos();
         bxgcd_run_time += stop_time - start_time;
         printf("\ns: %d\nt: %d\ngcd: %d", s, t, gcd);
         //  r_x = r_y;
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
 */