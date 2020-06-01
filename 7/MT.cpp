/*************************************************************************
 > File Name: MT.cpp
 > Author: Guaji
 > Mail: rdwentao@163.com 
 > Created Time: Sun 03 May 2020 07:27:25 PM CST
 ************************************************************************/

#include <stdio.h>
#define N 624
#define M 397
#define R 31
#define A 0x9908B0DF
#define F 1812433253
#define U 11
#define S 7
#define B 0x9D2C5680
#define T 15
#define C 0xEFC60000
#define L 18
#define MASK_LOWER 0x7fffffff
#define MASK_UPPER !MASK_LOWER

static unsigned int mt[N];
static unsigned int index;

void Initialize(const unsigned int seed)
{
    index = N;
    mt[0] = seed;
    for ( int i = 1; i < N; i++ )
    {
        mt[i] = (F * (mt[i - 1] ^ (mt[i - 1] >> 30)) + i);
    }
}


void Twist()
{
    unsigned int x, xA;
    for ( int i = 0; i < N; i++ )
    {
        x = (mt[i] & MASK_UPPER) + (mt[(i + 1) % N] & MASK_LOWER);
        xA = x >> 1;
        if ( x & 0x1 )
        {
            xA ^= A;
        }
        mt[i] = mt[(i + M) % N] ^ xA;
    }

    index = 0;
}

unsigned int MT()
{
    unsigned int y;
    int i = index;
    if ( index >= N )
    {
        Twist();
        i = index;
    }
    y = mt[i];
    index = i + 1;
    y ^= (y >> U);
    y ^= (y << S) & B;
    y ^= (y << T) & C;
    y ^= (y >> L);
    return y;
}

int main(){
    Initialize(128);
    printf("%u\n",MT());
    printf("%u\n",MT());
    printf("%u\n",MT());

    return 0;
}
