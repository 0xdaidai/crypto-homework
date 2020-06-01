#!/usr/bin/env python
# -*- coding: utf-8 -*-

def one_or_zero(x,b):
    return (x>>(b-1))&1


def prime_list(n):
#    list_bool=list(range(1,n+1))
#    list_bool[0]=0
#
#    for i in range(2,n+1):
#        if list_bool[i-1]!=0:
#            for j in range(i*i,n+1,i):
#                list_bool[j-1]=0
#
#    prime_list = [k for k in list_bool if k!=0]
#    return prime_list 
    print("[2",end='')
    prime_list=[]
    bool_int=0

    for i in range(3,n+1,2):
        if one_or_zero(bool_int,i)==0: 
            #prime_list.append(i)
            print(","+str(i),end='')
            for j in  range(i*i,n+1,2*i): 
                bool_int=(bool_int|(1<<(j-1)))
    #return prime_list
    print("]")


