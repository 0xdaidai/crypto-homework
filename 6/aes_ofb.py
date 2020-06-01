#!/usr/bin/env python
# -*- coding: utf-8 -*-
import aes

def encode(k,p,IV): 
    p_len=len(p)
    p_str=''
    for i in range(p_len):
        p_str+='{:0>2x}'.format(p[i])
    tmp = IV
    c_str=''
    index = 0
    p_len=len(p_str)
    while True:
        if index+32>=p_len:
            delta = p_len-index
            tmp = aes.encode(tmp,k)
            c_str += str_xor(p_str[index:index+delta],tmp[0:delta],delta)
            break

        tmp = aes.encode(tmp,k)
        c_str += str_xor(p_str[index:index+32],tmp,32)
        index +=32
    return bytes.fromhex(c_str)

def f_encode(k,fp,fr,IV):
    p=fp.read()
    fr.write(encode(k,p,IV))


def decode(k,p,IV): 
    p_len=len(p)
    p_str=''
    for i in range(p_len):
        p_str+='{:0>2x}'.format(p[i])
    tmp = IV
    c_str=''
    index = 0
    p_len=len(p_str)
    while True:
        if index+32>=p_len:
            delta = p_len-index
            tmp = aes.encode(tmp,k)
            c_str += str_xor(p_str[index:index+delta],tmp[0:delta],delta)
            break

        tmp = aes.encode(tmp,k)
        c_str += str_xor(p_str[index:index+32],tmp,32)
        index +=32
    return bytes.fromhex(c_str)

def f_decode(k,fp,fr,IV):
    p=fp.read()
    fr.write(decode(k,p,IV))


def str_xor(a,b,length):
    return '%0*x'%(length,int(a,16)^int(b,16))

if __name__ == '__main__':
    #fp=open('vim操作方法.png','rb')
    #fr=open('out','wb')
    #f_encode('6173646667686a6b6c7a786376626e6d',fp,fr,'6173646667686a6b6c7a786376626e6d')
    
    #  fpp=open('out','rb')
    #  frr=open('out1','wb')
    #  f_decode('6173646667686a6b6c7a786376626e6d',fpp,frr,'6173646667686a6b6c7a786376626e6d')
    IV="595cd11eb9b6fcf17ac54b101b97ab3d"
    key='0123456789abcdeffedcba9876543210'
    x = b'0123456789abcdeffedcba9876543210'
    print(encode(key, x, IV).hex())
