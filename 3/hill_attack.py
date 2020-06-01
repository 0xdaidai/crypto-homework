#!/usr/bin/env python
# -*- coding: utf-8 -*-
from hill import *
import random
import numpy as np
import sys
import copy

def getAllP(tp, vis, lens,cur,res,tmp):
    if cur == lens:
        for i in range(lens-1):
            for j in range(i,lens):
                if tmp[i]<tmp[j]:
                    return
        res.append(copy.deepcopy(tmp))
        return
    for i in range(lens):
        if vis[i] ==0:
            vis[i]=1
            tmp.append(tp[i])
            cur +=1
            getAllP(tp,vis,lens,cur,res,tmp)
            cur -=1
            vis[i] =0
            tmp.pop()

def hill_attack(p,c,n): 
    p_len=len(p)
    l = p_len//n
    p_mat=[[0 for i in range(n)]for i in range(n)]
    c_mat=[[0 for i in range(n)]for i in range(n)]
    p_mat = np.array(p_mat,dtype=int)
    c_mat = np.array(p_mat,dtype=int)
    times=1000
    flag = 0
    for time in range(times): 
        index = 0
        index_list=[]
        for i in range(n): 
            index = random.randint(index,l-n+i)
            index_list.append(index)
            for j in range(n):
                p_mat[i][j]=ord(p[index*n+j])-97
            index = index + 1 
        det_tmp = my_det(p_mat)
        if det_tmp%13!=0 and det_tmp%2!=0: 
            p_mat_inv = inv(p_mat,n)
            for i in range(n): 
                for j in range(n): 
                    c_mat[i][j]=ord(c[index_list[i]*n+j])-97
            arr=np.dot(p_mat_inv,c_mat)
            flag = 1
            break
    if flag==1: 
        for i in range(n):
            arr[i]=arr[i]%26
        print(arr)
        return 1
    return 0

