#!/usr/bin/env python
# -*- coding: utf-8 -*-
import hashlib
import random

def base(mm):
    mm_l = []
    mm_h = []
    hLen = 20*8
    num = 2**80
    tab = bytearray()
    for i in range(32,47):
        tab.append(i)
    for i in range(58,64):
        tab.append(i)
    for i in range(91,97):
        tab.append(i)

    tLen = len(tab)
    for i in range(num):
        tmp = random.randint(0, tLen-1)
        index = random.randint(0, len(mm))
        m_t = bytearray(mm)
        for j in range(10):
            m_t.insert(index, tab[tmp])
        mm_l.append(m_t)

    #  print("base")
    for i in mm_l:
        mm_h.append(get_str_sha1_secret_str(i))
    for i in range(len(mm_h)):
        print(mm_h[i])
    return mm_l,mm_h

def attack(m,mm_l,mm_h):
    m_l = []
    hLen = 20*8
    num = 2**80
    tab = bytearray()
    for i in range(32,47):
        tab.append(i)
    for i in range(58,64):
        tab.append(i)
    for i in range(91,97):
        tab.append(i)

    tLen = len(tab)
    for i in range(num):
        tmp = random.randint(0, tLen-1)
        index = random.randint(0, len(m))
        m_t = bytearray(m)
        for j in range(10):
            m_t.insert(index, tab[tmp])
        m_l.append(m_t)

    print('\n\n\n')

    for i in m_l:
        print(i)
        for j in mm_h:
            hashres = get_str_sha1_secret_str(i)
            if hasheq(hashres,j):
                return mm_l[mm_h.index(j)]
    return '404 Not found'

def hasheq(a,b):
    for i in range(3):
        if a[i]!=b[i]:
            return 0
    return 1


def get_str_sha1_secret_str(res):
    sha = hashlib.sha1(res)
    encrypts = sha.digest()
    return encrypts

if __name__ == '__main__':
    m = b'holyshit'
    mm = b'ao li gei'
    mm = bytearray(mm)
    m = bytearray(m)
    num = 0
    #  print(get_str_sha1_secret_str(m))
    mm_l,mm_h = base(mm)
    while 1:
        num += 1
        print(num)
        res = attack(m,mm_l, mm_h)
        if res[0:3]!='404' :
            print('success')
            break
        m.append(32)
    print(res)
