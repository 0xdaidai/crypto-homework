/*************************************************************************
 > File Name: last/main.c
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Mon 29 Jun 2020 01:46:17 AM CST
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#include <cjson/cJSON.h>
#include <sys/stat.h>
#include <unistd.h>
#include "aes.h"
#include "bloom.h"
#include "murmurhash2.h"
#include "file.h"
#define BUFFER_SIZE 1024

const unsigned char iv[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
const unsigned char K[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};

char *file_EN(char name[],BYTE K[],unsigned char IV[]){
    char name_enc[1024] = {0};
    memcpy(name_enc,name,strlen(name));
    strcpy(name_enc,".enc");
    FILE *fp = fopen(name,"rb");
    FILE *out = fopen(name_enc,"wb");
    char buf[BUFFER_SIZE]; 
    char res[BUFFER_SIZE]; 
    int len_file, len_block;
    int totalBlock;
    unsigned int key_schedule[60];
	aes_key_setup(K, key_schedule, 256);
    fseek(fp,0,2);//将指针放到文件末尾
    len_file = ftell(fp); //统计文本的数据长度
    rewind(fp); //让指针指向文件开始位置
    printf("文件的总长度len_file:%d\n",len_file);
    totalBlock = len_file % BUFFER_SIZE == 0 ? len_file / BUFFER_SIZE : ((len_file / BUFFER_SIZE) + 1); //除最后一快的总块数
    printf("文件的总块数totalBlock:%d\n",totalBlock);
    for(int i = 1; i <= totalBlock; i++) {
        memset(buf,0,BUFFER_SIZE);
        memset(res,0,BUFFER_SIZE);
        len_block = fread(buf, 1, BUFFER_SIZE, fp);
        buf[len_block]='\0';
        aes_encrypt_cbc(buf,BUFFER_SIZE,res,key_schedule,256,IV);
        fwrite(buf,BUFFER_SIZE,1,out);
    }
    printf("file: %s is ready\n",name_enc );
    return name_enc;
}

char *make_BF(char name[],BYTE K[],unsigned char IV[]){
    char name_bf[1024] = {0};
    memcpy(name_bf,name,strlen(name));
    strcpy(name_bf,".bf");
    FILE *fp = fopen(name,"rb");
    FILE *out = fopen(name_bf,"wb");
    char buf[1024*1204]={0};
    char res[80];
    int len_file, len_block;
    int totalBlock;
    unsigned int key_schedule[60];
    const char s[2] = " ";
    char *token;
	aes_key_setup(K, key_schedule, 256);
    fseek(fp,0,2);//将指针放到文件末尾
    len_file = ftell(fp); //统计文本的数据长度
    rewind(fp); //让指针指向文件开始位置
    printf("文件的总长度len_file:%d\n",len_file);
    fread(buf,1,len_file,fp);
    int i = 0;
    while(buf[i]!='\0'){
        if((buf[i]>='a'&&buf[i]<='z')||(buf[i]>='A'&&buf[i]<='Z'))  
            continue;
        else
            buf[i] = ' ';
    }
    token = strtok(buf, s);
    while( token != NULL ) {
        memset(res,0,80);
        aes_encrypt_cbc(token,strlen(token),res,key_schedule,256,IV);
        //add in bf
        token = strtok(NULL, s);
    }
}
