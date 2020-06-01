/*************************************************************************
 > File Name: Schnorr.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Tue 19 May 2020 05:58:35 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "ari.h"
#include "sha1.h"
#include "tools.h"

void init(mpz_t p,mpz_t q,mpz_t a,mpz_t s,mpz_t v);
void sign(mpz_t e,mpz_t y,unsigned char *M,long long int mLen,mpz_t p,mpz_t q,mpz_t a,mpz_t s);
int checking(unsigned char *M,long long int mLen,mpz_t e,mpz_t y,mpz_t p,mpz_t a,mpz_t v);

int main(){
    mpz_t p,q,a,s,v,e,y;
    mpz_inits(p,q,a,s,v,e,y,NULL);

    unsigned char M[] = {10,11,12,15,18,19,30,31,32,40,42,58,66,79,88,91,96,97,101,150,130,200,199,166,201};
    long long int mLen = strlen(M);
    init(p, q, a, s, v);
    sign(e, y, M,mLen, p, q, a, s);
    int res = checking(M,mLen, e, y, p, a, v);
    if(res)
        printf("True\n");
    else
        printf("False\n");

    mpz_clears(p,q,a,s,v,e,y,NULL);
    return 0;
}


/*
 * init the keys
 * p :1024b
 * q :160b
 * public key: v
 * private key: s
 */
void init(mpz_t p,mpz_t q,mpz_t a,mpz_t s,mpz_t v){
    mpz_t base,tmp;
    gmp_randstate_t state;
    init_state(state);
    mpz_init(base);
    mpz_init_set_ui(tmp,1);
    mpz_set_ui(a,1);
    mpz_mul_2exp(base,tmp,159);
    while(mpz_cmp(q,base)<0|| !isprime(q))
        mpz_urandomb(q,state,160);
    printf("q is prepared!\n");
    mpz_mul_2exp(base,tmp,1023);// base = 2**1023
    mpz_tdiv_q(tmp,base,q);// tmp = base//q
    mpz_mul(p,q,tmp);
    mpz_add(p,p,q);
    mpz_add_ui(p,p,1);// after that we can see p>2**1023 and p-1 = k*q
    while(!isprime(p))
        mpz_add(p,p,q);
    printf("p is prepared!\n");
    mpz_sub_ui(tmp,p,1);
    mpz_tdiv_q(base,tmp,q); // base = (p-1)/q
    mpz_set_ui(tmp,1); //h == tmp
    while(mpz_cmp_ui(a, 1) == 0){
        mpz_add_ui(tmp,tmp,1);
        fast_pow_mod(a,tmp,base,p);
    }
    printf("a is ready!\n");
    while(mpz_cmp_ui(s, 0)<=0)
        mpz_urandomm(s,state,q);
    fast_pow_mod(v, a, s, p);
    invert(v, v, p);
    printf("s,v is ready!\n");
    gmp_printf("p = %Zd\nq = %Zd\na = %Zd\ns = %Zd\nv = %Zd\n\n",p,q,a,s,v);
    mpz_clears(tmp,base,NULL);
}

/*
 * sign for the message
 * return (e,y)
 */
void sign(mpz_t e,mpz_t y,unsigned char *M,long long int mLen,mpz_t p,mpz_t q,mpz_t a,mpz_t s){
    SHA1_CTX ctx;
	unsigned char buf[SHA1_BLOCK_SIZE];
    mpz_t r,x;
    mpz_inits(r,x,NULL);
    gmp_randstate_t state;
    init_state(state);
    while(mpz_cmp_ui(r, 0)<=0)
        mpz_urandomm(r,state,q);
    fast_pow_mod(x, a, r, p);
    gmp_printf("x = %Zd\n",x); //
    unsigned char *x_c = I2OSP(x, 128);
    unsigned char *e_c = array_append(M, mLen, x_c, 128);
	sha1_init(&ctx);
	sha1_update(&ctx, e_c, mLen+128);
	sha1_final(&ctx, buf);
    OS2IP(e, buf, SHA1_BLOCK_SIZE);
    mpz_mul(y,s,e);
    mpz_add(y,y,r);
    mpz_mod(y,y,q);
    printf("sign over!\n");
    gmp_printf("e = %Zd\ny = %Zd\n\n",e,y);
    mpz_clears(r,x,NULL);
}

/* check the correctness
 * return 1/0
 */
int checking(unsigned char *M,long long int mLen,mpz_t e,mpz_t y,mpz_t p,mpz_t a,mpz_t v){
    SHA1_CTX ctx;
    int res = 0;
	unsigned char buf[SHA1_BLOCK_SIZE];
    mpz_t x,tmp,e_;
    mpz_inits(x,e_,tmp,NULL);
    fast_pow_mod(x, a, y, p);
    fast_pow_mod(tmp, v, e, p);
    mpz_mul(x,x,tmp);
    mpz_mod(x,x,p);
    gmp_printf("x\' = %Zd\n",x); //
    unsigned char *x_c = I2OSP(x, 128);
    unsigned char *e_c = array_append(M, mLen, x_c, 128);
	sha1_init(&ctx);
	sha1_update(&ctx, e_c, mLen+128);
	sha1_final(&ctx, buf);
    OS2IP(e_, buf, SHA1_BLOCK_SIZE);
    if(mpz_cmp(e,e_)==0)
        res = 1;
    mpz_clears(x,tmp,e_,NULL);
    return res;
}
