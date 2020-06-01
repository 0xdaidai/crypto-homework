/*************************************************************************
 > File Name: init.cpp
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Sun 10 May 2020 09:13:55 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmpxx.h>

void rsa_init(mpz_t n, mpz_t e, mpz_t d );

int main(){
    mpz_t n,e,d;
    mpz_inits(n,e,d,NULL);
    rsa_init(n, e, d);
    gmp_printf("n = %Zd ,e = %Zd ,z = %Zd\n",n,e,d);
    mpz_clears(n,e,d,NULL);

    return 0;
}

void rsa_init(mpz_t n, mpz_t e, mpz_t d )
{
    unsigned long int cur;
    cur = ((time(0) % 1000) + 10000) * ((clock() % 1000) + 10000);
    //initialize state
    gmp_randstate_t state;
    gmp_randinit_default(state);
    gmp_randseed_ui(state, cur);

    mpz_t min, base, num_one, p, q, phi_n, p_1, q_1;
    mpz_init_set_ui(num_one, 1);
    mpz_inits(phi_n, min, base, p, q, p_1, q_1, NULL);
    mpz_mul_2exp(min, num_one, 511);

    mpz_set_ui(base, 1);
    while (mpz_cmp(base, min) <= 0)
        mpz_urandomb(base, state, 512);

    mpz_nextprime(p, base);
    mpz_sub_ui(p_1, p, 1);

    mpz_set_ui(base, 1);
    while (mpz_cmp(base, min) <= 0)
        mpz_urandomb(base, state, 512);
    mpz_nextprime(q, base);
    mpz_sub_ui(q_1, q, 1);

    mpz_mul(n, p, q);
    mpz_mul(phi_n, p_1, q_1);
    //create e
    mpz_set_ui(e,(1<<16)+1);
    //calculate d
    mpz_invert(d, e, phi_n);
    mpz_clears(min, base, num_one, p, q, phi_n, p_1, q_1, NULL);
}

