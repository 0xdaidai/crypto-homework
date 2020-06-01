#!/usr/bin/env python
# -*- coding: utf-8 -*-

import hashlib

def get_str_sha1_secret_str(res):
    """
    使用sha1加密算法，返回str加密后的字符串
    """
    sha = hashlib.sha1(res)
    encrypts = sha.hexdigest()
    return encrypts

a=bytearray(b'\x01\x02\x03\x04')
b=get_str_sha1_secret_str(a)
print(len(b))

