#!/usr/bin/env python
# -*- coding: utf-8 -*-

from arithmetic import *
from fast_mod import *
from ojld import *
from gcd import *
from prime import *

while 1: 
    oper=input("Pleaase input your choice -> ")
    if oper == 'exit': 
        exit()

    elif oper =='ari': 
        while 1: 
            oper_1 = input("What can I do for you? ")
            if oper_1 == 'back': 
                break;
            elif oper_1 == '+': 
                a,b=input("Please enter two number -> ").split()
                a,b=int(a,16),int(b,16)
                tmp = plus(a,b)
                print("a +- b = "+bin(tmp)+' '+hex(tmp))
            elif oper_1 == '*': 
                a,b=input("Please enter two number -> ").split()
                a,b=int(a,16),int(b,16)
                tmp=mul(a,b)
                print("a * b = "+bin(tmp)+' '+hex(tmp))
            elif oper_1 == '/': 
                a,b=input("Please enter two number -> ").split()
                a,b=int(a,16),int(b,16)
                tmp = div(a,b)
                print("a / b = "+bin(tmp)+' '+hex(tmp))
            elif oper_1 == '%': 
                a,b=input("Please enter two number -> ").split()
                a,b=int(a,16),int(b,16)
                tmp=mod(a,b)
                print("a % b = "+bin(tmp)+' '+hex(tmp))
            else: 
                print('error input! ')

    elif oper == 'fast_mod': 
        while 1:
            a,b=input("Please enter two number -> ").split()
            a,b=int(a,16),int(b)
            tmp=fast_mod(a,b)
            print("a fast mod b == "+bin(tmp)+' '+hex(tmp))
            oper_1 = input('Go on ? [y/n]')
            if oper_1 == 'n':
                break

    elif oper == 'ojld': 
        while 1: 
            a=int(input('Pleaase enter your number -> '),16)
            m=0b100011011
            tmp=ojld(a,m)
            print('Inverse element is '+bin(tmp)+' '+hex(tmp))
            oper_1 = input('Go on ? [y/n]')
            if oper_1 == 'n':
                break

    elif oper == 'gcd': 
        while 1: 
            a,b=input("Please enter two number -> ").split()
            a,b=int(a,16),int(b,16)
            tmp=gcd(a,b)
            print("gcd is "+bin(tmp)+' '+hex(tmp))
            oper_1 = input('Go on ? [y/n]')
            if oper_1 == 'n':
                break

    elif oper == 'extgcd': 
        while 1:
            a,b=input("Please enter two number -> ").split()
            a,b=int(a,16),int(b,16)
            tmp=ojld(a,b)
            oper_1 = input('Go on ? [y/n]')
            if oper_1 == 'n':
                break

    elif oper == 'prime': 
        while 1: 
            a=int(input('Pleaase enter your number -> '))
            tmp=prime(reduci(a))
            print("Output: ")
            for i in tmp: 
                print("("+bin(i)+','+hex(i)+') ')
            oper_1 = input('Go on ? [y/n]')
            if oper_1 == 'n':
                break

    elif oper == 'help':
        print('Hi,your choice can be like those:')
        print('ari,fast_mod,gcd,extgcd,ojld,prime')
        print('You can go back by input "back"')

    else :
        print('Error input! ')
