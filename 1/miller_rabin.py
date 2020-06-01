#!/usr/bin/env python
# -*- coding: utf-8 -*-

import random
from mod import *

def miller_rabin(p): 
    k=0
    q=p-1
    while q%2==0: 
        k=k+1
        q=q>>1

    randomnum = random.randint(1,p-1)
    print("random_number is "+str(randomnum))
    if fast_mod(randomnum,q,p)==1: 
        return 1
    for i in range(0,k): 
        if fast_mod(randomnum,(2**i)*q,p)==p-1: 
            return 1
    return -1
