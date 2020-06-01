/*************************************************************************
 > File Name: test.cpp
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Sun 03 May 2020 02:05:00 PM CST
 ************************************************************************/

#include <stdio.h>
#include <stdlib.h>

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

void pr( int **a){
    for(int i = 0; i< 4; i++){
        for(int j = 0; j< 4; j++){
            printf("%d ", a[i][j] );
        }
        printf("\n");
    }
    printf("\n");
}

void f(int **a){
    int *tmp;
    tmp = a[0];
    a[0] = a[3];
    a[3] = tmp;
}

int main(){
    int **a = (int **)malloc(sizeof(int *)*4);
    for(int i = 0; i< 4; i++){
        a[i] = (int*)malloc(sizeof(int)*4);
    }

    for(int i = 0; i< 4; i++){
        for(int j = 0; j< 4; j++){
            a[i][j] = i+j;
        }
    }
    pr(a);
    f(a);
    pr(a);


    return 0;
}
