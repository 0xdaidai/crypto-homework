/*************************************************************************
 > File Name: 10/rabin-plus.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Sat 23 May 2020 12:31:08 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include <gmp.h>
#include "ari.h"
#include "tools.h"

void init(mpz_t n,mpz_t p,mpz_t q,mpz_t d);
unsigned char *sign(unsigned char *M,long long int mLen,mpz_t n,mpz_t d,long long int sLen);
void checking(mpz_t m,mpz_t n,unsigned char *S,long long int sLen);

int main(){
    mpz_t n,p,q,d,m,mm,tmp;
    mpz_inits(n,p,q,d,m,mm,tmp,NULL);
    long long int sLen = 512+512;
    unsigned char M[] = {10,11,12,15,18,19,30,31,32,40,42,58,66,79,88,91,96,97,101,150,130,200,199,150};
    long long int mLen = strlen(M);
    OS2IP(mm, M, mLen);
    mpz_mod_ui(tmp,mm,16);
    gmp_printf("mod = %Zd\n",tmp);

    init(n, p, q, d);
    unsigned char *S = sign(M, mLen, n, d, sLen);

    checking(m, n, S, sLen);
    gmp_printf("m = %Zd\nmm = %Zd\n\n",m,mm);
    printf("%d\n", mpz_cmp(m,mm));
    mpz_clears(n,p,q,d,m,mm,tmp,NULL);
    
    return  0;
}

void init(mpz_t n,mpz_t p,mpz_t q,mpz_t d){
    gmp_randstate_t state;
    init_state(state);
    mpz_t tmp;
    mpz_init(tmp);
    do{
        mpz_urandomb(p,state,512);
        mpz_mod_ui(tmp,p,8);
    }while(!isprime(p)||mpz_cmp_ui(tmp, 3)!=0);

    do{
        mpz_urandomb(q,state,512);
        mpz_mod_ui(tmp,q,8);
    }while(!isprime(q)||mpz_cmp_ui(tmp, 7)!=0);

    mpz_mul(n,p,q);
    mpz_sub(d,n,p);
    mpz_sub(d,d,q);
    mpz_add_ui(d,d,5);
    mpz_tdiv_q_ui(d,d,8);
    gmp_printf("p = %Zd\nq = %Zd\nn = %Zd\nd = %Zd\n\n",p,q,n,d);
    mpz_clear(tmp);
}

unsigned char *sign(unsigned char *M,long long int mLen,mpz_t n,mpz_t d,long long int sLen){
    mpz_t m,s;
    mpz_inits(m,s,NULL);
    OS2IP(m, M, mLen);
    mpz_mul_ui(m,m,16);
    mpz_add_ui(m,m,6);
    int x = mpz_jacobi(m,n);
    printf("x = %d\n",x );
    if(x == -1)
        mpz_tdiv_q_ui(m,m,2);
    fast_pow_mod(s, m, d, n);
    unsigned char *S= I2OSP(s, sLen);
    mpz_clear(m);
    mpz_clear(s);
    return S;
}

void checking(mpz_t m,mpz_t n,unsigned char *S,long long int sLen){
    mpz_t s;
    mpz_init(s);
    OS2IP(s, S, sLen);
    gmp_printf("S = %Zd\n",s);
    mpz_mul(m,s,s);
    mpz_mod(m,m,n);
    mpz_mod_ui(s,m,8);
    unsigned long long mod = mpz_get_ui(s);
    printf("mod = %d\n",mod );
    if(mod == 3)
        mpz_mul_ui(m,m,2);
    else if(mod == 7)
        mpz_sub(m,n,m);
    else if(mod == 2){
        mpz_sub(m,n,m);
        mpz_mul_ui(m,m,2);
    }

    /*
     * check m in Mr
     */

    mpz_sub_ui(m,m,6);
    mpz_tdiv_q_ui(m,m,16);
    mpz_clear(s);
}
