#!/usr/bin/env python
# -*- coding: utf-8 -*-
import pwn
#import aes_cbc_pad as aes

def refresh(x):
    l = len(x)
    for i in range(l):
        x[i] = 0

if __name__ == '__main__':
    #key = '6173646667686a6b6c7a786376626e6d'
    p=bytearray()
    IV_t = bytearray.fromhex('6173646667686A6B6C7A786376626E6D')
    c = bytearray.fromhex('D4C8D8DFC3604A465FE1477C92FCFF3D60490AA863AACC4C0D51FD678DC13F1F')
    tmp= bytearray.fromhex('00'*16)
    IV_n = bytearray.fromhex('00'*16)
    p_t = bytearray.fromhex('00'*16)
    lon = pwn.remote('39.96.57.195',9090)
    s= lon.recv().decode()
    print(s)
    s= lon.recv().decode()
    print(s)
    c_len=len(c)
    index = 0
    flag1 = False
    
    while index*16<c_len:
        c_tmp = c[index*16:index*16+16]
        refresh(p_t)
        refresh(IV_n)
        for nr in range(15,13,-1):
            #init the IV_n
            for i in range(15,nr,-1):
                IV_n[i] = p_t[i]^(16-nr)
            while True:
                data = IV_n+c_tmp
                lon.send(data)
                s= lon.recv()
                #s= aes.decode(key,c_tmp,IV_n.hex())
                if s[0]!=87:
                    if nr == 15:
                        IV16 = IV_n[nr]
                    p_t[nr] = IV_n[nr]^(16-nr)
                    break
                else:
                    if IV_n[nr] == 255:
                        flag1 = True
                        break
                    IV_n[nr]+=1
        #Multiple solutions may appear in the first poll
        if flag1:
            refresh(p_t)
            refresh(IV_n)
            IV_n[15] = IV16+1
            for nr in range(15,13,-1):
                #init the IV_n
                for i in range(15,nr,-1):
                    IV_n[i] = p_t[i]^(16-nr)
                while True:
                    data = IV_n+c_tmp
                    lon.send(data)
                    s= lon.recv()
                    #s= aes.decode(key,c_tmp,IV_n.hex())
                    if s[0]!=87:
                        p_t[nr] = IV_n[nr]^(16-nr)
                        break
                    else:
                        IV_n[nr]+=1


        for nr in range(13,-1,-1):
            #init the IV_n
            for i in range(15,nr,-1):
                IV_n[i] = p_t[i]^(16-nr)
            while True:
                data = IV_n+c_tmp
                lon.send(data)
                s= lon.recv()
                #s= aes.decode(key,c_tmp,IV_n.hex())
                if s[0]!=87:
                    p_t[nr] = IV_n[nr]^(16-nr)
                    break
                else:
                    IV_n[nr]+=1
        for i in range(16):
            tmp[i] = p_t[i]^IV_t[i]
        p = p+tmp
        index+=1
        IV_t = c_tmp
    padd = p[len(p)-1]
    p = p[:len(p)-padd]
    print(p)
