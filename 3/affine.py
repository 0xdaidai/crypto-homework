#!/usr/bin/env python
# -*- coding: utf-8 -*-
from ojld import *

def affine_encode(p,k,b):
    if k%2==0 or k%13==0: 
        return 'Error k'
    c=""
    for i in p: 
        c=c+chr(((ord(i)-97)*k+b)%26+97)
    return c

def affine_decode(c,k,b): 
    if k%2==0 or k%13==0: 
        return 'Error k'
    p=''
    kk=ojld(k,26)
    for i in c: 
        p=p+chr(kk*(ord(i)-97-b)%26+97)
    return p

