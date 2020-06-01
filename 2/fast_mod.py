#!/usr/bin/env python
# -*- coding: utf-8 -*-
from arithmetic import *

def fast_mod(x,n,m=0b100011011):
    d=1
    while n>0: 
        if n&1==1: 
            d=mod(mul(d,x),m)
        n=n>>1
        x=mod(mul(x,x),m)
    return d

