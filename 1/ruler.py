#!/usr/bin/env python
# -*- coding: utf-8 -*-

def one_or_zero(x,b):
    return (x>>(b-1))&1

def eulerprime(n): 
    visit_bool=0
    primelist=[]

    for i in range(3,n+1,2): 
        if one_or_zero(visit_bool,i)==0: 
            primelist.append(i)
        for j in primelist: 
            if i*j>n: 
                break
            visit_bool=(1<<(i*j-1))|visit_bool 
            if i%j==0: 
                break

    primelist.insert(0,2)
    return primelist 


#test 
a=int(input("Please enter a number -> "))
print(eulerprime(a))
