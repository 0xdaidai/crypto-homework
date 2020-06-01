#include <cstdio>
#include "GF.h"
 
unsigned int GF_add(unsigned int a,unsigned int b)
{
	return a^b;
}

unsigned int GF_mul(unsigned int a,unsigned int b)
{
	unsigned int ans=0;
	while(b>0)
	{
		//b�����һλ��1��ans+=a
		if(b&0x1)
			ans^=a;
		b>>=1;
		//x^7����ڣ�������ģ���㣬x^8(0x100)==x^4+x^3+x+1(0x1B) 
		if(a&0x80)
			a=(a<<1)^0x1B;
		//x^7����ڣ�ֱ������ 
		else
			a<<=1;
		//���8λ���ϵĶ���λ 
		a&=0xFF;
		
	}
	return ans;
}

unsigned int GF_div(unsigned int a,unsigned int b)
{
	unsigned int d=0;
	unsigned int ord_a,ord_b;
	if(b==0) {printf("Error!"); return 0;}
	//��a��b�Ľ�(���ܳ���8λ) 
	for(ord_a=8;a>>ord_a==0&&a!=0;ord_a--);
	for(ord_b=8;b>>ord_b==0&&a!=0;ord_b--);
	while(ord_a>=ord_b&&a!=0)
	{
		d^=1<<(ord_a-ord_b); //�̵���Ӧλ��1 
		a^=b<<(ord_a-ord_b); //aȡ���� 
		for(ord_a--;a>>ord_a==0&&a!=0;ord_a--); //����a�Ľ� 
	}
	return d;
}

unsigned int GF_mod(unsigned int a,unsigned int b)
{
	unsigned int d=0;
	unsigned int ord_a,ord_b;
	if(b==0) {printf("Error!"); return 0;}
	//��a��b�Ľ�(���ܳ���8λ)
	for(ord_a=8;a>>ord_a==0&&a!=0;ord_a--);
	for(ord_b=8;b>>ord_b==0&&a!=0;ord_b--);
	while(ord_a>=ord_b&&a!=0)
	{
		d^=1<<(ord_a-ord_b); //�̵���Ӧλ��1 
		a^=b<<(ord_a-ord_b); //aȡ���� 
		for(ord_a--;a>>ord_a==0&&a!=0;ord_a--); //����a�Ľ� 
	}
	return a;
}

unsigned int GF_pow(unsigned int x,unsigned int n)
{
	unsigned int d=1;
	while(n>0)
	{
		if(n&1)
			d=GF_mul(d,x);
		n>>=1;
		x=GF_mul(x,x);
	}
	return d;
}

unsigned int GF_gcd(unsigned int a,unsigned int b)
{
	if(b==0)
		return a;
	else
		return GF_gcd(b,GF_mod(a,b));
}

unsigned int GF_exgcd(unsigned int &x,unsigned int &y,unsigned int a,unsigned int b)
{
	unsigned int d,x1,y1;
	if(b==0)
	{
		x=1;
		y=0;
		return a;
	}
	else
	{
		d=GF_exgcd(x1,y1,b,GF_mod(a,b));
		x=y1;
		//��y=x1-[a/b]*y1
		y=x1^GF_mul(GF_div(a,b),y1);
		return d;
	}
}

unsigned int GF_inverse(unsigned int a)
{
	unsigned int e,y;
	GF_exgcd(e,y,a,0x11b);
	return e;
}
