#!/usr/bin/env python
# -*- coding: utf-8 -*-

def plus(a,b): 
    return a^b

def sub(a,b): 
    return plus(a,b)

def mul(a,b): 
    res=0
    while b!=0: 
        if (b&1)!=0: 
            res = plus(res,a)
        a=a<<1
        b=b>>1
    return res

def div(a,b): 
    res=0
    x = a.bit_length()-b.bit_length()

    while x>=0: 
        temp = b<<x
        res+=(1<<x)
        a=sub(a,temp)
        x=a.bit_length()-b.bit_length()
    return res 

def mod(a,b): 
    res=0
    x = a.bit_length()-b.bit_length()
    while x>=0: 
        temp = b<<x
        a=a^temp
        x=a.bit_length()-b.bit_length()
    return a
