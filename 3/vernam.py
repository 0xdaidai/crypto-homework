#!/usr/bin/env python
# -*- coding: utf-8 -*-

def vernam_en_decode(fpr,k,fpw): 
    p=fpr.read()
    print(type(p))
    tmp = -1
    k_len=len(k)
    c=''
    for i in p: 
        tmp=(tmp+1)%k_len 
        c= c+chr(ord(k[tmp])^i)
    fpw.write(c)

