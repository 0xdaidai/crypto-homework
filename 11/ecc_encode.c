/*************************************************************************
 > File Name: 11/ecc_encode.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Sun 31 May 2020 01:55:00 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "ecc_ari.h"
#include "tools.h"

void encode(ecc *cm[],ecc *Pm,ecc *G,mpz_t a,mpz_t q,ecc *pB);
void decode(ecc *Pm,ecc *cm[],mpz_t q,mpz_t a,mpz_t nB);

int main(){
    mpz_t q,a,b,n,dA,r,s,gmp_t,nA;
    mpz_inits(q,a,b,n,dA,r,s,nA,gmp_t,NULL);
    ecc G,pA,ecc_t,cm[2],Pm,Pmm;
    ecc_init(&G);
    ecc_init(&pA);
    ecc_init(&Pm);
    ecc_init(&Pmm);
    ecc_init(&ecc_t);
    ecc_init(&cm[0]);
    ecc_init(&cm[1]);
    ecc *CM[2];


    mpz_set_str(q,   "8542D69E4C044F18E8B92435BF6FF7DE457283915C45517D722EDB8B08F1DFC3",16);
    mpz_set_str(a,   "787968B4FA32C3FD2417842E73BBFEFF2F3C848B6831D7E0EC65228B3937E498",16);
    mpz_set_str(b,   "63E4C6D3B23BOC849CF84241484BFE48F61D59A5B16BAo6E6E12D1DA27C5249A",16);
    mpz_set_str(n,   "8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E79B7",16);
    mpz_set_str(dA,  "128B2FA8BD433C6C068C8D803DFF79792A519A55171B1B650C23661D15897263",16);
    mpz_set_str(G.x, "421DEBD61B62EAB6746434EBC3CC315E32220B3BADD50BDC4C4E6C147FEDD43D",16);
    mpz_set_str(G.y, "0680512BCBB42C07D47349D2153B70C4E5D7FDFCBFA36EA1A85841B9E46E09A2",16);
    mpz_set_str(nA,   "8542D69E4C044F18E8B92435BF6FF7DD297720630485628D5AE74EE7C32E77",16);
    mpz_set_str(Pm.x,"0AE4C7798AA0F119471BEE11825BE46202BB79E2A5844495E97C04FF4DF2548A",16);
    mpz_set_str(Pm.y,"7C0240F88F1CD4E16352A73C17B7F16F07353E53A176D684A9FE0C6BB798E857",16);
    ecc_mul(&pA, &G, nA, q, a);

    CM[0] = &cm[0];
    CM[1] = &cm[1];
    encode(CM, &Pm, &G, a, q, &pA);
    decode(&Pmm, CM, q, a, nA);
    printf("res : %d\n", ecc_eq(Pm, Pmm));

    ecc_clear(&cm[0]);
    ecc_clear(&cm[1]);
    mpz_clears(q,a,b,n,nA,dA,r,s,gmp_t,NULL);
    ecc_clear(&G);
    ecc_clear(&pA);
    ecc_clear(&ecc_t);
    ecc_clear(&Pm);
    ecc_clear(&Pmm);
    return 0;
}

void encode(ecc *cm[],ecc *Pm,ecc *G,mpz_t a,mpz_t q,ecc *pB){
    mpz_t k;
    mpz_init(k);
    gmp_randstate_t state;
    init_state(state);
    mpz_urandomb(k,state,512);
    ecc_mul(cm[0], G, k, q, a);
    ecc_mul(cm[1], pB, k, q, a);
    ecc_add(cm[1], cm[1], Pm, q, a);
    mpz_clear(k);
    printf("encode !\n");
}

void decode(ecc *Pm,ecc *cm[],mpz_t q,mpz_t a,mpz_t nB){
   ecc_mul(Pm, cm[0], nB, q, a); 
   ecc_sub(Pm, cm[1], Pm, q, a);
}
