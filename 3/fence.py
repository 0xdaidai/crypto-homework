def fence_encode(p, skip):
    key = 0
    c=''
    while key < skip:
        for i in range(0, len(p), skip):
            if (i + key) < len(p):
                c=c+p[i+key]
            else: 
                break
        key = key + 1
    return c

def fence_decode(c,skip):
    key=0
    p=''
    c_len=len(c)
    skip=c_len//skip 
    flag=c_len%skip
    if flag!=0: 
        skip=skip+1
    while key < skip:
        i=0
        count=0
        while (i+key)<c_len : 
            p=p+c[i+key]
            if count<flag: 
                i=i+skip 
            else: 
                i=i+skip-1
            count=count+1
        key=key+1
    return p

