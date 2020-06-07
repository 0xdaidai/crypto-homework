/*************************************************************************
 > File Name: 12/birth_1.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Sun 07 Jun 2020 01:58:13 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include <time.h>
#include "sha1.h"

char* genRandomString(int length);
char *col(unsigned char hash[],int l);

int main(){
    unsigned char M[] = "sadadsdsa"; 
    long long int mLen = strlen(M);
    unsigned char hash[SHA1_BLOCK_SIZE] = {0};
    SHA1_CTX ctx;
    sha1_init(&ctx);
    sha1_update(&ctx, M, mLen);
    sha1_final(&ctx, hash);
    char *res = col(hash, 10);
    printf("res = %s\n", res);

    return 0;
}

char *col(unsigned char hash[],int l){
    char *str;
    SHA1_CTX ctx;
    unsigned char buf[SHA1_BLOCK_SIZE];
    while(1){
        str = genRandomString(l);
        printf("%s\n", str);
        memset(buf,0,SHA1_BLOCK_SIZE);
        sha1_init(&ctx);
        sha1_update(&ctx, (unsigned char*)str, l);
        sha1_final(&ctx, buf);
        if(buf[0]==hash[0]&&buf[1]==hash[1]){
            break;
        }
        free(str);
    }
    return str;
}

char* genRandomString(int length){
	int flag, i;
	char* string;
    unsigned long long cur = ((time(0) % 1000) + 10000) * ((clock() % 1000) + 10000);
	srand(cur);
	if ((string = (char*) malloc(length)) == NULL )
	{
		printf("Malloc failed!flag:14\n");
		return NULL ;
	}
 
	for (i = 0; i < length - 1; i++)
	{
		flag = rand() % 3;
		switch (flag)
		{
			case 0:
				string[i] = 'A' + rand() % 26;
				break;
			case 1:
				string[i] = 'a' + rand() % 26;
				break;
			case 2:
				string[i] = '0' + rand() % 10;
				break;
			default:
				string[i] = 'x';
				break;
		}
	}
	string[length - 1] = '\0';
	return string;
}
