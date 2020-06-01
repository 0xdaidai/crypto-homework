#!/usr/bin/env python
# -*- coding: utf-8 -*-

def fast_mod(x,n,m):
    d=1
    while n>0: 
        if n%2==1:
            d=((d%m)*(x%m))%m
        n = n>>1
        x=((x%m)*(x%m))%m
    return d


