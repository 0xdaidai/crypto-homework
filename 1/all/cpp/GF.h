#ifndef GF_H_INCLUDED
#define GF_H_INCLUDED

unsigned int GF_add(unsigned int a,unsigned int b);
unsigned int GF_mul(unsigned int a,unsigned int b);
unsigned int GF_div(unsigned int a,unsigned int b);
unsigned int GF_mod(unsigned int a,unsigned int b);
unsigned int GF_pow(unsigned int x,unsigned int n);
unsigned int GF_gcd(unsigned int a,unsigned int b);
unsigned int GF_exgcd(unsigned int &x,unsigned int &y,unsigned int a,unsigned int b);
unsigned int GF_inverse(unsigned int a);

#endif
