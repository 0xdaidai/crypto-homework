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
    unsigned char M[]="asdsadsagdsghshsgfhshgfhsh";
    long long int mLen = strlen(M);
    unsigned char K[SHA1_BLOCK_SIZE] = {21,22,23,24,25};

    SHA1_CTX ctx;
    sha1_init(&ctx);
    sha1_update(&ctx, K,SHA1_BLOCK_SIZE );
    memset(K,0,SHA1_BLOCK_SIZE);
    sha1_final(&ctx, K);

    int b = 512;

    unsigned char hash[SHA1_BLOCK_SIZE];
    memset(hash,0,SHA1_BLOCK_SIZE);
    HMAC(hash, M, mLen, K, SHA1_BLOCK_SIZE, b);


}

void HMAC(unsigned char *hash,unsigned char *M,long long int mLen,unsigned char *K,long long int kLen,int b){
    unsigned char *K_;
    int bLen = b/8;
    if(kLen<b/8){
        unsigned char *zero = (unsigned char*)malloc(sizeof(unsigned char )*(bLen-kLen));
        memset(zero,0,bLen - kLen);
        K_ = array_append(zero, bLen-kLen, K, kLen);
        free(zero);
    }
    else
        *K_ = K;
    unsigned char *S = (unsigned char*)malloc(sizeof(unsigned char) * bLen);
    for(int i =0; i< bLen; i++)
        S[i] = K_[i]^0x36;
    unsigned char *tmp = array_append(S, bLen, M, mLen);

    SHA1_CTX ctx;
    memset(hash,0,SHA1_BLOCK_SIZE);
    sha1_init(&ctx);
    sha1_update(&ctx, tmp, bLen+mLen);
    sha1_final(&ctx, hash);

    unsigned char *S0 = (unsigned char*)malloc(sizeof(unsigned char) * bLen);
    for(int i =0; i< bLen; i++)
        S[i] = K_[i]^0x5c;

    free(tmp);
    tmp = array_append(hash, SHA1_BLOCK_SIZE, S0, bLen);

    memset(hash,0,SHA1_BLOCK_SIZE);
    sha1_init(&ctx);
    sha1_update(&ctx, tmp, bLen+ SHA1_BLOCK_SIZE);
    sha1_final(&ctx, hash);
}
