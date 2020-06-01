#!/usr/bin/env python
# -*- coding: utf-8 -*-
import random
import math
from enchant.checker import SpellChecker 
from enchant.tokenize import get_tokenizer

def check(s):
    chkr = SpellChecker("en_US")
    chkr.set_text(s)
    chkr_count=0
    thnzr_count=0
    for i in chkr: 
        chkr_count=chkr_count+1
    thnzr = get_tokenizer("en_US")
    for i in thnzr(s): 
        thnzr_count=thnzr_count+1
    return 1 - chkr_count / thnzr_count


def cor(f,c_count,all_length): 
    my_dicc = {
        'a':8.167,
        'b':1.492,
        'c':2.872,
        'd':4.253,
        'e':12.702,
        'f':2.228,
        'g':2.015,
        'h':6.094,
        'i':6.996,
        'j':0.153,
        'k':0.772,
        'l':4.025,
        'm':2.406,
        'n':6.749,
        'o':7.507,
        'p':1.929,
        'q':0.095,
        'r':5.987,
        's':6.327,
        't':9.056,
        'u':2.758,
        'v':0.978,
        'w':2.360,
        'x':0.150,
        'y':1.974,
        'z':0.074
    }
    weight = 0
    for k,v in f.items(): 
        weight += (c_count[k]/all_length - my_dicc[v])**2
    return math.sqrt(weight)


def table_attack(c): 
    my_dic='etaonrishdlfcmugypwbvkjxqz'
    c=c.lower()
    c_count={}
    f={}
    p_list=[]
    all_length=0
    for i in c: 
        if i.isalpha(): 
            all_length += c.count(i)
        c_count[i]=c.count(i)
    c_sort=sorted(c_count.items(), key = lambda f:f[1], reverse = True)
    for i in range(26):
        alpha = chr(i+97) 
        if alpha not in c_count.keys(): 
            c_count[alpha] = 0
        if c_sort_in(alpha,c_sort):
            f[alpha] = my_dic[c_sort_index(c_sort,alpha)]
        else: 
            f[alpha] = alpha
    p= exc(c,f)
    rank = cor(f,c_count,all_length)
    p_list.append([p,rank])

    i_tmp=0
    j_tmp=0
    t = 1000 
    cooling = 0.999 
    min = 0.0001
    while(t>min): 
        print(t)
        i = random.randint(0,25) 
        j = random.randint(0,25) 
        if i == j: 
            j = (i+1) if i<25 else 0
        swap(f,i,j)
        tmp = cor(f,c_count,all_length)
        drank = rank - tmp 
        p = exc(c,f)
        if check(p)>0.3 and (0-check(p))<rank: 
            tmp = 0-check(p)
            drank = -1
        if drank<0 or random.random()< math.exp(-drank/t): 
            rank = tmp 
            p_list.append([p,rank])
        swap(f,i,j)
        t=t*cooling
    #for time in range(1000): 
    #    for i in range(25): 
    #        for j in range(i,26): 
    #            swap(f,i,j)
    #            tmp = cor(f,c_count,all_length)
    #            if tmp>max_cor: 
    #                max_cor = tmp 
    #                i_tmp = i 
    #                j_tmp = j
    #            swap(f,i,j)
    #    swap(f,i_tmp,j_tmp)
    #    p= exc(c,f)
    #    rank = cor(f,c_count,all_length)
    #    p_list.append([p,rank])
    p_list=sorted(p_list, key = lambda f:f[1], reverse = True)
    pr_time= 0
    for i in p_list: 
        pr_time =pr_time +1 
        if pr_time>10: 
            break
        print('level : ' + str(pr_time)+'    pos: '+str(i[1]))
        print(i[0])
        print('')


def exc(c,f): 
    p=''
    for i in c: 
        if i.isalpha():
            p=p+f[i]
        else: 
            p = p+i
    return p


def swap(f,i,j):
    tmp = f[chr(i+97)]
    f[chr(i+97)]=f[chr(j+97)]
    f[chr(j+97)] = tmp 


def c_sort_in(a,c_sort):
    for i in c_sort: 
        if a == i[0]:
            return True
    return False


def c_sort_index(c_sort,a): 
    time = 0
    for i in c_sort: 
        if a == i[0]: 
            return time
        time = time+1


c=input()
table_attack(c)
