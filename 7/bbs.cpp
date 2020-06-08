/*************************************************************************
 > File Name: bbs.cpp
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Tue 28 Apr 2020 05:00:20 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <gmpxx.h>
#include <time.h>
#include "ari.h"

void randnum(mpz_t key){
    clock_t time = clock();
    gmp_randstate_t grt;
    gmp_randinit_default(grt);
    gmp_randseed_ui(grt, time);

    mpz_rrandomb(key, grt, 16);
}

void bbs(mpz_t res,long long times){
    mpz_t p,q,s,g_tmp,x;
    mpz_t n; //q*p
    
    //init
    mpz_init(p);
    mpz_init(q);
    mpz_init(n);
    mpz_init(s);
    mpz_init(g_tmp);
    mpz_init(x);

    while(1){//make p
        randnum(p);
        mpz_mod_ui(g_tmp,p,4);
        if (mpz_cmp_ui(g_tmp,3)!=0)
            continue;
        
        if (isprime(p))
            break;
    }
    // printf("p is ok! \n");

    while(1){//make q
        randnum(q);
        mpz_mod_ui(g_tmp,q,4);
        if (mpz_cmp_ui(g_tmp,3)!=0)
            continue;
        
        if (isprime(q))
            break;
    }
    // printf("q is ok! \n");

    mpz_mul(n,p,q);//get n
    // printf("n initial\n");
    while(1){//get s
        randnum(s);
        mpz_mod(g_tmp,s,p);
        if (mpz_cmp_ui(g_tmp,0)==0)
            continue;
        mpz_mod(g_tmp,s,q);
        if (mpz_cmp_ui(g_tmp,0)==0)
            continue;
        break;
    }
    // printf("n is ok\n");
    mpz_mul(x,s,s);
    mpz_mod(x,x,n);
    
    while(times--){
        mpz_mul(x,x,x);
        mpz_mod(x,x,n);
        mpz_mod_ui(g_tmp,x,2);
        mpz_mul_ui(res,res,2);
        mpz_add(res,res,g_tmp);
    }

    mpz_clears(p,q,s,x,n,g_tmp,NULL);
}

int main(){
    mpz_t xx;
    mpz_init_set_str(xx,"0",10);
    bbs(xx,50);
    gmp_printf("%Zd\n",xx);
    bbs(xx,50);
    gmp_printf("%Zd\n",xx);
    bbs(xx,50);
    gmp_printf("%Zd\n",xx);
    mpz_clear(xx);
    return 0;
}
