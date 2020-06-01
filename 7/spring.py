counter = [0b11000000, 0b00100100, 0b00111011, 0b01000001, 0b01101101, 0b01001101, 0b11000011, 0b10110111, 0b11010111,
           0b01000101
           ]
def to_state(message):  # 转成state
    state = []
    for i in range(4):
        state_mid = []
        for j in range(4):
            state_mid.append(message[i * 4 + j])
        state.append(state_mid)
    return state
def add_round_key(state: list, k: list):  # 轮密钥加
    for i in range(len(state)):
        for j in range(len(state[i])):
            state[i][j] ^= k[i][j]
    return state
def transpose(state):  # 矩阵转置
    transpose_state = []
    for i in range(4):
        mid = []
        for j in range(4):
            mid.append(state[j][i])
        transpose_state.append(mid)
    return transpose_state
def get_int_bit(n: int, index: int):  # 获得整数n第index位二进制数
    return (n >> index) & 1
def fa(n: int):
    return get_int_bit(n, 6) ^ (get_int_bit(n, 4) & get_int_bit(n, 3)) ^ get_int_bit(n, 0)
def fb(n: int):
    return get_int_bit(n, 5) ^ (get_int_bit(n, 4) & get_int_bit(n, 3)) ^ get_int_bit(n, 0)
def fc(n: int):
    return get_int_bit(n, 3) ^ (get_int_bit(n, 4) & get_int_bit(n, 5)) ^ get_int_bit(n, 0)
def fd(n: int):
    return get_int_bit(n, 2) ^ (get_int_bit(n, 4) & get_int_bit(n, 5)) ^ get_int_bit(n, 0)
def sub_row(row: list):  # 行变换
    for i in range(32):
        # 保存最低位
        a0, b0, c0, d0 = get_int_bit(row[0], 0), get_int_bit(row[1], 0), get_int_bit(row[2], 0), get_int_bit(row[3], 0)
        a7 = fa(row[0]) ^ c0 ^ d0
        row[0] = (row[0] >> 1) + (a7 << 7)
        b7 = fb(row[1]) ^ a0 ^ d0
        row[1] = (row[1] >> 1) + (b7 << 7)
        c7 = fc(row[2]) ^ b0 ^ a0
        row[2] = (row[2] >> 1) + (c7 << 7)
        d7 = fd(row[3]) ^ c0 ^ b0
        row[3] = (row[3] >> 1) + (d7 << 7)
    return row
def sub_row_inv(row: list):  # 逆行变换
    for i in range(32):
        f_list = [fa(row[0]), fb(row[1]), fc(row[2]), fd(row[3])]
        a7 = f_list[0] ^ f_list[2] ^ f_list[3]
        row[0] = (row[0] >> 1) + (a7 << 7)
        b7 = f_list[1] ^ f_list[0] ^ f_list[3]
        row[1] = (row[1] >> 1) + (b7 << 7)
        c7 = f_list[1] ^ f_list[2] ^ f_list[0]
        row[2] = (row[2] >> 1) + (c7 << 7)
        d7 = f_list[1] ^ f_list[2] ^ f_list[3]
        row[3] = (row[3] >> 1) + (d7 << 7)
    return row
def bit_spin(n: int):  # 字节内部旋转
    result = 0
    for i in range(8):
        result <<= 1
        result += n & 1
        n >>= 1
    return result
def bit_permutation(state: list):  # 最后矩阵变换
    result = [[0, 0, 0, 0] for i in range(4)]
    for i in range(4):
        for j in range(4):
            result[i][j] = bit_spin(state[3 - i][3 - j])
    return result
def round(state: list, k: list, inv=False):  # 轮函数
    state = add_round_key(state, k)
    for i in range(4):
        if not inv:
            state[i] = sub_row(state[i])  # 行变换
        else:
            state[i] = sub_row_inv(state[i])  # 逆行变换
    state = transpose(state)
    return state
def final_round(state: list, k1: list, k2: list, inv=False):  # 最后一轮
    state = add_round_key(state, k1)
    for i in range(4):
        if not inv:
            state[i] = sub_row(state[i])
        else:
            state[i] = sub_row_inv(state[i])
    state = add_round_key(state, k2)
    state = bit_permutation(state)
    return state
def f(index: int, n: int):  # 密钥生成的f函数
    if 0 <= index <= 3:
        return get_int_bit(n, 1) ^ get_int_bit(n, 3 + index) ^ (get_int_bit(n, 8) & get_int_bit(n, 9))
    else:
        return get_int_bit(n, 15) ^ get_int_bit(n, 6 + index) ^ (get_int_bit(n, 7) & get_int_bit(n, 8))
def to_IS(key):  # 转成IS向量，分量为16比特
    IS = []
    for i in range(8):
        IS.append((key[i * 2] << 8) + key[i * 2 + 1])
    return IS
def IS2state(IS: list):  # IS向量转成state矩阵
    state = []
    for i in range(4):
        mid = []
        for j in range(2):
            n = IS[i * 2 + j]
            mid.append(n >> 8)  # 高八位
            mid.append(n & 0xff)  # 低八位
        state.append(mid)
    return state
def key_expansion(key):  # 密钥拓展算法
    round_key = []  # 轮密钥列表
    IS = to_IS(key)  # 转成IS向量
    round_key.append(IS2state(IS))  # 第一个轮密钥
    for i in range(10):
        IS[0] ^= counter[i] << 8  # 初始化IS[0]
        for j in range(16):
            zero = [get_int_bit(n, 0) for n in IS]  # 存储IS中所有元素的最低位
            for k in range(8):  # 循环右移
                x15 = f(k, IS[k]) ^ zero[(k - 1) % 8]
                IS[k] >>= 1
                IS[k] += x15 << 15
        round_key.append(IS2state(IS))  # 生成轮密钥
    return round_key
def key_expansion_inv(round_key: list):  # 基于加密轮密钥生成解密轮密钥
    round_key_inv = [bit_permutation(round_key[-1])]
    for i in range(1, 10):
        round_key_inv.append(transpose(bit_permutation(round_key[10 - i])))
    round_key_inv.append(bit_permutation(round_key[0]))
    return round_key_inv
def encode(key, message):  # 加密
    state = to_state(message)  # 转成state
    round_key = key_expansion(key)  # 生成轮密钥
    for i in range(9):
        state = round(state, round_key[i])  # 轮函数
    state = final_round(state, round_key[9], round_key[10])  # 最后一轮
    return state
def decode(key, message):  # 解密
    state = to_state(message)  # 转成state
    round_key = key_expansion(key)  # 生成加密轮密钥
    round_key = key_expansion_inv(round_key)  # 生成解密轮密钥
    print(round_key)
    for i in range(9):
        state = round(state, round_key[i], inv=True)  # 逆轮函数
    state = final_round(state, round_key[9], round_key[10], inv=True)  # 最后一轮
    return state
def state2hex(state: list):  # state矩阵转成hex
    hex_str = ''
    for i in state:
        for j in i:
            hex_str += "{:02x}".format(j)
    return hex_str
def padding(message: bytes, n: int = 16):
    l = len(message)
    m = n - l % n
    message += bytes.fromhex("{:02x}".format(m)) * m
    return message
def divide(s: bytes, n: int = 16):  # 分组
    result = []
    l = len(s)
    q = l // n
    r = l % n
    for i in range(q):
        result.append(s[i * n:(i + 1) * n])
    if r != 0:
        result.append(s[-r:])
    return result
def padding_inv(s: bytes):  # 解填充
    n = s[-1]
    return s[:-n]
if __name__ == '__main__':
    #  K = input('请输入密钥文件名\n')
    #  pattern = int(input("请选择模式：1，加密；2，解密\n"))
    #  file = input("请输入文件名：\n")
    #  Message = open(file, 'rb').read()
    #  K = open(K, 'rb').read()
    M = [0xe1,0x0c,0xe7,0x2e,0x06,0xea,0xa7,0x25,0xb8,0x4b,0x5f,0xa6,0x7a,0x73,0xdc,0x9b]
    N = [0xa1,0x43 ,0x83 ,0xe7 ,0x40 ,0x34 ,0x8d, 0xba ,0x5e ,0xd0 ,0x42 ,0x2f ,0x7f ,0x25 ,0x6b ,0xfd]
    K = [0xef,0x06,0x8d,0x86,0x1c,0x48,0xaa,0xc2,0xec,0x8a,0x0c,0x63,0xfb,0xc9,0x4d,0xe6]
    pattern = 0
    if pattern == 1:
        #  out = open('encoded_' + file, 'wb')
        #  Message = padding(Message)
        #  m_list = divide(Message)
        #  for M in m_list:
            #  c = encode(K, M)
            #  c = state2hex(c)
            #  out.write(bytes.fromhex(c))
        c = encode(K, M)
        c = state2hex(c)
        print(c)
    else:
        #  out = open('decoded_' + file, 'wb')
        #  m_list = divide(Message)
        #  p = bytes()
        #  for M in m_list:
            #  c = decode(K, M)
            #  p += bytes.fromhex(state2hex(c))
        #  p = padding_inv(p)
        #  out.write(p)
        c = decode(K, N)
        c = state2hex(c)
        print(c)
'''
key=7d0e7b4757bcfe5895f1d89e8bfd2b80
m=8d340a9963938b74f7a71e9451ab1db8
c=8e16e05d7d9975ca4980cbdf9f1041d8
'''
