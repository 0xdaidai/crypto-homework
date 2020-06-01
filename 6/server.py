#!/usr/bin/python
# -*- coding: UTF-8 -*-

import sys
import socket
import aes_cbc_pad as aes
# 建立一个服务端
server = socket.socket(socket.AF_INET,socket.SOCK_STREAM)
server.bind(('localhost',9090)) #绑定要监听的端口
server.listen(5) #开始监听 表示可以使用五个链接排队
while True:# conn就是客户端链接过来而在服务端为期生成的一个链接实例
    conn,addr = server.accept() #等待链接,多个链接的时候就会出现问题,其实返回了两个值
    print(conn,addr)
    data = 'Hi, I hava intercepted an interesting message. Do you know what it measns?\n'
    conn.send(data.encode())
    conn.send('Here is the message:\n'.encode())
    conn.send('6173646667686A6B6C7A786376626E6DD4C8D8DFC3604A465FE1477C92FCFF3D60490AA863AACC4C0D51FD678DC13F1F\n\n'.encode())
    key = '6173646667686a6b6c7a786376626e6d'

    while True:
        data = conn.recv(4096)  #接收数据
        if not data:
            break
        print('recive:',data) #打印接收到的数据
        print(len(data))
        if len(data)<32:
            data = 'Wrong length'.encode()
        else:
            IV = data[:16].hex()
            c = data[16:]
            data = aes.decode(key,c,IV)
        conn.send(data.upper()) #然后再发送数据
    conn.close()
