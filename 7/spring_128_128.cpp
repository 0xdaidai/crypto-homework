/*************************************************************************
 > File Name: spring.cpp
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Mon 04 May 2020 01:14:44 AM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#define NR 10

static unsigned char **s ;

const unsigned char counter[] = {0b11000000,0b00100100,0b00111011,0b01000001,0b01101101
                                ,0b01001101,0b11000011,0b10110111,0b11010111,0b01000101};
const int k_f[][4] = {
    {1,3,8,9},{1,4,8,9},{1,5,8,9},{1,6,8,9},
    {15,10,7,8},{15,11,7,8},{15,12,7,8},{15,13,7,8}
};
const int f_s[][4] = {
    {0,6,3,4},{0,5,3,4},{0,3,4,5},{0,2,4,5}
};

void pr_s(){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            printf("%d ",  s[i][j] );
        }
        printf("\n");
    }
}

void init_state(){
    s = (unsigned char **)malloc(sizeof(unsigned char*)*4);
    for(int i = 0; i< 4; i++){
        s[i] = (unsigned char*)malloc(sizeof(unsigned char)*4);
    }
}

unsigned char reverse_bit(unsigned char value)
{
    unsigned char ret = 0;
    int i = 0;
    for(i=0;i<8;i++)
    {
        int j = value & 1;//依次得到最低位
        ret |=(j<<(8-i-1));
        value >>= 1;
    }
    return ret;
}

unsigned int f(unsigned char x,const int *a){
    return (((x>>a[0])&1)^((x>>a[1])&1)^(((x>>a[2])&1)&((x>>a[3])&1)));
}

unsigned char ff(unsigned char x,unsigned char y,const int *a){
    unsigned int tmp = (unsigned int)x;
    tmp = (tmp<<8)+(unsigned int)y;
    return (unsigned char)(((tmp>>a[0])&1)^((tmp>>a[1])&1)^(((tmp>>a[2])&1)&((tmp>>a[3])&1)));
}

void NFSR_Key(unsigned char *IS){
    unsigned char tmp ,tmp1;
    for(int i=0; i<16 ;i++){
        tmp = IS[15]&1;
        for(int j=0; j<8; j++){
            tmp1 = IS[j*2]&1;
            IS[j*2] = (IS[j*2]>>1)|((tmp^ff(IS[j*2],IS[j*2+1],k_f[j]))<<7);

            tmp = IS[j*2+1]&1;
            IS[j*2+1] = (IS[j*2+1]>>1)|(tmp1<<7);
        } 
    }
}

unsigned char **key_exp(unsigned char *key){
    unsigned char *IS = (unsigned char*)malloc(sizeof(unsigned char) * 16);
    unsigned char **k = (unsigned char**)malloc(sizeof(unsigned char*)* (NR+1));
    for(int i = 0; i< NR+1; ++i){
        k[i] = (unsigned char*)malloc(sizeof(unsigned char)*16);
    }
    for(int i = 0; i< 16; ++i){
        IS[i] = key[i];
        k[0][i] = IS[i];
    }
    for(int i = 1; i<=NR ;++i){
        IS[0] = IS[0]^counter[i-1];
        NFSR_Key(IS);
        for(int j=0; j<16; j++){
            k[i][j] = IS[j];
        }
    }
    return k;
}

void Add(unsigned char *k){
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            s[i][j] ^= k[i*4+j];
        }
    }
}

void Subrow(int x){
    unsigned char tmp_l[4];
    int count = 32;
    while(count--){
        for(int i=0; i<4; i++){
            tmp_l[i] = s[x][i]&1;
        }

        // refresh
        s[x][0] = (s[x][0]>>1)|((f(s[x][0],f_s[0])^tmp_l[2]^tmp_l[3])<<7);
        s[x][1] = (s[x][1]>>1)|((f(s[x][1],f_s[1])^tmp_l[0]^tmp_l[3])<<7);
        s[x][2] = (s[x][2]>>1)|((f(s[x][2],f_s[2])^tmp_l[0]^tmp_l[1])<<7);
        s[x][3] = (s[x][3]>>1)|((f(s[x][3],f_s[3])^tmp_l[2]^tmp_l[1])<<7);
    }
}

void Subrow_inv(int x){
    unsigned char tmp_l[4];
    int count = 32;
    while(count--){
        for(int i=0; i<4; i++){
            tmp_l[i] = f(s[x][i],f_s[i]);
        }

        // refresh
        s[x][0] = (s[x][0]>>1)|((tmp_l[0]^tmp_l[2]^tmp_l[3])<<7);
        s[x][1] = (s[x][1]>>1)|((tmp_l[0]^tmp_l[1]^tmp_l[3])<<7);
        s[x][2] = (s[x][2]>>1)|((tmp_l[0]^tmp_l[1]^tmp_l[2])<<7);
        s[x][3] = (s[x][3]>>1)|((tmp_l[2]^tmp_l[1]^tmp_l[3])<<7);
    }
}

void Transpose(unsigned char **x){
    int i,j;
    unsigned char tmp;
    for(i = 1;i < 4;i++){
        for(j = 0; j < i; j++){
            tmp = x[i][j];
            x[i][j] = x[j][i];
            x[j][i] = tmp;
        }
    }
}

void Round(unsigned char *k, int flag){
    Add(k);
    if(flag)
        for(int i = 0; i < 4; i++){
            Subrow(i);
        }
    else
        for(int i = 0; i < 4; i++){
            Subrow_inv(i);
        }
    Transpose(s);
}

void BitPermutation(unsigned char **x){
    unsigned char tmp1,tmp2;
    for(int i=1; i<4; i++){
        for(int j=0; j<i; j++){
            tmp1 = x[i][j];
            tmp2 = x[3-i][3-j];
            x[i][j] = reverse_bit(tmp2);
            x[3-i][3-j] = reverse_bit(tmp1);
        }
    }
    for(int i=0; i<2; i++){
        tmp1 = x[i][i];
        tmp2 = x[3-i][3-i];
        x[i][i] = reverse_bit(tmp2);
        x[3-i][3-i] = reverse_bit(tmp1);
    }
}

void F_Round(unsigned char *k, unsigned char *kk, int flag){
    Add(k);
    if(flag)
        for(int i = 0; i < 4; i++){
            Subrow(i);
        }
    else
        for(int i = 0; i < 4; i++){
            Subrow_inv(i);
        }
    Add(kk);
    BitPermutation(s);
}

void key_exp_inv(unsigned char **k){
    unsigned char **mat;
    mat = (unsigned char **)malloc(sizeof(unsigned char*)*4);
    for(int i = 0; i< 4; i++){
        mat[i] = (unsigned char*)malloc(sizeof(unsigned char)*4);
    }

    for(int i = 1; i<NR; i++){
        for(int j = 0; j<4; j++){
            for(int t = 0; t< 4; t++){
                mat[j][t] = k[i][j*4+t];
            }
        }

        BitPermutation(mat);
        Transpose(mat);

        for(int j = 0; j<4; j++){
            for(int t = 0; t< 4; t++){
                k[i][j*4+t] = mat[j][t];
            }
        }
    }


    //0 and NR
    for(int j = 0; j<4; j++){
        for(int t = 0; t< 4; t++){
            mat[j][t] = k[0][j*4+t];
        }
    }
    BitPermutation( mat);
    for(int j = 0; j<4; j++){
        for(int t = 0; t< 4; t++){
            k[0][j*4+t] = mat[j][t];
        }
    }

    for(int j = 0; j<4; j++){
        for(int t = 0; t< 4; t++){
            mat[j][t] = k[NR][j*4+t];
        }
    }
    BitPermutation( mat);
    for(int j = 0; j<4; j++){
        for(int t = 0; t< 4; t++){
            k[NR][j*4+t] = mat[j][t];
        }
    }
// permu key
    unsigned char *tmp = NULL;
    for(int i = 0; i< NR/2; i++){
        tmp = k[i];
        k[i] = k[NR-i];
        k[NR-i] = tmp;
    }
}

unsigned char *encode(unsigned char *p, unsigned char *key){
    if(s == NULL)
        init_state();
    // init
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            s[i][j] = p[i*4+j];
        }
    }
    unsigned char **k = key_exp(key);

    for(int nr = 0; nr < NR -1; nr++){
        Round(k[nr],1); 
    }

    F_Round(k[NR - 1],k[NR],1);

    unsigned char *c = (unsigned char*)malloc(sizeof(unsigned char)*16);
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            c[i*4+j] = s[i][j];
        }
    }
    return c;
}

unsigned char *decode(unsigned char *c, unsigned char *key){
    if(s == NULL)
        init_state();
    // init
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            s[i][j] = c[i*4+j];
        }
    }
    unsigned char **k = key_exp(key);
    key_exp_inv(k);

    for(int nr = 0; nr < NR -1; nr++){
        Round(k[nr],0); 
    }

    F_Round(k[NR - 1],k[NR],0);

    unsigned char *p = (unsigned char*)malloc(sizeof(unsigned char)*16);
    for(int i=0; i<4; i++){
        for(int j=0; j<4; j++){
            p[i*4+j] = s[i][j];
        }
    }
    return p;
}

int main(){
    unsigned char p[] = {0xe1,0x0c,0xe7,0x2e,0x06,0xea,0xa7,0x25,0xb8,0x4b,0x5f,0xa6,0x7a,0x73,0xdc,0x9b};
    unsigned char k[] = {0xef,0x06,0x8d,0x86,0x1c,0x48,0xaa,0xc2,0xec,0x8a,0x0c,0x63,0xfb,0xc9,0x4d,0xe6};
    unsigned char c[] = {0xa1,0x43 ,0x83 ,0xe7 ,0x40 ,0x34 ,0x8d, 0xba ,0x5e ,0xd0 ,0x42 ,0x2f ,0x7f ,0x25 ,0x6b ,0xfd};
    unsigned char *cc = encode(p, k);
    unsigned char *pp = decode(c,k);
    for(int i = 0; i< 16; i++){
        printf("%x ", cc[i]);
    }
    putchar('\n');

    for(int i = 0; i< 16; i++){
        printf("%x ", pp[i]);
    }
    putchar('\n');

    return 0;
}
