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

void sign(mpz_t r,mpz_t s,mpz_t dA,unsigned char *M,long long int mLen ,unsigned char *IDa,long long int entLen,mpz_t a,mpz_t b,mpz_t q,ecc *G,mpz_t n,ecc *pA);
int checking(mpz_t r,mpz_t s,unsigned char *M,long long int mLen ,unsigned char *IDa,long long int entLen,mpz_t a,mpz_t b,mpz_t q,ecc *G,mpz_t n,ecc *pA);
unsigned char *zA(unsigned char *IDa,long long int entLen,mpz_t a,mpz_t b,mpz_t q,ecc *G,ecc *pA);

int main(){
    printf("sizeof(uchar) = %d\n",sizeof(unsigned char) );
    mpz_t q,a,b,n,dA,r,s,gmp_t;
    mpz_inits(q,a,b,n,dA,r,s,gmp_t,NULL);
    ecc P,Q,ecc_t;
    ecc_init(&P);
    ecc_init(&Q);
    ecc_init(&ecc_t);

    unsigned char IDa[] = "ALICE123@YAHOO,COM";
    long long int entLen = 0x0090;
    unsigned char M[]="message digest";
    long long int mLen = strlen(M);

    mpz_set_ui(q,23);
    mpz_set_ui(a,1);
    mpz_set_ui(b,1);

    mpz_set_ui(P.x,3);
    mpz_set_ui(P.y,10);
    mpz_set_ui(Q.x,9);
    mpz_set_ui(Q.y,7);
    ecc_add(&ecc_t, &P, &Q, q, a);
    gmp_printf("x = %Zd\ny=%Zd\n\n",ecc_t.x,ecc_t.y);
    ecc_add(&ecc_t, &P, &P, q, a);
    gmp_printf("x = %Zd\ny=%Zd\n\n",ecc_t.x,ecc_t.y);
    mpz_set_ui(gmp_t,2);
    ecc_mul(&ecc_t, &P, gmp_t, q, a);
    gmp_printf("x = %Zd\ny=%Zd\n\n",ecc_t.x,ecc_t.y);

    // sign(r, s, dA, M, mLen, IDa, entLen, a, b, q, &G, n, &pA);
    // int res = checking(r, s, M, mLen, IDa, entLen, a, b, q, &G, n, &pA);
    // printf("res = %d\n", res);

    mpz_clears(q,a,b,n,dA,r,s,gmp_t,NULL);
    ecc_clear(&P);
    ecc_clear(&Q);
    ecc_clear(&ecc_t);
    return 0;
}

void sign(mpz_t r,mpz_t s,mpz_t dA,unsigned char *M,long long int mLen ,unsigned char *IDa,long long int entLen,mpz_t a,mpz_t b,mpz_t q,ecc *G,mpz_t n,ecc *pA){
    long long qLen = mpz_sizeinbase(q,2);
    gmp_printf("q = %Zd\n",q);
    printf("qLen = %lld\n",qLen);
    unsigned char *ZA=zA(IDa, entLen, a, b, q, G, pA);
    printf("ZA OVER!\n");
    unsigned char *M_ = array_append(ZA, 2+entLen+6*qLen, M, mLen);

    SHA256_CTX ctx;
    unsigned char *buf = (unsigned char*)malloc(sizeof(unsigned char)*BUFSIZ);
    sha256_init(&ctx);
    sha256_update(&ctx, M_, 2+entLen+6*qLen + mLen);
    sha256_final(&ctx, buf);
    mpz_t e;mpz_init(e);
    OS2IP(e, buf, BUFSIZ);
    int flag=1;
    mpz_t k;mpz_init(k);
    mpz_t tmp1;mpz_init(tmp1);
    gmp_randstate_t state; init_state(state);
    ecc tmp;ecc_init(&tmp);

    while(flag){
        while(mpz_cmp_ui(k, 0) == 0)
            mpz_urandomm(k,state,n);
        ecc_mul(&tmp, G, k, q, a);
        mpz_add(r,tmp.x,e);
        mpz_mod(r,r,n);
        mpz_add(tmp1,r,k);
        if(mpz_cmp_ui(r, 0)||mpz_cmp(tmp1,n))
            flag = 2;
        mpz_add_ui(s,dA,1);
        mpz_invert(s,s,n);
        mpz_mul(tmp1,r,dA);
        mpz_sub(tmp1,k,tmp1);
        mpz_mul(s,s,tmp1);
        mpz_mod(s,s,n);
        if(mpz_cmp_ui(s, 0)!=0&&flag == 2)
            break;
        flag = 1;
    }
    printf("sign over!\n");
    gmp_printf("r = %Zd\n",r);
    gmp_printf("s = %Zd\n",s);
    mpz_clears(e,k,tmp1,NULL);
    ecc_clear(&tmp);
}

int checking(mpz_t r,mpz_t s,unsigned char *M,long long int mLen ,unsigned char *IDa,long long int entLen,mpz_t a,mpz_t b,mpz_t q,ecc *G,mpz_t n,ecc *pA){
    if(mpz_cmp(r,n)>=0||mpz_cmp_d(r,0)<=0){
        printf("wrong range of r\n");
        return 0;
    }
    if(mpz_cmp(s,n)>=0||mpz_cmp_d(s,0)<=0){
        printf("wrong range of s\n");
        return 0;
    }
    long long qLen = mpz_sizeinbase(q,2);
    unsigned char *ZA=zA(IDa, entLen, a, b, q, G, pA);
    unsigned char *M_ = array_append(ZA, 2+entLen+6*qLen, M, mLen);

    SHA256_CTX ctx;
    unsigned char *buf = (unsigned char*)malloc(sizeof(unsigned char)*BUFSIZ);
    sha256_init(&ctx);
    sha256_update(&ctx, M_, 2+entLen+6*qLen + mLen);
    sha256_final(&ctx, buf);
    mpz_t e;mpz_init(e);
    OS2IP(e, buf, BUFSIZ);

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
