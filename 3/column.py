#!/usr/bin/env python
# -*- coding: utf-8 -*-
import numpy as np

def column_encode(k,p):
    c=''
    lie=len(k)
    hang=len(p)//len(k)
    if len(p)%len(k)!=0: 
        hang=hang+1
        for i in range(lie-len(p)%lie): 
            p=p+'z'

    mat=[]
    for i in p: 
        mat.append(ord(i))
    mat = np.array(mat).reshape(hang,lie).transpose()
    mat_tmp=[[]for i in range(lie)]
    count=0
    for i in k: 
        tmp =int(i)
        mat_tmp[tmp-1]=mat[count]
        count=count+1
    for i in mat_tmp: 
        for j in i: 
            c=c+chr(j)
    return c

def column_decode(k,c):
    p=''
    lie=len(k)
    hang=len(c)//len(k)
    mat=[]
    for i in c: 
        mat.append(ord(i))
    mat = np.array(mat).reshape(lie,hang)
    #matprint(mat.transpose())
    mat_tmp=[[]for i in range(lie)]
    count=0
    for i in range(lie): 
        tmp=k.index(str(i+1))
        mat_tmp[tmp]=mat[count]
        count=count+1
    mat_tmp=np.array(mat_tmp).transpose()
    #matprint(mat_tmp)
    for i in mat_tmp: 
        #print(i)
        for j in i: 
            p=p+chr(j)
    return p

def matprint(mat):
    for i in mat: 
        for j in i: 
            print(chr(j),end=' ')
        print('')
    print('')


