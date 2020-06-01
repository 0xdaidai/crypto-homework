/*************************************************************************
 > File Name: 11/sm2_sign.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Mon 01 Jun 2020 02:36:45 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "ecc_ari.h"
#include "sha256.h"
#include "tools.h"

void sign(mpz_t r,mpz_t s,unsigned char *ZA,mpz_t dA,unsigned char *M,long long int mLen ,mpz_t a,mpz_t q,ecc *G,mpz_t n);
int checking(mpz_t r,mpz_t s,unsigned char *ZA,unsigned char *M,long long int mLen ,mpz_t a,mpz_t b,mpz_t q,ecc *G,mpz_t n,ecc *pA);
unsigned char *zA(unsigned char *IDa,long long int entLen,mpz_t a,mpz_t b,mpz_t q,ecc *G,ecc *pA);

int main(){
    printf("BUFSIZ: %d\n",BUFSIZ);
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

    mpz_set_str(gmp_t,"F4A38489 E32B45B6 F876E3AC 2168CA39 2362DC8F 23459C1D 1146FC3D BFB7BC9A",16);
    unsigned char *ZA =I2OSP(gmp_t, 256);
    sign(r, s, ZA, dA, M, mLen, a, q, &G, n);
    int res = checking(r, s, ZA, M, mLen, a, b, q, &G, n, &pA);
    printf("res = %d\n", res);

    mpz_clears(q,a,b,n,dA,r,s,gmp_t,NULL);
    ecc_clear(&G);
    ecc_clear(&pA);
    ecc_clear(&ecc_t);
    return 0;
}

void sign(mpz_t r,mpz_t s,unsigned char *ZA,mpz_t dA,unsigned char *M,long long int mLen ,mpz_t a,mpz_t q,ecc *G,mpz_t n){
    long long qLen = mpz_sizeinbase(q,2);
    printf("qLen = %lld\n",qLen);
    unsigned char *M_ = array_append(ZA,64, M, mLen);

    SHA256_CTX ctx;
    unsigned char *buf = (unsigned char*)malloc(sizeof(unsigned char)*64);
    memset(buf,0,64);
    sha256_init(&ctx);
    sha256_update(&ctx, M_, 64 + mLen);
    sha256_final(&ctx, buf);
    mpz_t e;mpz_init(e);
    OS2IP(e, buf, 64);
    gmp_printf("e = %Zx\n",e);
    int flag=1;
    mpz_t k;mpz_init(k);
    mpz_t tmp1;mpz_init(tmp1);
    gmp_randstate_t state; init_state(state);
    ecc tmp;ecc_init(&tmp);

    while(flag){
        while(mpz_cmp_ui(k, 0) == 0)
            mpz_urandomm(k,state,n);
        mpz_set_str(k,"6CB28D99 385C175C 94F94E93 4817663F C176D925 DD72B727 260DBAAE 1FB2F96F",16);
        ecc_mul(&tmp, G, k, q, a);
        // gmp_printf("kg:\nx = %Zx\ny = %Zx\n\n",tmp.x,tmp.y);
        mpz_add(r,tmp.x,e);
        mpz_mod(r,r,n);
        mpz_add(tmp1,r,k);
        if(mpz_cmp_ui(r, 0)==0||mpz_cmp(tmp1,n)==0)
            continue;
        mpz_add_ui(s,dA,1);
        mpz_invert(s,s,n);
        mpz_mul(tmp1,r,dA);
        mpz_sub(tmp1,k,tmp1);
        mpz_mul(s,s,tmp1);
        mpz_mod(s,s,n);
        if(mpz_cmp_ui(s, 0)!=0)
            break;
    }
    printf("sign over!\n");
    gmp_printf("r = %Zx\n",r);
    gmp_printf("s = %Zx\n",s);
    mpz_clears(e,k,tmp1,NULL);
    ecc_clear(&tmp);
}

int checking(mpz_t r,mpz_t s,unsigned char *ZA,unsigned char *M,long long int mLen ,mpz_t a,mpz_t b,mpz_t q,ecc *G,mpz_t n,ecc *pA){
    if(mpz_cmp(r,n)>=0||mpz_cmp_d(r,0)<=0){
        printf("wrong range of r\n");
        return 0;
    }
    if(mpz_cmp(s,n)>=0||mpz_cmp_d(s,0)<=0){
        printf("wrong range of s\n");
        return 0;
    }
    long long qLen = mpz_sizeinbase(q,2);
    unsigned char *M_ = array_append(ZA, 64, M, mLen);

    SHA256_CTX ctx;
    unsigned char *buf = (unsigned char*)malloc(sizeof(unsigned char)*64);
    memset(buf,0,64);
    sha256_init(&ctx);
    sha256_update(&ctx, M_, 64 + mLen);
    sha256_final(&ctx, buf);
    mpz_t e;mpz_init(e);
    OS2IP(e, buf, 64);
    gmp_printf("e: %Zx\n\n",e);

    mpz_t t;mpz_init(t);
    mpz_add(t,r,s);
    mpz_mod(t,t,n);
    if(mpz_cmp_d(t,0)==0){
        mpz_clears(t,e,NULL);
        printf("wrong: t = 0\n");
        return 0;
    }

    ecc e1,e2;
    ecc_init(&e1);
    ecc_init(&e2);
    ecc_mul(&e1, G, s, q, a);
    ecc_mul(&e2, pA, t, q, a);
    ecc_add(&e1,&e1,&e2,q,a);

    mpz_t R;mpz_init(R);
    mpz_add(R,e,e1.x);
    mpz_mod(R,R,n);
    gmp_printf("R = %Zd\n",R);
    int res = 0;
    if(mpz_cmp(R,r) == 0)
        res = 1;
    else
        printf("wrong: R != r\n");

    mpz_clears(e,t,R,NULL);
    ecc_clear(&e1);
    ecc_clear(&e2);
    return res;
}

unsigned char *zA(unsigned char *IDa,long long int entLen,mpz_t a,mpz_t b,mpz_t q,ecc *G,ecc *pA){
    unsigned char *ENTLa = (unsigned char *)malloc(sizeof(unsigned char)*2);
    ENTLa[0] = entLen&0xFF;ENTLa[1] = entLen>>8;
    long long qLen = mpz_sizeinbase(q,2);
    unsigned char *a_ch = I2OSP(a, qLen);
    unsigned char *b_ch = I2OSP(b, qLen);
    unsigned char *xG_ch = I2OSP(G->x, qLen);
    unsigned char *yG_ch = I2OSP(G->y, qLen);
    unsigned char *xP_ch = I2OSP(pA->x, qLen);
    unsigned char *yP_ch = I2OSP(pA->y, qLen);
    unsigned char *input = (unsigned char *)malloc(sizeof(unsigned char)*(2+entLen+6*qLen));
    unsigned long long index = 0;
    memcpy(input,ENTLa,2);index+=2;
    memcpy(input+index,IDa,entLen);index+=entLen;
    memcpy(input+index,a_ch,qLen);index+=qLen;
    memcpy(input+index,b_ch,qLen);index+=qLen;
    memcpy(input+index,xG_ch,qLen);index+=qLen;
    memcpy(input+index,yG_ch,qLen);index+=qLen;
    memcpy(input+index,xP_ch,qLen);index+=qLen;
    memcpy(input+index,yP_ch,qLen);index+=qLen;

    SHA256_CTX ctx;
    unsigned char *buf = (unsigned char*)malloc(sizeof(unsigned char)*BUFSIZ);
    sha256_init(&ctx);
    sha256_update(&ctx, input, 2+entLen+6*qLen);
    sha256_final(&ctx, buf);

    free(ENTLa);free(a_ch);free(b_ch);free(xG_ch);free(yG_ch);free(xP_ch);free(yP_ch);free(input);
    return buf;
}
