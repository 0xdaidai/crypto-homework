/*************************************************************************
 > File Name: ecc_ari.h
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Sat 30 May 2020 05:29:55 PM CST
 ************************************************************************/

#ifndef ECC_ARI_H
#define ECC_ARI_H 

#include <gmp.h>
#include "ari.h"

typedef struct ecc{
    mpz_t x;mpz_t y;
}ecc;

void ecc_init(ecc *p);
void ecc_clear(ecc *p);
int ecc_eq(ecc p,ecc q);
void ecc_add(ecc *r,ecc *p, ecc *q,mpz_t n,mpz_t a);
void ecc_sub(ecc *r,ecc *p, ecc *q,mpz_t n,mpz_t a);
void ecc_mul(ecc *r,ecc *p, mpz_t x,mpz_t n,mpz_t a);
int notin(ecc *X,mpz_t a,mpz_t b,mpz_t p);

void ecc_init(ecc *p){
    mpz_init(p->x);
    mpz_init(p->y);
}

void ecc_clear(ecc *p){
    mpz_clear(p->x);
    mpz_clear(p->y);
}

int ecc_eq(ecc p,ecc q){
    if(mpz_cmp(p.x,q.x)==0&&mpz_cmp(p.y,q.y)==0)
        return 1;
    return 0;
}

void ecc_add(ecc *r,ecc *p, ecc *q,mpz_t n,mpz_t a){
    mpz_t lambda,tmp,rx;
    mpz_inits(lambda,tmp,rx,NULL);
    if(!ecc_eq(*p, *q)){
        mpz_sub(tmp,q->x,p->x);
        mpz_invert(tmp, tmp, n);
        mpz_sub(lambda,q->y,p->y);
    }
    else {
        mpz_mul_ui(tmp,p->y,2);
        mpz_invert(tmp, tmp, n);
        mpz_mul(lambda,p->x,p->x);
        mpz_mul_ui(lambda,lambda,3);
        mpz_add(lambda,lambda,a);
    }
    mpz_mul(lambda,tmp,lambda);
    mpz_mod(lambda,lambda,n);
    
    mpz_mul(tmp,lambda,lambda);
    mpz_sub(tmp,tmp,p->x);
    mpz_sub(tmp,tmp,q->x);
    mpz_mod(tmp,tmp,n);
    mpz_set(rx,tmp); // get res->x

    mpz_sub(tmp,p->x,rx);
    mpz_mul(tmp,lambda,tmp);
    mpz_sub(tmp,tmp,p->y);
    mpz_mod(tmp,tmp,n);
    mpz_set(r->x,rx); // get res->y
    mpz_set(r->y,tmp); // get res->y
    mpz_clears(lambda,tmp,rx,NULL);
}

void ecc_sub(ecc *r,ecc *p, ecc *q,mpz_t n,mpz_t a){
    mpz_mod(q->y,q->y,n);
    mpz_sub(q->y,n,q->y);
    ecc_add(r, p, q, n, a);
}

void ecc_mul(ecc *r,ecc *p, mpz_t k,mpz_t n,mpz_t a){
    mpz_t x;
    mpz_init_set(x,k);
    ecc tmp,res;
    ecc_init(&tmp);
    ecc_init(&res);
    int flag = 1;
    mpz_set(tmp.x,p->x);
    mpz_set(tmp.y,p->y);
    while(mpz_cmp_ui(x, 0)>0){
        if(mpz_odd_p(x)){
            if(flag){
                flag = 0;
                mpz_set(res.x,tmp.x);
                mpz_set(res.y,tmp.y);
            }
            else
                ecc_add(&res, &res,&tmp, n, a);
        }
        ecc_add(&tmp, &tmp, &tmp, n, a);
        mpz_tdiv_q_2exp(x,x,1);
    }

    mpz_set(r->x,res.x);
    mpz_set(r->y,res.y);
    
    ecc_clear(&tmp);
    ecc_clear(&res);
    mpz_clear(x);
}

int notin(ecc *X,mpz_t a,mpz_t b,mpz_t p){
    int res = 1;
    mpz_t left,right;
    mpz_init(left);mpz_init(right);
    mpz_mul(left,X->y,X->y);
    mpz_mul(right,X->x,X->x);
    mpz_add(right,right,a);
    mpz_mod(right,right,p);
    mpz_mul(right,right,X->x);
    mpz_add(right,right,b);
    mpz_mod(right,right,p);
    if(mpz_cmp(left,right))
        res = 0;
    mpz_clears(left,right,NULL);
    return res;
}

#endif
