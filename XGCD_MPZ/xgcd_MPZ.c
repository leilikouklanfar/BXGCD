#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

const unsigned long int ui_one = 1;
const unsigned long int ui_zero = 0;
const unsigned long int ui_two = 2;
// A faster version of the algorithm defined in the paper(same idea, less instansations). The algorithm defined in the paper is xgcd_paper.c.
static inline long get_nanos()
{
    struct timespec ts;
    timespec_get(&ts, TIME_UTC);
    return (long)ts.tv_sec * 1000000000L + ts.tv_nsec;
}

long xgcd(mpz_t a, mpz_t b)
{
    long start_time, stop_time;
    mpz_t s, r, t, last_s, last_t, last_r, quotient, temp;
    mpz_init(temp);
    mpz_init(quotient);

    start_time = get_nanos();
    mpz_init_set_ui(last_s, ui_one);
    mpz_init_set_ui(last_t, ui_zero);
    mpz_init_set_ui(s, ui_zero);
    mpz_init_set_ui(t, ui_one);
    mpz_init_set(r, b);      // b
    mpz_init_set(last_r, a); // a
    while (mpz_sgn(r) != 0)
    {

        mpz_tdiv_q(quotient, last_r, r); // quotient = last_r / r;
        mpz_set(temp, last_r);           // temp = last_r;
        mpz_set(last_r, r);              // last_r = r;

        mpz_set(r, temp);                // mpz_submul (mpz_t rop, const mpz_t op1, const mpz_t op2) = rop - op1 times op2.
        mpz_submul(r, quotient, last_r); // r = temp - quotient * r;

        mpz_set(temp, last_s); // temp = last_s;
        mpz_set(last_s, s);    // last_s = s;
        mpz_set(s, temp);
        mpz_submul(s, quotient, last_s); // s = temp - quotient * s;

        mpz_set(temp, last_t); // temp = last_t;
        mpz_set(last_t, t);    // last_t = t;
        mpz_set(t, temp);
        mpz_submul(t, quotient, last_t); // t = temp - quotient * t;
    }
    stop_time = get_nanos();
    // gmp_printf("\nrGCD: %Zd\nNum1: %Zd\nrNum2: %Zd\nlast_s: %Zd\nlast_t: %Zd", last_r, a, b, last_s, last_t);
    // gmp_printf("\nIN XGCD\ns: %Zd\nt: %Zd\ngmp: %Zd\n\n", last_s, last_t, last_r);
    return stop_time - start_time;
}

long bxgcd(mpz_t x, mpz_t y)
{
    long start_time, stop_time;
    mpz_t u, s1, t1, s2, t2, v, rop;
    mpz_init(rop);

    // x = 693;
    // y = 609;
    // printf("\nNum1: %d\nrNum2: %d", rNum1, rNum2);
    start_time = get_nanos();
    unsigned long int g = 1;
    // For both to be even, isEven will return
    while ((mpz_even_p(x) != 0) && (mpz_even_p(y) != 0))
    {
        mpz_divexact_ui(rop, x, ui_two);
        mpz_set(x, rop); // x = x / 2;
        mpz_divexact_ui(rop, y, ui_two);
        mpz_set(y, rop); // y = y / 2;
        g++;
    }

    // printf("[STEP 3]\n");
    mpz_init_set(u, x);           // u=x
    mpz_init_set(v, y);           // v=y
    mpz_init_set_ui(s1, ui_one);  // s1= 1;
    mpz_init_set_ui(t1, ui_zero); // t1 = 0;
    mpz_init_set_ui(s2, ui_zero); // s2 = 0;
    mpz_init_set_ui(t2, ui_one);  // t2= 1;

    while (mpz_sgn(u) != 0)
    {
        if (mpz_even_p(u) != 0) //((u % 2) == 0)
        {
            mpz_divexact_ui(rop, u, ui_two);
            mpz_set(u, rop); // u = u / 2;

            if ((mpz_even_p(s1) != 0) && (mpz_even_p(t1) != 0)) // ((s1 % 2 == 0) && (t1 % 2 == 0))
            {
                mpz_divexact_ui(rop, s1, ui_two);
                mpz_set(s1, rop); // s1 = s1 / 2;
                mpz_divexact_ui(rop, t1, ui_two);
                mpz_set(t1, rop); // t1 = t1 / 2;
            }
            else
            {
                mpz_add(rop, s1, y); // s1 = (s1 + y) / 2;
                mpz_tdiv_q_2exp(s1, rop, 1);

                mpz_sub(rop, t1, x); // t1 = (t1 - x) / 2;
                mpz_tdiv_q_2exp(t1, rop, 1);
            }
        }
        else if (mpz_even_p(v) != 0) //((v % 2) == 0)
        {
            mpz_divexact_ui(rop, v, ui_two);
            mpz_set(v, rop);                                    // v = v / 2;
            if ((mpz_even_p(s2) != 0) && (mpz_even_p(t2) != 0)) //((s2 % 2 == 0) && (t2 % 2 == 0))
            {
                mpz_divexact_ui(rop, s2, ui_two);
                mpz_set(s2, rop); // s2 = s2 / 2;
                mpz_divexact_ui(rop, t2, ui_two);
                mpz_set(t2, rop); // t2 = t2 / 2;
            }
            else
            {
                mpz_add(rop, s2, y); // s2 = (s2 + y) / 2;
                mpz_tdiv_q_2exp(s2, rop, 1);
                mpz_sub(rop, t2, x); // t2 = (t2 - x) / 2;
                mpz_tdiv_q_2exp(t2, rop, 1);
            }
        }

        if (mpz_cmp(u, v) < 0) //(u < v)
        {
            mpz_sub(rop, v, u); // v = v - u;
            mpz_set(v, rop);

            mpz_sub(rop, s2, s1); // s2 = s2 - s1;
            mpz_set(s2, rop);

            mpz_sub(rop, t2, t1); // t2 = t2 - t1;
            mpz_set(t2, rop);
        }
        else
        {

            mpz_sub(rop, u, v); // u = u - v;
            mpz_set(u, rop);

            mpz_sub(rop, s1, s2); // s1 = s1 - s2;
            mpz_set(s1, rop);

            mpz_sub(rop, t1, t2); // t1 = t1 - t2;
            mpz_set(t1, rop);
        }
    }
    mpz_mul_2exp(rop, v, g);
    mpz_set(v, rop); //     v = v * g;
    stop_time = get_nanos();
    // gmp_printf("INBXGCD\ns: %Zd\nt: %Zd\ngcd: %Zd", s2, t2, v);
    return stop_time - start_time;
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("One argument expected.\n");
    }
    else
    {
        printf("Bit Size is %s\n", argv[1]);
        mp_bitcnt_t nBits = atoi(argv[1]);

        long xgcd_run_time = 0UL;
        long bxgcd_run_time = 0UL;
        int sampleSize = 100000;
        unsigned long int seed = 0UL;
        gmp_randstate_t state;
        // Initialize state with default algorithm
        gmp_randinit_default(state);
        gmp_randseed_ui(state, seed);

        // Initialize mp numbers
        mpz_t rNum1;
        mpz_init(rNum1);
        mpz_t rNum2;
        mpz_init(rNum2);

        for (int i = 0; i < sampleSize; i++)
        {
            mpz_rrandomb(rNum1, state, nBits);
            mpz_rrandomb(rNum2, state, nBits);

            if (mpz_cmp(rNum1, rNum2) < 0)
            {
                mpz_swap(rNum1, rNum2);
            }
            // gmp_printf("\nrNum1: %Zd\nNum2: %Zd\n", rNum1, rNum2);
            xgcd_run_time += xgcd(rNum1, rNum2);
            bxgcd_run_time += bxgcd(rNum1, rNum2);
        }

        printf("\n****************************************************************************************************************************\n");
        printf("XGCD RUNTIME FOR COMPUTING THE XGCD OF TWO %s-BIT NUMBERS\n[SAMPLE SIZE: 10000 iterations per divisor]", argv[1]);
        printf("\n****************************************************************************************************************************\n");
        // printf("(Approximatly around the ~87000000ns-88000000ns range)\n");
        printf("XGCD(Total Run Time):  %lu nanosecond\n", xgcd_run_time);
        printf("BXGCD(Total Run Time): %lu nanosecond\n", bxgcd_run_time);
        printf("****************************************************************************************************************************\n");

        mpz_clear(rNum1);
        mpz_clear(rNum2);
    }
    return 0;
}

/**
mpz_t divisor;
mpz_init_set_ui(divisor, d);
mpz_t rNum1;
mpz_init_set_si(rNum1, r1);
mpz_t rNum2;
mpz_init_set_si(rNum2, r2);
mpz_t temp;
mpz_init(temp);
// DIVIDE MPZ by MPZ
printf("\nDIVIDE MPZ by MPZ(mpz_tdiv_q)\n\nBEFORE:");
gmp_printf("\nrNum1: %Zd", rNum1);
gmp_printf("\nrNum2: %Zd\n", rNum2);
mpz_tdiv_q(temp, rNum1, rNum2);
gmp_printf("\nAFTER:\nrNum1: %Zd", rNum1);
gmp_printf("\nrNum2: %Zd", rNum2);
gmp_printf("\ntemp: %Zd\n", temp);

// DIVIDE MPZ by unsinged divisor
printf("\nDIVIDE MPZ by singed divisor\n\nBEFORE:");
gmp_printf("\nrNum1: %Zd", rNum1);
gmp_printf("\ndividor: %Zd\n", divisor);
mpz_tdiv_q(temp, rNum1, divisor);
gmp_printf("\nAFTER:\nrNum1: %Zd", rNum1);
gmp_printf("\ndividor: %Zd", divisor);
gmp_printf("\ntemp: %Zd\n", temp);
 *
 */