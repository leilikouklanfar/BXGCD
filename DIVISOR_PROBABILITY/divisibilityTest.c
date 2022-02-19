#include <stdio.h>
#include <gmp.h>
// gcc divisibilityTest.c -o  divisibilityTest -lgmp

int checkExactDivisors(unsigned long divisor, mp_bitcnt_t bitLength)
{
    int sampleSize = 100000;
    int divCtr = 0;

    mp_bitcnt_t nBits = bitLength;
    unsigned long int seed = 0UL;
    gmp_randstate_t state;

    // Initialize state with default algorithm
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);

    // Initialize mp number
    mpz_t rNum;
    mpz_init(rNum);

    for (int i = 0; i < sampleSize; i++)
    {
        mpz_rrandomb(rNum, state, nBits);
        // printf("> rNum:");
        // gmp_printf("%Zd\n", rNum);
        if (mpz_divisible_ui_p(rNum, divisor) != 0)
        {
            divCtr++;
        }
    }
    mpz_clear(rNum);
    return divCtr;
}

int main()
{
    printf("\n****************************************************************************************************************************\n");
    printf("PROBRABLITY OF AN NBIT RANDOM NUMBER BEING DIVISABLE BY THE GIVEN DIVISOR [SAMPLE SIZE: 100 000]");
    printf("\n****************************************************************************************************************************\n");
    printf("[NUMBER OF BITS]\t\t[32bit]\t\t[64bit]\t\t[128bit]\t[256bit]\t[512bit]\t[1024bit]\n");
    printf("DIVISABLE BY   2\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", checkExactDivisors(2, 32), checkExactDivisors(2, 64), checkExactDivisors(2, 128), checkExactDivisors(2, 256), checkExactDivisors(2, 512), checkExactDivisors(2, 1024));
    printf("DIVISABLE BY   3\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", checkExactDivisors(3, 32), checkExactDivisors(3, 64), checkExactDivisors(3, 128), checkExactDivisors(3, 256), checkExactDivisors(3, 512), checkExactDivisors(3, 1024));
    printf("DIVISABLE BY   5\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", checkExactDivisors(5, 32), checkExactDivisors(5, 64), checkExactDivisors(5, 128), checkExactDivisors(5, 256), checkExactDivisors(5, 512), checkExactDivisors(5, 1024));
    printf("DIVISABLE BY   7\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", checkExactDivisors(7, 32), checkExactDivisors(7, 64), checkExactDivisors(7, 128), checkExactDivisors(7, 256), checkExactDivisors(7, 512), checkExactDivisors(7, 1024));
    printf("DIVISABLE BY  11\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", checkExactDivisors(11, 32), checkExactDivisors(11, 64), checkExactDivisors(11, 128), checkExactDivisors(11, 256), checkExactDivisors(11, 512), checkExactDivisors(11, 1024));
    printf("DIVISABLE BY  17\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", checkExactDivisors(17, 32), checkExactDivisors(17, 64), checkExactDivisors(17, 128), checkExactDivisors(17, 256), checkExactDivisors(17, 512), checkExactDivisors(17, 1024));
    printf("****************************************************************************************************************************\n\n");
}

// the code below is the divisibiliy test but for only one random number
/**#include <stdio.h>
#include <gmp.h>
#include <time.h>

int main()
{
    unsigned long int seed = 0UL;
    gmp_randstate_t state;
    // Initialize state with default algorithm
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);

    int div2Ctr = 0;
    int div3Ctr = 0;
    int div5Ctr = 0;
    int div7Ctr = 0;
    int div11Ctr = 0;
    int div17Ctr = 0;

    checkExactDivisors(100000, 32, );
    printf("\n\n[SAMPLE SIZE: %d]\t[32bit]\t\t[64bit]\n", i);
    printf("DIVISABLE BY  2\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", div2Ctr32bit, div2Ctr64bit, div2Ctr128bit, div2Ctr256bit, div2Ctr512bit, div2Ctr1024bit);
    printf("DIVISABLE BY  3\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", div3Ctr32bit, div3Ctr64bit, div3Ctr128bit, div3Ctr256bit, div3Ctr512bit, div3Ctr1024bit);
    printf("DIVISABLE BY  5\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", div5Ctr32bit, div5Ctr64bit, div5Ctr128bit, div5Ctr256bit, div5Ctr512bit, div5Ctr1024bit);
    printf("DIVISABLE BY  7\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", div7Ctr32bit, div7Ctr64bit, div7Ctr128bit, div7Ctr256bit, div7Ctr512bit, div7Ctr1024bit);
    printf("DIVISABLE BY 11\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", div11Ctr32bit, div11Ctr64bit, div11Ctr128bit, div11Ctr256bit, div11Ctr512bit, div11Ctr1024bit);
    printf("DIVISABLE BY 17\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n", div17Ctr32bit, div17Ctr64bit, div17Ctr128bit, div17Ctr256bit, div17Ctr512bit, div17Ctr1024bit);

    mpz_clear(rNum);
}

void checkExactDivisors(int sampleSize, mp_bitcnt_t bitLength, int *div2Ctr, int *div3Ctr, int *div5Ctr, int *div7Ctr, int *div11Ctr, int *div17Ctr)
{
    div2Ctr = 0;
    div3Ctr = 0;
    div5Ctr = 0;
    div7Ctr = 0;
    div11Ctr = 0;
    div17Ctr = 0;

    mp_bitcnt_t nBits = bitLength;

    unsigned long int seed = 0UL;
    gmp_randstate_t state;

    // Initialize state with default algorithm
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);
    // Initialize mp number
    mpz_t rNum;
    mpz_init(rNum);

    for (int i = 0; i < sampleSize; i++)
    {
        mpz_rrandomb(rNum, state, nBits);
        printf("> rNum:");
        gmp_printf("%Zd\n", rNum);
        // 2
        if (mpz_divisible_ui_p(rNum, 2UL) != 0)
        {
            div2Ctr++;
        }
        // 3
        if (mpz_divisible_ui_p(rNum, 3UL) != 0)
        {
            div3Ctr++;
        }
        // 5
        if (mpz_divisible_ui_p(rNum, 5UL) != 0)
        {
            div5Ctr++;
        }
        // 7
        if (mpz_divisible_ui_p(rNum, 7UL) != 0)
        {
            div7Ctr++;
        }
        // 11
        if (mpz_divisible_ui_p(rNum, 11UL) != 0)
        {
            div11Ctr++;
        }

        // 17
        if (mpz_divisible_ui_p(rNum, 17UL) != 0)
        {
            div17Ctr++;
        }
    }
}*/