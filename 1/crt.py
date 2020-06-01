#!/usr/bin/env python
# -*- coding: utf-8 -*-
from ojld import *

def crt(a,m,k): 
    M=1
    e=[]
    for i in m: 
        M=M*i 
    for i in range(0,k): 
        e.append(gcd_plus(M//m[i],m[i]))

    sum=0
    for i in range(0,k): 
        sum+=M//m[i]*e[i]*a[i]

    return sum%M
