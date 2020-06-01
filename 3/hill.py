#!/usr/bin/env python
# -*- coding: utf-8 -*-
import numpy as np
from ojld import *

def my_det(mt): 
    det = np.linalg.det(mt)
    if det<0:
        det -= 0.5
    else: 
        det +=0.5
    return int(det)

def hill_encode(k,p,n,flag=1):
    c=''
    p_list=[]
    tmp_list=[0]*n
    if flag==1:
        k=np.array(k,dtype=int).reshape(n,n)
    for i in p: 
        p_list.append(ord(i)%97)
    for j in range(0,len(p_list),n):
        for jj in range(n):
            tmp_list[jj]=p_list[j+jj]
        tmp_list=np.array(tmp_list)
        c_arr=np.dot(tmp_list,k)
        for i in c_arr: 
            c=c+chr(int(i)%26+97)
    return c

def inv(k,n): 
    det_k = my_det(k)
    det_k_inv = ojld(det_k,26)
    tmp=adjoint(k,n)
    for i in range(n): 
        for j in range(n): 
            tmp[i][j]=(int(tmp[i][j]) * det_k_inv) % 26
    return tmp

def cofactor(k,x,y,n): 
    tmp_mat=[[0 for i in range(n-1)]for j in range(n-1)]
    for i in range(n-1):
        for j in range(n-1): 
            tmp_mat[i][j]=k[i if i<x else i+1][j if j<y else j+1]
    tmp=my_det(tmp_mat)
    return ((-1)**(x+y))*tmp
#%26?

def adjoint(k,n): 
    tmp_mat=[[0 for i in range(n)]for j in range(n)]
    for i in range(n):
        for j in range(n):
            tmp_mat[i][j]=cofactor(k,i,j,n)
    return np.array(tmp_mat).transpose()

def hill_decode(k,c,n): 
    k=np.array(k,dtype=int).reshape(n,n)
    k_inv=inv(k,n)
    return hill_encode(k_inv,c,n,0)


