#!/usr/bin/env python
# -*- coding: utf-8 -*-

def one_table_encode(p,k): 
    c=''
    for i in p: 
        tmp=ord(i)-97 
        c=c+k[tmp]
    return c 

def one_table_decode(c,k):
    p=''
    for i in c: 
        p=p+chr(k.index(i)+97)
    return p

