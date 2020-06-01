#!/usr/bin/env python
# -*- coding: utf-8 -*-
import aes

def encode(k,p): 
    p_len=len(p)
    block = 16
    str_tmp='{:0>2x}'.format(p[0])
    c_str=''
    for i in range(1,p_len):
        if i%block == 0:
            c_str += aes.encode(str_tmp,k)
            str_tmp='{:0>2x}'.format(p[i])
        else:
            str_tmp=str_tmp+'{:0>2x}'.format(p[i])
    delt = block- p_len%block
    delt_str = '{:0>2x}'.format(delt)
    for i in range(delt):
        str_tmp += delt_str
    c_str += aes.encode(str_tmp,k)
    c_len = len(c_str)
    return bytes.fromhex(c_str)

def f_encode(k,fp,fr):
    p = fp.read()
    fr.write(encode(k,p))

def decode(k,p): 
    p_len=len(p)
    block = 16
    str_tmp='{:0>2x}'.format(p[0])
    c_str=''
    for i in range(1,p_len):
        if i%block == 0:
            c_str += aes.decode(str_tmp,k)
            str_tmp='{:0>2x}'.format(p[i])
        else:
            str_tmp=str_tmp+'{:0>2x}'.format(p[i])
    c_str += aes.decode(str_tmp,k)
    c_len = len(c_str)
    res=bytes.fromhex(c_str)
    res_len=len(res)
    pad = res[res_len-1]
    return res[:res_len-pad]

def f_decode(k,fp,fr):
    p = fp.read()
    fr.write(decode(k,p))

if __name__ == '__main__':
    #fp=open('vim操作方法.png','rb')
    #fr=open('out','wb')
    #f_encode('6173646667686a6b6c7a786376626e6d',fp,fr)
    
    #  fpp=open('out','rb')
    #  frr=open('out1','wb')
    #  f_decode('6173646667686a6b6c7a786376626e6d',fpp,frr)
    IV="595cd11eb9b6fcf17ac54b101b97ab3d"
    key='0123456789abcdeffedcba9876543210'
    p=b"0123456789abcdeffedcba9876543210"
    c = encode(key, p)
    print(c.hex())
    print(decode(key, c))
