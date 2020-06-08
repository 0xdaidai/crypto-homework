/*************************************************************************
 > File Name: rc4.cpp
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Wed 29 Apr 2020 03:53:05 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int len(unsigned char *x){
    int l=0;
    while(x[l]!='\0')
        l++;
    return l;
}

unsigned char *s_init(unsigned char *key){
    unsigned char *s = NULL;
    s = (unsigned char*)malloc(sizeof(unsigned char) * 260); 
    memset(s,0,260);
    unsigned char *t = NULL;
    t = (unsigned char*)malloc(sizeof(unsigned char) * 260); 
    memset(t,0,260);

    int k_l = len(key);
    for(int i=0;i<256;i++){
        s[i] = i;
        t[i] = key[i%k_l];
    }

    int j=0, tmp;
    for(int i=0;i<256;i++){
        j = (j+s[i]+t[i])%256;
        tmp = s[i];
        s[i]=s[j];
        s[j] = tmp;
    }
    return s;
}

unsigned char *k_set(unsigned char *s,int len,int *a,int *b){
    int index=0, tmp;
    int i = *a,j=*b;
    unsigned char *k = NULL;
    k = (unsigned char*)malloc(sizeof(unsigned char) * len); 
    memset(k,0,len);
    while(index<len){
        i=(i+1)%256;
        j=(j+s[i])%256;
        tmp = s[i];
        s[i]=s[j];
        s[j] = tmp;
        k[index++] = s[(s[i]+s[j])%256];
    }
    *a=i;
    *b=j;
    return k;
}

unsigned char *rc4_encode(unsigned char *p,unsigned char *k,int *a,int *b,int p_l){
    unsigned char *key = k_set(s_init(k),p_l,a,b);
    unsigned char *c = NULL;
    c = (unsigned char*)malloc(sizeof(unsigned char) * p_l); 
    memset(c,0,p_l);
    
    for(int i = 0;i<p_l;i++){
        c[i] = p[i]^key[i];
    }
    return c;
}

unsigned char *rc4_decode(unsigned char *p,unsigned char *k,int *a,int *b,int rc){
    return rc4_encode(p,k,a,b,rc);
}

int main(){
    // FILE *infile =fopen("test.txt","rb");
    // FILE *outfile = fopen("out","wb");
    // FILE *infile =fopen("out","rb");
    // FILE *outfile = fopen("out1","wb");
    // unsigned char *s;
    // s = (unsigned char*)malloc(sizeof(unsigned char) * 260);
    // memset(s,0,260);
    unsigned char s[] = {0x01,0x23,0x45,0x67,0x89,0xfe,0xdc,0xba,0xab,0xcd,0xef,0x98,0x76,0x54,0x32,0x10};
    unsigned char k[] = {0x01,0x23,0x45,0x67,0x89,0xfe,0xdc,0xba,0xab,0xcd,0xef,0x98,0x76,0x54,0x32,0x10};
    int len = 16;
    int rc=0;
    int a =0,b=0;
    unsigned char *res = rc4_encode(s, k, &a, &b, len);
    for (int i =0;i<256;i++){
        printf("%x ", res[i]);
    }
    putchar('\n');
    // while( (rc = fread(s,sizeof(unsigned char), 256,infile)) != 0 ){
        // printf("rc = %d\n",rc);
        // fwrite(rc4_encode(s,k,&a,&b,rc),sizeof(unsigned char),rc,outfile);
        // printf("a = %d, b= %d\n",a,b);
        // memset(s,0,260);
        // fwrite(s,sizeof(unsigned char),rc,outfile);
    // }
    return 0;
}
