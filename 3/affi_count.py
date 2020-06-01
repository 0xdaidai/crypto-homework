#!/usr/bin/env python
# -*- coding: utf-8 -*-
from enchant.checker import SpellChecker 
from enchant.tokenize import get_tokenizer

def cor(s):
    chkr = SpellChecker("en_US")
    chkr.set_text(s)
    chkr_count=0
    thnzr_count=0
    for i in chkr: 
        chkr_count=chkr_count+1
    thnzr = get_tokenizer("en_US")
    for i in thnzr(s): 
        thnzr_count=thnzr_count+1
    return str(1 - chkr_count / thnzr_count)

def affi_count(c):
    c=c.lower()
    c_count={}
    for i in c: 
        c_count[i]=c.count(i)
    c_sort=sorted(c_count.items(), key = lambda f:f[1], reverse = True)
    times=0
    for  it in c_sort: 
        p=''
        if it[0].isalpha(): 
            k=(ord(it[0])- ord('e'))%26 #get key 
            times+=1
            for i in c: 
                if i.isalpha(): 
                    p=p+chr((ord(i)-97-k)%26+97)
                else: 
                    p=p+i 
            print('Level : '+str(times)+' , key = '+str(k) + ' correct probability: '+cor(p))
            print(p)
            print('')
        if times == 10:
            break;

