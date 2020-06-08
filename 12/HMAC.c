/*************************************************************************
 > File Name: 12/HMAC.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Sat 06 Jun 2020 06:40:02 PM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include "sha1.h"
#include "tools.h"

void HMAC(unsigned char *hash,unsigned char *M,long long int mLen,unsigned char *K,long long int kLen,int b);

int main(){
    unsigned char M[] = "Sample message for keylen=blocklen";
    long long int mLen = strlen(M);
    unsigned char K[64] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63};

    int b = 512;

    unsigned char hash[SHA1_BLOCK_SIZE];
    memset(hash,0,SHA1_BLOCK_SIZE);
    HMAC(hash, M, mLen, K, 64, b);

    for(int i = 0;i<SHA1_BLOCK_SIZE;i++){
        printf("%x ",hash[i] );
    }
    printf("\n\n");
}

void HMAC(unsigned char *hash,unsigned char *M,long long int mLen,unsigned char *K,long long int kLen,int b){
    unsigned char *K_;
    int bLen = b/8;
    if(kLen<b/8){
        unsigned char *zero = (unsigned char*)malloc(sizeof(unsigned char )*(bLen-kLen));
        memset(zero,0,bLen - kLen);
        K_ = array_append(zero, bLen-kLen, K, kLen);
        free(zero);
        printf("paddKey\n" );
    }
    else{
        K_ = (unsigned char*)malloc(sizeof(unsigned char )*(kLen));
        memcpy(K_,K,kLen);
    }
    unsigned char *S = (unsigned char*)malloc(sizeof(unsigned char) * bLen);
    for(int i =0; i< bLen; i++)
        S[i] = K_[i]^0x36;
    unsigned char *tmp = array_append(S, bLen, M, mLen);

    SHA1_CTX ctx;
    memset(hash,0,SHA1_BLOCK_SIZE);
    sha1_init(&ctx);
    sha1_update(&ctx, tmp, bLen+mLen);
    sha1_final(&ctx, hash);

    for(int i =0;i<20;i++)
        printf("%x ",hash[i]);
    putchar('\n');

    unsigned char *S0 = (unsigned char*)malloc(sizeof(unsigned char) * bLen);
    for(int i =0; i< bLen; i++)
        S0[i] = K_[i]^0x5c;

    free(tmp);
    tmp = array_append( S0, bLen,hash,SHA1_BLOCK_SIZE);

    memset(hash,0,SHA1_BLOCK_SIZE);
    sha1_init(&ctx);
    sha1_update(&ctx, tmp, bLen+ SHA1_BLOCK_SIZE);
    sha1_final(&ctx, hash);
}
