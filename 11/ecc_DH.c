/*************************************************************************
 > File Name: 11/ecc_DH.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Mon 01 Jun 2020 11:38:35 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include <gmp.h>
#include "ecc_ari.h"

void getKey(ecc K,ecc PB,mpz_t nA,mpz_t q,mpz_t a){
    ecc_mul(&K, &PB, nA, q, a);
    printf("GetKey\nx = %Zx\ny = %Zx\n\n",K.x,K.y);
}

int main(){
    mpz_t q,a,b,n,dA,r,s,gmp_t;
    mpz_inits(q,a,b,n,dA,r,s,gmp_t,NULL);
    ecc G,pA,ecc_t;
    ecc_init(&G);
    ecc_init(&pA);
    ecc_init(&ecc_t);

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


    mpz_t nA,nB;
    mpz_inits(nA,nB,NULL);
    mpz_set_str(nA,  "8542D69E4C044F18E8B9245BF6FF7DD297720630485628D5AE74EE7C3200000",16);
    mpz_set_str(nB,  "8542D69E4C044F18E8B9245BF6FF7DD297720630485628D5AE74EE7C3200012",16);
    ecc PB;ecc_init(&PB);
    ecc_mul(&PB, &G, nB, q, a);
    getKey(pA, PB, nA, q, a);

    mpz_clears(q,a,b,n,dA,r,s,gmp_t,nA,nB,NULL);
    ecc_clear(&G);
    ecc_clear(&pA);
    ecc_clear(&ecc_t);
    ecc_clear(&PB);
 
    return 0;
}
