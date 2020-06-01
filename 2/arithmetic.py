#!/usr/bin/env python
# -*- coding: utf-8 -*-

def plus(a,b,m=0b100011011): 
    a=plus(~a,1) if(a<0) else a 
    b=plus(~b,1) if(b<0) else b 
    return mod(a^b,m)

def sub(a,b,m=0b100011011): 
    return plus(a,b,m)

def mul(a,b,m=0b100011011): 
    a=plus(~a,1) if(a<0) else a 
    b=plus(~b,1) if(b<0) else b 
    res=0
    while b!=0: 
        if (b&1)!=0: 
            res = plus(res,a)
        a=a<<1
        b=b>>1
    return mod(res,m)

def div(a,b,m=0b100011011): 
    a=plus(~a,1) if(a<0) else a 
    b=plus(~b,1) if(b<0) else b 
    res=0
    x = a.bit_length()-b.bit_length()

    while x>=0: 
        temp = b<<x
        res+=(1<<x)
        a=sub(a,temp)
        x=a.bit_length()-b.bit_length()
    return mod(res,m) 

def mod(a,b): 
    a=plus(~a,1) if(a<0) else a 
    b=plus(~b,1) if(b<0) else b 
    res=0
    x = a.bit_length()-b.bit_length()
    while x>=0: 
        temp = b<<x
        a=a^temp
        x=a.bit_length()-b.bit_length()
    return a
