#!/usr/bin/env python
# -*- coding: utf-8 -*-
from arithmetic import *

def reduci(n=8): 
    lhs=(1<<n)+1
    rhs=1<<(n+1)
    index=1<<(n//2+1)
    reducilist=[]
    for i in range(lhs,rhs,2): 
        flag=0
        for j in range(2,index,2): 
            if mod(i,j)==0: 
                flag=1
                break
        if flag==0: 
            reducilist.append(i)
    return reducilist

def prime(reducilist,n=8): 
    primelist=[]
    t=2**n-1
    for i in reducilist: 
        flag=0
        k=(1<<t)+1
        if mod(k,i)==0: 
            flag=1
            for j in range(t):
                k=(1<<j)+1
                if mod(k,i)==0:
                    flag=0
                    break
        if flag==1: 
            primelist.append(i)
    return primelist

