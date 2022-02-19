#include <stdio.h>
#include <gmp.h>
#include <time.h>

int main(){
    unsigned long int seed = 0UL;
    gmp_randstate_t state;
    // Initialize state with default algorithm
    gmp_randinit_default(state);
    gmp_randseed_ui(state, seed);

    mpz_t rNum;
    mpz_init(rNum);
    mp_bitcnt_t nBits = 16;
    mpz_rrandomb(rNum, state, nBits);
    gmp_printf("\nRNG(16bit):  %Zd\n",rNum);
    mpz_clear(rNum);
}
