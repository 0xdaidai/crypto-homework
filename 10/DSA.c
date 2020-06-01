/*************************************************************************
 > File Name: 10/DSA.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Wed 20 May 2020 02:04:16 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include <gmp.h>
#include "ari.h"
#include "tools.h"
#include "sha256.h"
#define L 1024
#define N 160

void init(mpz_t p,mpz_t q,mpz_t g,mpz_t x,mpz_t y);
void sign(mpz_t r,mpz_t s,unsigned char *M,long long int mLen,mpz_t p,mpz_t q,mpz_t g,mpz_t x);
int checking(unsigned char *M,long long int mLen,mpz_t r,mpz_t s,mpz_t p,mpz_t q,mpz_t g,mpz_t y);

int main(){
    mpz_t p,q,g,x,y,r,s;
    mpz_inits(p,q,g,x,y,r,s,NULL);
    init(p, q, g, x, y);
    unsigned char M[] = {10,11,12,15,18,19,30,31,32,40,42,58,66,79,88,91,96,97,101,150,130,200,199,166,201};
    long long int mLen = strlen(M);
    sign(r, s, M, mLen, p, q, g, x);
    int res = checking(M, mLen, r, s, p, q, g, y);
    printf("res = %d\n",res );

    mpz_clears(p,q,g,x,y,r,s,NULL);
    return 0;
}

void init(mpz_t p,mpz_t q,mpz_t g,mpz_t x,mpz_t y){
    gmp_randstate_t state;
    init_state(state);
    mpz_t min,h,tmp;
    mpz_inits(min,tmp,NULL);
    mpz_init_set_ui(h,1);
    mpz_set_ui(g,1);
    mpz_mul_2exp(min,h,N-1);
    while(mpz_cmp(q,min)<0|| !isprime(q))
        mpz_urandomb(q,state,N);
    printf("q is prepared!\n");
    mpz_mul_2exp(min,h,L-1);// base = 2**1023
    mpz_tdiv_q(tmp,min,q);// tmp = base//q
    mpz_mul(p,q,tmp);
    mpz_add(p,p,q);
    mpz_add_ui(p,p,1);// after that we can see p>2**1023 and p-1 = k*q
    while(!isprime(p))
        mpz_add(p,p,q);
    printf("p is prepared!\n");
    mpz_sub_ui(tmp,p,1);
    mpz_tdiv_q(tmp,tmp,q); 
    mpz_set_ui(h,1); 
    while(mpz_cmp_ui(g, 1) == 0){
        mpz_add_ui(h,h,1);
        fast_pow_mod(g,h,tmp,p);
    }
    printf("g is ready!\n");
    while(mpz_cmp_ui(x,0)<=0)
        mpz_urandomm(x,state,q);
    fast_pow_mod(y, g, x, p);
    printf("x,y is prepared!\n");
    gmp_printf("p = %Zd\nq = %Zd\ng = %Zd\nx = %Zd\ny = %Zd\n\n",p,q,g,x,y);
    printf("init over\n\n");
    mpz_clears(min,h,tmp,NULL);
}

void sign(mpz_t r,mpz_t s,unsigned char *M,long long int mLen,mpz_t p,mpz_t q,mpz_t g,mpz_t x){
    gmp_randstate_t state;
    init_state(state);
    mpz_t k,e;
    mpz_inits(k,e,NULL);
	unsigned char buf[SHA256_BLOCK_SIZE];
    
    while(mpz_cmp_ui(k,0)<=0)
        mpz_urandomm(k,state,q);
    fast_pow_mod(r,g , k, p);
    mpz_mod(r,r,q);
    SHA256_CTX ctx;
	sha256_init(&ctx);
	sha256_update(&ctx, M, mLen);
	sha256_final(&ctx, buf);
    OS2IP(e, buf, SHA256_BLOCK_SIZE);
    mpz_mul(s,x,r);
    mpz_add(s,s,e);
    invert(k, k, q);
    mpz_mul(s,s,k);
    mpz_mod(s,s,q);
    printf("sign over!\n\n");
    mpz_clears(k,e,NULL);
}

int checking(unsigned char *M,long long int mLen,mpz_t r,mpz_t s,mpz_t p,mpz_t q,mpz_t g,mpz_t y){
    mpz_t w,u1,u2,v,e;
    mpz_inits(w,u1,u2,v,e,NULL);
	unsigned char buf[SHA256_BLOCK_SIZE];
    invert(w, s, q);
    SHA256_CTX ctx;
	sha256_init(&ctx);
	sha256_update(&ctx, M, mLen);
	sha256_final(&ctx, buf);
    OS2IP(e, buf, SHA256_BLOCK_SIZE);
    mpz_mul(u1,e,w);
    mpz_mod(u1,u1,q);
    mpz_mul(u2,r,w);
    mpz_mod(u2,u2,q);
    fast_pow_mod(v, g, u1, p);
    fast_pow_mod(w, y, u2, p);
    mpz_mul(v,v,w);
    mpz_mod(v,v,p);
    mpz_mod(v,v,q);
    int res = 0;
    if(mpz_cmp(v,r) == 0)
        res = 1;

    mpz_clears(w,u1,u2,v,e,NULL);
    return res;
}
