#!/usr/bin/env python
# -*- coding: utf-8 -*-

def sha1(m:bytearray):
    mLen = len(m)
    l = mLen * 8
    print(mLen)
    m.append(0x80)
    mLen +=1
    if mLen%64 != 56:
        while mLen %64!=56: 
            m.append(0)
            mLen += 1
    m = m+ bytearray.fromhex("{:0>16x}".format(l))
    mLen = len(m)
    print(mLen)

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
        w.append(int(m[4*i:4*i+4].hex(),16))

    for i in range(16,80):
        temp = w[i-3]^w[i-8]^w[i-14]^w[i-16]
        w.append((temp<<1 | temp>>31)&0xffffffff)

    for i in range(80):
        temp2 = (a<<5|a>>27)&0xffffffff
        if i//20 == 0:
            temp = (b&c)|((~(b))&d)
            temp += K[0]
        elif i//20 == 1:
            temp = (b^c^d)
            temp += K[1]
        elif i//20 == 2:
            temp = (b&c)|(b&d)|(c&d)
            temp += K[2]
        else:
            temp = (b^c^d)
            temp += K[3]
        temp += temp2 + e + w[i]
        temp &= 0xffffffff
        e = d;
        d = c;
        c = (b<<30 | b>>2)&0xffffffff; 
        b = a;
        a = temp;
    return (aa+a)&0xffffffff,(bb+b)&0xffffffff,(cc+c)&0xffffffff,(dd+d)&0xffffffff,(ee+e)&0xffffffff

if __name__ == "__main__":
    m = bytearray("abcdbcdecdefdefgefghfghighijhijkijkljklmklmnlmnomnopnopq",encoding='utf8')
    print(sha1(m).hex())
