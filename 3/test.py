#!/usr/bin/env python
# -*- coding: utf-8 -*-
from affine import *
from affi_count import *
from column import *
from fence import *
from hill import *
from one_table import *
from vernam import *
from vigenere import *
from hill_attack import *

print('Welcome to the pub!')
print("Maybe you can see the help manual.")
print('What are you going to do this time? ')
while 1: 
    man = input("your choice -> ")
    if man == 'affine': 
        while 1:
            code=input('1 = encode , 0 = decode -> ')
            if code == '1':
                a,k,b = input('Please enter p k b -> ').split()
                k,b = int(k),int(b)
                print(affine_encode(a,k,b))
            else: 
                a,k,b = input('Please enter c k b -> ').split()
                k,b = int(k),int(b)
                print(affine_decode(a,k,b))
            on = input('go on or not? y/n ')
            if on =='n':
                break
    
    elif man == 'one_table': 
        while 1:
            code=input('1 = encode , 0 = decode -> ')
            if code == '1':
                a,k = input('Please enter p k -> ').split()
                print(one_table_encode(a,k))
            else: 
                a,k = input('Please enter c k -> ').split()
                print(one_table_decode(a,k))
            on = input('go on or not? y/n ')
            if on =='n':
                break
    
    elif man == 'vigenere': 
        while 1:
            code=input('1 = encode , 0 = decode -> ')
            if code == '1':
                a,k = input('Please enter p k -> ').split()
                print(vigenere_encode(a,k))
            else: 
                a,k = input('Please enter c k -> ').split()
                print(vigenere_decode(a,k))
            on = input('go on or not? y/n ')
            if on =='n':
                break
    
    elif man == 'vernam': 
        while 1:
            k = input('Please enter k -> ')
            s_r=input("input file -> ")
            s_w=input('output file -> ')
            fpr = open(s_r,'rb')
            fpw = open(s_w,'w')
            vernam_en_decode(fpr,k,fpw)
            on = input('go on or not? y/n ')
            if on =='n':
                break
    
    elif man == 'fence': 
        while 1:
            code=input('1 = encode , 0 = decode -> ')
            if code == '1':
                a,k = input('Please enter p skip -> ').split()
                k=int(k)
                print(fence_encode(a,k))
            else: 
                a,k = input('Please enter c skip -> ').split()
                k=int(k)
                print(fence_decode(a,k))
            on = input('go on or not? y/n ')
            if on =='n':
                break
    
    elif man =='hill':
        while 1:
            code=input('1 = encode , 0 = decode -> ')
            if code == '1':
                n = int(input('dim is? -> '))
                a = input('p? -> ')
                l = list(input(' k is here -> ').split())
                print(hill_encode(l,a,n))
            else: 
                n = int(input('dim is? -> '))
                a = input('c? -> ')
                l = list(input(' k is here -> ').split())
                print(hill_decode(l,a,n))
            on = input('go on or not? y/n ')
            if on =='n':
                break
    
    elif man == 'affi_count': 
        while 1:
            a = input('Please enter c -> ')
            affi_count(a)
            on = input('go on or not? y/n ')
            if on =='n':
                break
    
    elif man == 'column': 
        while 1:
            code=input('1 = encode , 0 = decode -> ')
            if code == '1':
                a,k = input('Please enter p k -> ').split()
                print(column_encode(k,a))
            else: 
                a,k = input('Please enter c k -> ').split()
                print(column_decode(k,a))
            on = input('go on or not? y/n ')
            if on =='n':
                break
    
    elif man == 'hill_attack': 
        while 1:
            p,c,n = input('Please enter p c dim -> ').split()
            n=int(n)
            tmp = hill_attack(p,c,n)
            if tmp == 0:
                print("Holy shit, I can't find the key! ")
            on = input('go on or not? y/n ')
            if on =='n':
                break
    
    else: 
        print('Your choice can be : ')
        print('affine affi_count column fence hill hill_attack one_table table_attack vigenere vernam')
        print('')
