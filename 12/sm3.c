/*************************************************************************
 > File Name: 12/sm3.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Sun 07 Jun 2020 02:29:33 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "tools.h"
#define ROTLEFT(a, b) ((a << b) | (a >> (32 - b)))

unsigned char *sm3(unsigned char *M,long long int mLen,unsigned char *IV);
void *CF(unsigned char *V,unsigned char *BB);
void wext(unsigned int *w,unsigned char *BB);
unsigned int FF(unsigned int x,unsigned int y,unsigned int z,int j);
unsigned int GG(unsigned int x,unsigned int y,unsigned int z,int j);
unsigned int p0(unsigned int x);
unsigned int p1(unsigned int x);
unsigned char *padding(unsigned char *M,long long int *mLen);

static unsigned int A,B,C,D,E,F,G,H;
static unsigned int w[132] ;

int main(){
    unsigned char M[] = "abc";
    long long int mLen = strlen(M);
    unsigned char IV[] = {115,128,22,111,73,20,178,185,23,36,66,215,218,138,6,0,169,111,48,188,22,49,56,170,227,141,238,77,176,251,14,78};
    unsigned char *res = sm3(M, mLen, IV);
    for(int i = 0;i<32;i++)
        printf("%x ", res[i]);
    printf("\n");
    return 0;
}

unsigned char *sm3(unsigned char *Msg,long long int mLen,unsigned char *IV){
    unsigned char *M = padding(Msg, &mLen);
    printf("mLen = %d\n",mLen );
    unsigned char *V = (unsigned char*)malloc(32);
    memcpy(V,IV,32);
    unsigned char *BB = (unsigned char*)malloc(64);
    long long int n =  (mLen)*8/512;
    printf("n = %d\n", n);
    for(int i =0;i< n;i++){
        memcpy(BB,M+i*64,64);
        CF(V,BB);
    }
    return V;
}

void *CF(unsigned char *V,unsigned char *BB){
    unsigned int SS1,SS2,TT1,TT2,T;
    unsigned int VV[8];
    wext(w, BB);
    // memcpy(VV,V,32);
    for(int i = 0; i< 8; i++){
        VV[i] = (V[i*4]<<24)+(V[i*4+1]<<16)+(V[i*4+2]<<8)+V[i*4+3];
    }
    A = VV[0];B = VV[1];C = VV[2];D = VV[3];
    E = VV[4];F = VV[5];G = VV[6];H = VV[7];

    for(int j = 0; j< 64; j++){
        T = (j>15)?0x7a879d8a:0x79cc4519;
        SS1 = ROTLEFT((ROTLEFT(A,12) + E + ROTLEFT(T,(j%32))),7);
        SS2 = ROTLEFT(A,12) ^ SS1;
        TT1 = FF(A, B, C, j) + D + SS2 + w[j+68];
        TT2 = GG(E, F, G, j) + H + SS1 + w[j];
        D = C;
        C = ROTLEFT(B, 9);
        B = A;
        A = TT1;
        H = G;
        G = ROTLEFT(F, 19);
        F = E;
        E = p0(TT2);
    }

    VV[0] ^= A; VV[1] ^= B; VV[2] ^= C; VV[3] ^= D;
    VV[4] ^= E; VV[5] ^= F; VV[6] ^= G; VV[7] ^= H; 

    // memcpy(V,VV,32);
    for(int i = 0; i< 8; i++){
        V[4*i] = (VV[i]>>24);
        V[4*i+1] = (VV[i]>>16)&0xff;
        V[4*i+2] = (VV[i]>>8)&0xff;
        V[4*i+3] = VV[i]&0xff;
    }
}

/* unsigned int w[132] */
void wext(unsigned int *w,unsigned char *BB){
    // memcpy(w,BB,64);
    for(int i = 0; i< 16; i++){
        w[i] = (BB[i*4]<<24)+(BB[i*4+1]<<16)+(BB[i*4+2]<<8)+BB[i*4+3];
    }
    int j =16;
    for(; j<68; j++)
        w[j] = p1(w[j-16]^w[j-9]^ROTLEFT(w[j-3],15))^ROTLEFT(w[j-13],7)^w[j-6];
    for(; j<132; j++)
        w[j] = w[j-68]^w[j-64];
}

unsigned char *padding(unsigned char *M,long long int *mLen){
    long long int paddLen;
    if(*mLen%64>=56)
        paddLen = 56 + 64 - *mLen % 64;
    else
        paddLen = 56 - *mLen %64;
    
    unsigned char *padd = (unsigned char*)malloc(sizeof(unsigned char) * (paddLen+8));
    memset(padd,0,paddLen+8);
    padd[0] = 0b10000000;

    *mLen *=8;
    padd[paddLen] = *mLen >> 56;
    padd[paddLen+1] = (*mLen >> 48)&0xff;
    padd[paddLen+2] = (*mLen >> 40)&0xff;
    padd[paddLen+3] = (*mLen >> 32)&0xff;
    padd[paddLen+4] = (*mLen >> 24)&0xff;
    padd[paddLen+5] = (*mLen >> 16)&0xff;
    padd[paddLen+6] = (*mLen >> 8)&0xff;
    padd[paddLen+7] = (*mLen )&0xff;

    *mLen /= 8;
    unsigned char *res = array_append(M, *mLen, padd, paddLen+8);
    *mLen += paddLen+8;
    free(padd);
    return res;
}

unsigned int FF(unsigned int x,unsigned int y,unsigned int z,int j){
    if(j<=15)
        return x^y^z;
    return (x&y)|(x&z)|(y&z);
}

unsigned int GG(unsigned int x,unsigned int y,unsigned int z,int j){
    if(j<=15)
        return x^y^z;
    return (x&y)|((~x)&z);
}

unsigned int p0(unsigned int x){
    return x^ROTLEFT(x, 9)^ROTLEFT(x,17);
}

unsigned int p1(unsigned int x){
    return x^ROTLEFT(x, 15)^ROTLEFT(x,23);
}
