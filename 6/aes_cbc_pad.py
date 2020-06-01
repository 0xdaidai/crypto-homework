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
    flag = True
    if pad ==0:
        flag = False
    else:
        for i in range(res_len-1,res_len-1-pad,-1):
            if res[i] !=pad:
                flag = False
                break
    if flag == False:
        return bytes('Wrong padding!!',encoding='utf8')
    return bytes('That maybe true',encoding='utf8')

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

    x = 'D4C8D8DFC3604A465FE1477C92FCFF3D60490AA863AACC4C0D51FD678DC13F1F'
    key = '6173646667686a6b6c7a786376626e6d'
    c=decode(key,bytes.fromhex(x),key)
    print(c.decode())
    print(key.upper())
