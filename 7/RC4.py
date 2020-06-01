def key_s(key):
    s = [0] * 256
    t = [0] * 256
    key0 = []
    length = len(key)
    if length % 2 == 0:
        num = len(key) // 2
    else:
        num = len(key)//2 + 1
    for i in range(num):
        key0.append(int(key[i * 2:i * 2 + 2], 16))
    # 初始化
    for i in range(256):
        s[i] = i
        t[i] = key0[i % num]
    # 初始排列s
    j = 0
    for i in range(256):
        j = (j + s[i] + t[i]) % 256
        s[i], s[j] = s[j], s[i]
    i = 0
    j = 0
    return s


def encrypt(m, key, fw):
    s = key_s(key)
    # 将字符转换为数字
    c = []
    list0 = []
    for i in m:
        list0.append(int('{:0>2x}'.format(i), 16))

    length0 = len(m)
    i = 0
    j = 0
    n = 0
    while length0 != 0:
        i = (i + 1) % 256
        j = (j + s[i]) % 256
        s[i], s[j] = s[j], s[i]
        t = (s[i] + s[j]) % 256
        k = s[t]
        list0[n] = list0[n] ^ k
        temp1 = '{:0>2x}'.format(list0[n])
        n = n + 1
        temp = bytes.fromhex(temp1)
        fw.write(temp)
        length0 = length0 - 1


def decode(m, key, fw):
    s = key_s(key)
    # 将字符转换为数字
    c = []
    list0 = []
    for i in m:
        list0.append(int('{:0>2x}'.format(i), 16))

    length0 = len(m)
    i = 0
    j = 0
    n = 0
    while length0 != 0:
        i = (i + 1) % 256
        j = (j + s[i]) % 256
        s[i], s[j] = s[j], s[i]
        t = (s[i] + s[j]) % 256
        k = s[t]
        list0[n] = list0[n] ^ k
        temp1 = '{:0>2x}'.format(list0[n])
        n = n + 1
        temp = bytes.fromhex(temp1)
        fw.write(temp)
        length0 = length0 - 1


def main():
    while True:
        print("请选择要进行的操作：1.加密 2.解密 3.退出")
        flag = input()
        if flag == '3':
            break
        else:
            print("请输入密钥：")
            key = input()
            if flag == '1':
                file = input("请输入文件名:")
                f = open(file, "rb")
                m = f.read()
                file0 = input("请输入加密后文件的文件名：")
                fw = open(file0, "wb")
                encrypt(m, key, fw)
                f.close()
                fw.close()
            if flag == '2':
                file = input("请输入文件名：")
                f = open(file, "rb")
                c = f.read()
                file0 = input("请输入解密后文件的文件名：")
                fw = open(file0, "wb")
                decode(c, key, fw)
                f.close()
                fw.close()


if __name__ == '__main__':
    main()