#!/usr/bin/env python
# -*- coding: utf-8 -*-
import aes

def encode(k,p,IV): 
    p_len=len(p)
    block = 16
    p_str=''
    for i in range(p_len):
        p_str+='{:0>2x}'.format(p[i])
    nr = p_len//block+1
    #padding 
    delta = block-p_len%block
    del_str = '{:0>2x}'.format(delta)
    for i in range(delta):
        p_str += del_str

    c_str=''
    c_nr = IV
    for i in range(nr):
        c_nr = aes.encode(str_xor_16(c_nr,p_str[i*32:i*32+32]),k)
        c_str += c_nr
    return bytes.fromhex(c_str)

def f_encode(k,fp,fr,IV):
    p=fp.read()
    fr.write(encode(k,p,IV))


def decode(k,p,IV):
    p_len=len(p)
    block = 16
    p_str=''
    nr = p_len//block
    for i in range(p_len):
        p_str+='{:0>2x}'.format(p[i])

    c_str=''
    c_nr = IV
    for i in range(nr):
        c_nr = str_xor_16(aes.decode(p_str[i*32:i*32+32],k),c_nr)
        c_str += c_nr
        c_nr = p_str[i*32:i*32+32]
    res=bytes.fromhex(c_str)
    res_len=len(res)
    pad = res[res_len-1]
    return res[:res_len-pad]

def f_decode(k,fp,fr,IV):
    p=fp.read()
    fr.write(decode(k,p,IV))

def str_xor_16(a,b):
    return '{:0>32x}'.format(int(a,16)^int(b,16))


if __name__ == '__main__':
    #fp=open('vim操作方法.png','rb')
    #fr=open('out','wb')
    #f_encode('6173646667686a6b6c7a786376626e6d',fp,fr,'6173646667686a6b6c7a786376626e6d')
    
    #fpp=open('out','rb')
    #frr=open('out1','wb')
    #f_decode('6173646667686a6b6c7a786376626e6d',fpp,frr,'6173646667686a6b6c7a786376626e6d')

    #x = 'flag{aes_ecb_is_interesting}'
    #key = '6173646667686a6b6c7a786376626e6d'
    #c=encode(key,bytes(x,encoding='utf8'),key)
    #print(c.hex().upper())
    #print(key.upper())

    x=b'0123456789abcdef'
    IV="595cd11eb9b6fcf17ac54b101b97ab3d"
    key='0123456789abcdeffedcba9876543210'
    c = encode(key, x, IV)
    print(c.hex())
    print(decode(key, c, IV))
