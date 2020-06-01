#!/usr/bin/env python
# -*- coding: utf-8 -*-

def mod_normal(a,b,c):
    d=1
    for i in range(1,b+1): 
        d = ((d%c)*(a%c))%c
    return d

