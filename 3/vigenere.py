#!/usr/bin/env python
# -*- coding: utf-8 -*-

def vigenere_encode(p,k):
    key_length=len(k)
    tmp=-1
    c=''
    for i in p: 
        tmp=(tmp+1)%key_length
        c=c+chr((ord(k[tmp])+ord(i)-97-97)%26+97)
    return c

def vigenere_decode(c,k): 
    key_length=len(k)
    tmp=-1
    p=''
    for i in c: 
        tmp=(tmp+1)%key_length
        p=p+chr((ord(i) - ord(k[tmp]))%26+97)
    return p

