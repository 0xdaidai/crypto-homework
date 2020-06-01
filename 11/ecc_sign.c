/*************************************************************************
 > File Name: 11/ecc_sign.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Sun 31 May 2020 06:15:08 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "ecc_ari.h"
#include "sha256.h"

void getKey(ecc *Q,mpz_t d,mpz_t n,ecc *G,mpz_t q,mpz_t a);
void sign(mpz_t r,mpz_t s,unsigned char *M,long long int mLen,mpz_t d,ecc *G,mpz_t n,mpz_t q,mpz_t a);
int checking(mpz_t r,mpz_t s,ecc *Q,unsigned char *M,long long int mLen,mpz_t n,ecc *G,mpz_t q,mpz_t a,mpz_t b);

int main(){
    mpz_t q,a,b,n,dA,r,s,gmp_t;
    mpz_inits(q,a,b,n,dA,r,s,gmp_t,NULL);
    ecc G,pA,ecc_t,Q;
    ecc_init(&G);
    ecc_init(&pA);
    ecc_init(&ecc_t);
    ecc_init(&Q);

    mpz_set_str(gmp_t,"414C494345313233405941484F4F2E434F4D",16);
    long long int entLen = 0x0090;
    unsigned char M[]="message digest";
    long long int mLen = strlen(M);


    mpz_set_str(q,   "8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3",16);
    mpz_set_str(a,   "787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498",16);
    mpz_set_str(b,   "63E4C6D3B23BOC849CF84241484BFE48F61D59A5B16BAo6E6E12D1DA27C5249A",16);
    mpz_set_str(n,   "8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7",16);
    mpz_set_str(dA,  "128B2FA8BD433C6C068C8D803DFF79792A519A55171B1B650C23661D15897263",16);
    mpz_set_str(G.x, "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D",16);
    mpz_set_str(G.y, "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2",16);
    mpz_set_str(pA.x,"0AE4C7798AA0F119471BEE11825BE46202BB79E2A5844495E97C04FF4DF2548A",16);
    mpz_set_str(pA.y,"7C0240F88F1CD4E16352A73C17B7F16F07353E53A176D684A9FE0C6BB798E857",16);

    getKey(&Q, gmp_t, n, &G, q, a);
    sign(r, s, M, mLen, gmp_t, &G, n, q, a);
    int res = checking(r, s, &Q, M, mLen, n, &G, q, a, b);
    printf("res = %d\n",res);

    mpz_clears(q,a,b,n,dA,r,s,gmp_t,NULL);
    ecc_clear(&G);
    ecc_clear(&pA);
    ecc_clear(&ecc_t);
    ecc_clear(&Q);
  
    return 0;
}

void getKey(ecc *Q,mpz_t d,mpz_t n,ecc *G,mpz_t q,mpz_t a){
    gmp_randstate_t state;
    init_state(state);
    mpz_init(d);
    while(mpz_cmp_ui(d,0)==0)
        mpz_urandomm(d,state,n);
    ecc_mul(Q, G, d, q, a);
    printf("Key is prepared!\n");
}

void sign(mpz_t r,mpz_t s,unsigned char *M,long long int mLen,mpz_t d,ecc *G,mpz_t n,mpz_t q,mpz_t a){
    mpz_t k,t,e;
    mpz_inits(k,t,e,NULL);
    gmp_randstate_t state;
    init_state(state);
    ecc P;ecc_init(&P);
    unsigned char buf[SHA256_BLOCK_SIZE];
    memset(buf,0,sizeof(unsigned char)*SHA256_BLOCK_SIZE);
    SHA256_CTX ctx;

    do{
        mpz_set_ui(r,0);
        mpz_set_ui(s,0);
        do{
            while(mpz_cmp_ui(k,0)==0)
                mpz_urandomm(k,state,n);
            ecc_mul(&P, G, k, q, a);
            mpz_mod(r,P.x,n);
        }while(mpz_cmp_ui(r, 0)==0);

        mpz_invert(t,k,n);
        
        sha256_init(&ctx);
	    sha256_update(&ctx, M, mLen);
	    sha256_final(&ctx, buf);
        OS2IP(e, buf, SHA256_BLOCK_SIZE);

        mpz_mul(s,d,r);
        mpz_mod(s,s,n);
        mpz_add(s,s,e);
        mpz_mod(s,s,n);
        mpz_mul(s,s,t);
        mpz_mod(s,s,n);
    }while(mpz_cmp_ui(s, 0)==0);

    mpz_clears(k,t,e,NULL);
    ecc_clear(&P);
}

int checking(mpz_t r,mpz_t s,ecc *Q,unsigned char *M,long long int mLen,mpz_t n,ecc *G,mpz_t q,mpz_t a,mpz_t b){
    if(mpz_cmp(r,n)>=0||mpz_cmp_d(r, 0)<=0)
        return 0;
    if(mpz_cmp(s,n)>=0||mpz_cmp_d(s, 0)<=0)
        return 0;
    mpz_t e,w,u1,u2,v;
    mpz_inits(e,w,u1,u2,v,NULL);
    unsigned char buf[SHA256_BLOCK_SIZE];
    memset(buf,0,sizeof(unsigned char)*SHA256_BLOCK_SIZE);
    SHA256_CTX ctx;
    
    sha256_init(&ctx);
    sha256_update(&ctx, M, mLen);
    sha256_final(&ctx, buf);
    OS2IP(e, buf, SHA256_BLOCK_SIZE);

    mpz_invert(w,s,n);
    mpz_mul(u1,e,w);mpz_mul(u2,r,w);
    ecc X,tmp;
    ecc_init(&X);
    ecc_init(&tmp);
    ecc_mul(&X, G, u1, q, a);
    ecc_mul(&tmp, Q, u2, q, a);
    ecc_add(&X, &X, &tmp, q, a);

    if(notin(&X,a,b,q))
        return 0;
    mpz_mod(v,X.x,n);
    if(mpz_cmp(v,r))
        return 0;
    return 1;
}
