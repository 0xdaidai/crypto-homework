#!/usr/bin/env python
# -*- coding: utf-8 -*-
import aes

def encode(k,p,IV,s=8):
    p_len=len(p)
    p_str=''
    s=s//4
    for i in range(p_len):
        p_str+='{:0>2x}'.format(p[i])
    p_len = len(p_str)
    c=''
    index = 0
    tmp_reg = aes.encode(IV,k)
    while True:
        if (p_len-index)<=s:
            s = p_len-index
            s_tmp = tmp_reg[0:s]
            c_tmp = str_xor(p_str[index:index+s],s_tmp,s)
            c=c+c_tmp
            break
        s_tmp = tmp_reg[0:s]
        c_tmp = str_xor(p_str[index:index+s],s_tmp,s)
        c=c+c_tmp
        index +=s
        tmp_reg = tmp_reg[s:]+c_tmp
        tmp_reg=aes.encode(tmp_reg,k)
    return bytes.fromhex(c)

def f_encode(k,fp,fr,IV,s=8):
    p=fp.read()
    fr.write(encode(k,p,IV,s))

def decode(k,p,IV,s=8):
    p_len=len(p)
    p_str=''
    s=s//4
    for i in range(p_len):
        p_str+='{:0>2x}'.format(p[i])
    p_len = len(p_str)
    c=''
    index = 0
    tmp_reg = aes.encode(IV,k)
    while True:
        if (p_len-index)<=s:
            s = p_len-index
            s_tmp = tmp_reg[0:s]
            c_tmp = str_xor(p_str[index:index+s],s_tmp,s)
            c=c+c_tmp
            break
        s_tmp = tmp_reg[0:s]
        c_tmp = str_xor(p_str[index:index+s],s_tmp,s)
        c=c+c_tmp
        tmp_reg = tmp_reg[s:]+p_str[index:index+s]
        index +=s
        tmp_reg=aes.encode(tmp_reg,k)
    return bytes.fromhex(c)

def f_decode(k,fp,fr,IV,s=8):
    p=fp.read()
    fr.write(decode(k,p,IV,s))

def str_xor(a,b,length):
    return '%0*x'%(length,int(a,16)^int(b,16))

if __name__ == '__main__':
    #fp=open('vim操作方法.png','rb')
    #fr=open('out','wb')
    #f_encode('6173646667686a6b6c7a786376626e6d',fp,fr,'6173646667686a6b6c7a786376626e6d')
    
    fpp=open('out','rb')
    frr=open('out1','wb')
    f_decode('6173646667686a6b6c7a786376626e6d',fpp,frr,'6173646667686a6b6c7a786376626e6d')
