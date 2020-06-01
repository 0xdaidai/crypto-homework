/*************************************************************************
 > File Name: rabin.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Wed 20 May 2020 06:12:40 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "ari.h"
#include "tools.h"
#define L 512

typedef struct field{
	mpz_t x,y;
}f_2;

const mpz_t w;

void init(mpz_t p,mpz_t q,mpz_t n);
void sign(unsigned char *M,long long int mLen,mpz_t n,mpz_t s,mpz_t p,mpz_t q);
void solve_pq(mpz_t x,mpz_t a,mpz_t p);
void solve_n(mpz_t x,mpz_t a,mpz_t n,mpz_t p,mpz_t q);
int checking(mpz_t R[],long long int rLen,mpz_t s,mpz_t n);

int main(){
    mpz_t p,q,n,s;
    mpz_inits(p,q,n,s,NULL);
    unsigned char M[] = {10,11,12,15,18,19,30,31,32,40,42,58,66,79,88,91,96,97,101,150,130,200,199,166,201};
    long long int mLen = strlen(M);
    init(p, q, n);
    sign(M, mLen, n, s, p, q);
    mpz_clears(p,q,n,s,NULL);
}

void init(mpz_t p,mpz_t q,mpz_t n){
    mpz_t min;
    gmp_randstate_t state;
    init_state(state);
    mpz_init_set_ui(min,1);
    mpz_mul_2exp(min,min,L-1);
    while(mpz_cmp_ui(p, 0)<=0 || !isprime(p))
        mpz_urandomb(p,state,L);
    while(mpz_cmp_ui(q, 0)<=0||mpz_cmp(p,q) == 0||!isprime(q))
        mpz_urandomb(q,state,L);
    mpz_mul(n,p,q);
    printf("init over!\n");
    gmp_printf("p = %Zd\nq = %Zd\nn = %Zd\n\n",p,q,n);
    mpz_clear(min);
}

void sign(unsigned char *M,long long int mLen,mpz_t n,mpz_t s,mpz_t p,mpz_t q){
    mpz_t m;
    mpz_init(m);
    OS2IP(m,M, mLen);
    gmp_printf("m = %Zd\n",m);
    if(mpz_legendre(m,n) == -1){
        printf("m does not have a square root modulo n\n");
        exit(-1);
    }
    solve_n(s,m,n,p,q);
    printf("sign over\n");
    gmp_printf("s = %Zd\n\n",s);
    mpz_clear(m);
}

int checking(mpz_t R[],long long int rLen,mpz_t s,mpz_t n){
    mpz_t m;
    mpz_init(m);
    mpz_mul_ui(m,s,2);
    mpz_mod(m,m,n);
    int flag = 0;
    for(long long int i = 0;i<rLen;i++){
        if(mpz_cmp(m,R[i]) == 0){
            flag = 1;
            break;
        }
    }
    mpz_clear(m);
    return flag;
}

void solve_n(mpz_t x,mpz_t a,mpz_t n,mpz_t p,mpz_t q){
    mpz_t r,s,c,d,tmp;
    mpz_inits(r,s,c,d,tmp,NULL);
    solve_pq(r, a, p);
    solve_pq(s, a, q);
    gcd_ex(c, d, p, q);
    mpz_mul(x,r,d);
    mpz_mul(x,x,q);
    mpz_mul(tmp,s,c);
    mpz_mul(tmp,tmp,p);
    mpz_add(x,x,tmp);
    mpz_mod(x,x,n);
    mpz_clears(r,s,c,d,tmp,NULL);
}

void solve_pq(mpz_t x,mpz_t a,mpz_t p){
    mpz_t b,s,t,c,r,d,tmp,p_1,i;
    mpz_inits(b,s,t,c,r,d,tmp,p_1,NULL);
    mpz_init_set_ui(i,1);
    gmp_randstate_t state;
    init_state(state);
    while(mpz_legendre(b,p)!=-1)
        mpz_urandomm(b,state,p);
    mpz_sub_ui(p_1,p,1);
    unsigned long index = mpz_scan1(p_1,0);
    mpz_set_ui(s,index); // p-1 = 2**s * t
    mpz_tdiv_q_2exp(t,p_1,index); // t = (p-1)/2**s
    fast_pow_mod(c, b, t, p); // c = b**t mod p
    mpz_add_ui(tmp,t,1); 
    mpz_tdiv_q_ui(tmp,tmp,2); // tmp = (t+1)/2
    fast_pow_mod(r, a, tmp, p);
    invert(a, a, p); // a= a'
    for(;mpz_cmp(i,s)<0;mpz_add_ui(i,i,1)){
        mpz_mul(d,r,r);
        mpz_mod(d,d,p);
        mpz_mul(d,d,a);
        mpz_mod(d,d,p); // d = r*r * a' mod p
        if(mpz_cmp(d,p_1) == 0){
            mpz_mul(r,r,c);
            mpz_mod(r,r,p);
        }
        mpz_mul(c,c,c);
        mpz_mod(c,c,p);
    }
    mpz_set(x,r);
    mpz_clears(i,b,s,t,c,r,d,tmp,p_1,NULL);
}



/*
void solve_pq(mpz_t x,mpz_t n,mpz_t p){
    mpz_t a,w,tmp;
    mpz_inits(a,w,tmp,NULL);
    gmp_randstate_t state;
    init_state(state);
    while(1){
        mpz_urandomm(a,state,p);
        mpz_mul(w,a,a);
        mpz_sub(w,n,w);// w = n-a*a
        if(mpz_legendre(w,p) == -1) {break;}
    }
    f_2 res;
    mpz_init_set(res.x,a);
    mpz_init_set_ui(res.y,1); // a+sqrt(w)
    mpz_add_ui(tmp,p,1);
    mpz_tdiv_q_ui(tmp,tmp,2);
    // printf("3\n");
    f_2_pow(x, res, tmp, w,p);
    // printf("4\n");
    mpz_clears(a,w,tmp,res.x,res.y,NULL);
}


void f_2_pow(mpz_t res,f_2 a,mpz_t b,mpz_t w,mpz_t p){
	f_2 base;
    mpz_t tmp;
    mpz_init(tmp);
    mpz_init_set_ui(base.x,1);
    mpz_init_set_ui(base.y,0);
	while(mpz_cmp_ui(b, 0)>0){
        mpz_mod_ui(tmp,b,2);
        // gmp_printf("b = %Zd\n",b);
		if(mpz_cmp_ui(tmp,1) == 0) f_2_mul(base,base,a,w,p); // base = base *a
        // gmp_printf("a.x = %Zd\n",a.x);
		f_2_mul(a,a,a,w,p);mpz_tdiv_q_ui(b,b,2);
        // printf("go\n");
	}
    mpz_mod(res,base.x,p);
    mpz_clears(tmp,base.x,base.y,NULL);
}

void f_2_mul(f_2 res,f_2 a,f_2 b,mpz_t w,mpz_t p){
    mpz_t x1,x2,y1,y2,tmp,ans1,ans2;
    mpz_inits(tmp,x1,x2,y1,y2,ans1,ans2,NULL);
    mpz_mod(x1,a.x,p);
    mpz_mod(x2,b.x,p);
    mpz_mod(y1,a.y,p);
    mpz_mod(y2,a.y,p);
    // printf("mul1\n");

    mpz_mul(ans1,x1,x2);
    // printf("mul1.1\n");
    mpz_mod(ans1,ans1,p);
    // printf("mul1.2\n");
    mpz_mul(tmp,y1,y2);
    // printf("mul1.3\n");
    mpz_mod(tmp,tmp,p);
    // printf("mul1.4\n");
    mpz_mul(tmp,tmp,w);
    // printf("mul1.5\n");
    mpz_mod(tmp,tmp,p);
    // printf("mul1.6\n");
    mpz_add(ans1,ans1,tmp); // res.x = x1*x2 + y1*y2*w
    // printf("mul1.7\n");
    mpz_mod(ans1,ans1,p);
    // printf("mul2\n");

    mpz_mul(ans2,x1,y2);
    // printf("mul2.1\n");
    mpz_mod(ans2,ans2,p);
    // printf("mul2.2\n");
    mpz_mul(tmp,x2,y1);
    // printf("mul2.3\n");
    mpz_mod(tmp,tmp,p);
    // printf("mul2.4\n");
    mpz_add(ans2,ans2,tmp); // res.y = x1*y2 + x2*y1
    // printf("mul2.5\n");
    mpz_mod(ans2,ans2,p);
    // printf("mul3\n");

    mpz_set(res.x,ans1);mpz_set(res.y,ans2);
    // printf("mul4\n");
    mpz_clears(x1,x2,y1,y2,tmp,ans1,ans2,NULL);
    // printf("mul end\n");
}*/
