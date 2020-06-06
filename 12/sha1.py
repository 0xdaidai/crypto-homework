#!/usr/bin/env python
# -*- coding: utf-8 -*-

def sha1(m:bytearray):
    mLen = len(m)
    print(mLen)
    if mLen%64 != 56:
        m.append(0x80)
        for i in range(mLen%64 - 1):
            m.append(0)
    m = m+ bytearray.fromhex("{:0>8x}".format(mLen))
    mLen = len(m)

    a = 0x67452301
    b = 0xEFCDAB89
    c = 0x98BADCFE
    d = 0x10325476
    e = 0xC3D2E1F0

    for i in range(mLen//64):
        a,b,c,d,e = F(m[i*64:i*64+64],a,b,c,d,e) 
    return bytearray.fromhex("{:0>8x}".format(a)+"{:0>8x}".format(b)+"{:0>8x}".format(c)+"{:0>8x}".format(d)+"{:0>8x}".format(e))

def F(m,aa,bb,cc,dd,ee):
    K = [0x5A827999,0x6ED9EBA1,0x8F1BBCDC,0xCA62C1D6]
    a = aa
    b = bb
    c = cc
    d = dd
    e = ee

    w=[]
    for i in range(16):
        w[i] = int(m[4*i:4*i+4].hex(),16)

    for i in range(16,80):
        temp = w[i-3]^w[i-8]^w[i-14]^w[i-16]
        w[i] = (temp<<1 | temp>>31)&0xffffffff

    for i in range(80):
        temp2 = a<<5|a>>27
        if i//20 == 0:
            temp = (b&c)|((~(b))&d)
            temp += K[0]
        elif i//20 == 1:
            temp = (b^c^d)
            temp += k[1]
        elif i//20 == 2:
            temp = (b&c)|(b&d)|(c&d)
            temp += k[2]
        else:
            temp = (b^c^d)
            temp += k[3]
        temp += temp2 + e + w[j]
        e = d;
        d = c;
        c = (b<<30 | b>>2)&0xffffffff; 
        b = a;
        a = temp;
        return aa+a,bb+b,cc+c,dd+d,ee+e

if __name__ == "__main__":
    m = bytearray.fromhex("8F7F6F5F4F3F2F1F0F")
    print(sha1(m).hex())
