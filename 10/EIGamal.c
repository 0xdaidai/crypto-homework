/*************************************************************************
 > File Name: EIGamal.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Sun 17 May 2020 04:48:32 PM CST
 ************************************************************************/
#include <stdio.h>
#include "ari.h"
#include "sha256.h"
#include "tools.h"

void init(mpz_t q, mpz_t a, mpz_t Xa,mpz_t Ya);
void sign(unsigned char *M,mpz_t q, mpz_t a, mpz_t Xa,mpz_t S1,mpz_t S2);
int checking(unsigned char *M,mpz_t a,mpz_t q,mpz_t Ya,mpz_t S1, mpz_t S2);

int main(){
    mpz_t q,a,Xa, Ya,S1,S2;
    mpz_inits(q,a,Xa,Ya,S1,S2,NULL);
    mpz_set_ui(q,19);
    mpz_set_ui(a,10);
    init(q, a, Xa, Ya);
    gmp_printf("q = %Zd\na = %Zd\nXa = %Zd\nYa = %Zd\n\n",q,a,Xa,Ya);
    unsigned char M[] = {112,113,114,90,91,96,97,98,99,85,86,87,16};
    sign(M, q, a, Xa, S1, S2);
    gmp_printf("S1 = %Zd\nS2 = %Zd\n\n",S1,S2);

    int res = checking(M, a, q, Ya, S1, S2);
    printf("res = %d\n", res);
    mpz_clears(q,a,Xa,Ya,S1,S2,NULL);
    return 0;
}

/*///////////////////////////////
get Xa,Ya by q,a
public key: q,a,Ya
private key: Xa
*/
/////////////////////////////////
void init(mpz_t q, mpz_t a, mpz_t Xa,mpz_t Ya){
    gmp_randstate_t state;
    init_state(state);
    mpz_urandomm(Xa,state,q);
    fast_pow_mod(Ya, a, Xa, q);
}

/*///////////////////////////////
make a sign for M
input q,a,Xa
return (S1,S2)
*/
/////////////////////////////////
void sign(unsigned char *M,mpz_t q, mpz_t a, mpz_t Xa,mpz_t S1,mpz_t S2){
    mpz_t K,q_1,m,tmp; //q_1 = q-1    
    mpz_inits(K,q_1,m,tmp,NULL);
    mpz_sub_ui(q_1,q,1);

    SHA256_CTX ctx;
    unsigned char buf[SHA256_BLOCK_SIZE];
    sha256_init(&ctx);
	sha256_update(&ctx, M, strlen(M));
	sha256_final(&ctx, buf);
    OS2IP(m, buf, strlen(buf));

    gmp_randstate_t state;
    init_state(state);
    while(mpz_cmp_ui(K, 0)<=0 || mpz_cmp_ui(tmp,1) != 0){
        mpz_urandomm(K,state,q);
        gcd(tmp,K,q_1);
    }
    fast_pow_mod(S1, a, K, q);
    invert(K, K, q_1);
    mpz_mul(S2,Xa,S1);
    mpz_sub(S2,m,S2);
    mpz_mul(S2,K,S2);
    mpz_mod(S2,S2,q_1);

    mpz_clears(K,q_1,m,tmp,NULL);
}

/*///////////////////////////////
ckeck the correctness
return 1/0
*/
/////////////////////////////////
int checking(unsigned char *M,mpz_t a,mpz_t q,mpz_t Ya,mpz_t S1, mpz_t S2){
    mpz_t V1,V2,m;
    mpz_inits(V1,V2,m,NULL);
    int res = 0;

    SHA256_CTX ctx;
    unsigned char buf[SHA256_BLOCK_SIZE];
    sha256_init(&ctx);
	sha256_update(&ctx, M, strlen(M));
	sha256_final(&ctx, buf);
    OS2IP(m, buf, strlen(buf));

    fast_pow_mod(V1, Ya, S1, q);
    fast_pow_mod(V2, S1, S2, q);
    mpz_mul(V2,V1,V2);
    fast_pow_mod(V1, a, m, q);
    mpz_mod(V2,V2,q);
    if(mpz_cmp(V1,V2) == 0){
        res = 1;
    }

    mpz_clears(V1,V2,m,NULL);
    return res;
}
