#!/usr/bin/env python
# -*- coding: utf-8 -*-
from arithmetic import *

def gcd(a,b): 
    if(b==0): 
        return a
    return gcd(b,mod(a,b))
