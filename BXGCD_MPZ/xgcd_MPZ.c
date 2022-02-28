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

    //gmp_printf("\nrNum1: %Zd\nrNum2: %Zd\n", a, b);
    long start_time, stop_time;
    mpz_t s, t, last_s, last_t, remainder, quotient, temp, temp_mult;
    mpz_init(temp);
    mpz_init(remainder);
    mpz_init(temp_mult);
    mpz_init(quotient);

    start_time = get_nanos();
    mpz_init_set_ui(last_s, ui_one);
    mpz_init_set_ui(last_t, ui_zero);
    mpz_init_set_ui(s, ui_zero);
    mpz_init_set_ui(t, ui_one);

    while (mpz_sgn(b) > 0)
    {

        mpz_tdiv_q(quotient, a, b); // quotient = last_r / r;
        mpz_mod(remainder, a, b);   // quotient = last_r / r;

        mpz_set(temp, s);
        mpz_mul(temp_mult, quotient, s); // s = temp - quotient * s;
        mpz_sub(s, last_s, temp_mult);
        mpz_set(last_s, temp); // last_s = s;

        mpz_set(temp, t);
        mpz_mul(temp_mult, quotient, t);
        mpz_sub(t, last_t, temp_mult);
        mpz_set(last_t, temp);

        mpz_set(a, b); // temp = last_r;
        mpz_set(b, remainder);
    }
    stop_time = get_nanos();
    // gmp_printf("\nrGCD: %Zd\nNum1: %Zd\nrNum2: %Zd\nlast_s: %Zd\nlast_t: %Zd", last_r, a, b, last_s, last_t);
    //gmp_printf("\nIN XGCD\ns: %Zd\nt: %Zd\ngmp: %Zd\n\n", last_s, last_t, a);
    mpz_clears(s, t, last_s, last_t, quotient, temp, (mpz_ptr)NULL);
    return stop_time - start_time;
}

long bxgcd(mpz_t x, mpz_t y)
{

    //gmp_printf("\nrNum1: %Zd\nrNum2: %Zd\n", x, y);
    long start_time, stop_time;
    mpz_t u, s1, t1, s2, t2, v, rop;
    mpz_init(rop);

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

    mpz_init_set(u, x);           // u=x
    mpz_init_set(v, y);           // v=y
    mpz_init_set_ui(s1, ui_one);  // s1= 1;
    mpz_init_set_ui(t1, ui_zero); // t1 = 0;
    mpz_init_set_ui(s2, ui_zero); // s2 = 0;
    mpz_init_set_ui(t2, ui_one);  // t2= 1;

    while (mpz_sgn(u) != 0)
    {
        while (mpz_even_p(u) != 0) //((u % 2) == 0)
        {
            mpz_tdiv_q_2exp(rop, u, 1);
            mpz_set(u, rop); // u = u / 2;

            if ((mpz_even_p(s1) != 0) && (mpz_even_p(t1) != 0)) // ((s1 % 2 == 0) && (t1 % 2 == 0))
            {
                mpz_tdiv_q_2exp(rop, s1, 1);
                mpz_set(s1, rop); // s1 = s1 / 2;

                mpz_tdiv_q_2exp(rop, t1, 1);
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
        while (mpz_even_p(v) != 0) //((v % 2) == 0)
        {
            mpz_tdiv_q_2exp(rop, v, 1);
            mpz_set(v, rop);                                    // v = v / 2;
            if ((mpz_even_p(s2) != 0) && (mpz_even_p(t2) != 0)) //((s2 % 2 == 0) && (t2 % 2 == 0))
            {
                mpz_tdiv_q_2exp(rop, s2, 1);
                mpz_set(s2, rop); // s2 = s2 / 2;
                mpz_tdiv_q_2exp(rop, t2, 1);
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

        // gmp_printf("\nVALUES: %Zd %Zd %Zd %Zd %Zd %Zd\n", u, v, s1, t1,s2,t2);
    }
    mpz_mul_2exp(rop, v, g);
    mpz_set(v, rop); //     v = v * g;
    stop_time = get_nanos();
    //gmp_printf("\ns: %Zd\nt: %Zd\ngcd: %Zd", s2, t2, v);
    mpz_clears(u, s1, t1, s2, t2, v, rop, (mpz_ptr)NULL);
    return stop_time - start_time;
}

long new_xgcd(mpz_t a, mpz_t b)
{

    long start_time, stop_time;
    mpz_t s, r, t, last_s, last_t, last_r, quotient, temp;
    mpz_init(temp);
    mpz_init(quotient);
    unsigned long int g = 1;
    start_time = get_nanos();
    // For both to be even, isEven will return
    while ((mpz_even_p(a) != 0) && (mpz_even_p(b) != 0))
    {
        mpz_divexact_ui(temp, a, ui_two);
        mpz_set(a, temp); // x = x / 2;
        mpz_divexact_ui(temp, b, ui_two);
        mpz_set(b, temp); // y = y / 2;
        g++;
    }
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
    mpz_mul_2exp(temp, last_r, g);
    mpz_set(last_r, temp); //     v = v * g;
    //gmp_printf("\n\nIN NEWXGCD\ns: %Zd\nt: %Zd\ngmp: %Zd\n\n", last_s, last_t, last_r);
    stop_time = get_nanos();
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
        long new_xgcdf_run_time = 0UL;
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
        mpz_t rNum1_temp, rNum2_temp;
        for (int i = 0; i < sampleSize; i++)
        {
            mpz_rrandomb(rNum1, state, nBits);
            mpz_rrandomb(rNum2, state, nBits);
            
            if (mpz_cmp(rNum1, rNum2) < 0)
            {
                mpz_swap(rNum1, rNum2);
            }
            
            mpz_init_set(rNum1_temp, rNum1);
            mpz_init_set(rNum2_temp, rNum2);
            //gmp_printf("\nrNum1: %Zd\nrNum2: %Zd\n", rNum1, rNum2);
            xgcd_run_time += xgcd(rNum1_temp, rNum2_temp);
            mpz_init_set(rNum1_temp, rNum1);
            mpz_init_set(rNum2_temp, rNum2);
            bxgcd_run_time += bxgcd(rNum1_temp, rNum2_temp);
            //new_xgcdf_run_time += new_xgcd(rNum1, rNum2);
        }

        printf("\n****************************************************************************************************************************\n");
        printf("XGCD RUNTIME FOR COMPUTING THE XGCD OF TWO %s-BIT NUMBERS\n[SAMPLE SIZE: 10000 iterations per divisor]", argv[1]);
        printf("\n****************************************************************************************************************************\n");
        // printf("(Approximatly around the ~87000000ns-88000000ns range)\n");
        printf("XGCD(Total Run Time):  %lu nanosecond\n", xgcd_run_time);
        printf("BXGCD(Total Run Time): %lu nanosecond\n", bxgcd_run_time);
        printf("NEWXGCD(Total Run Time): %lu nanosecond\n", new_xgcdf_run_time);
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