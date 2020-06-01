import math 

def c_mod(a,b): 
    return a - int(a/b)*b


def swap(a,b):
    return b,a


def ojld(a,b):
#最终结果：gcd = m1 * aa + m2 *b 

    flag=0
    if a<b:
        flag=1 
        a,b=swap(a,b)
    aa=a 
    bb=b
    #保存原来的数 

    #处理特殊情况，a = k*b。 
    if c_mod(a,b) == 0:
        print("gcd("+str(aa)+","+str(bb)+")="+str(b)) 
        print("(0) * "+str(aa)+" + (1) * "+str(bb)+" = "+str(b)) 
        #输出结果 
    else:
        m1 = 0
        n1 = 1
        m2 = 1
        n2 = int(-a/b)
        a = c_mod(a,b)
        a,b=swap(a,b)
        #一开始，r1 = a -q1*b,所以 m2 = 1, n2 = -q1 = -a/b而r2 = b - q2*r2 .所以m1 = 0, n1 = 1. 

        while c_mod(a,b):
            mm = m2
            nn = n2
            m2 = m1 - int(a/b)*m2
            n2 = n1 - int(a/b)*n2
            m1 = mm
            n1 = nn#递归的改变 m2 和 n2 的数值，由于 ri+2 = ri - qi*ri+1,所以我用了m1,n1来表示ri，m2,n2表示ri+1 

            #print("m1 = "+str(m1)+",n1 = "+str(n1)+",m2 = "+str(m2)+",n2 = "+str(n2)+",a = "+str(a)+",b = "+str(b))
            a = c_mod(a,b)
            a,b=swap(a,b)
            #使得a为较大的那个数 

        a = b
        if a<0: 
            a=-a 
            m2=-m2 
            n2=-n2 
        print("gcd("+str(aa)+","+str(bb)+")="+str(a)) 
        print("("+str(m2)+") * "+str(aa)+" + ("+str(n2)+") * "+str(bb)+" = "+str(a))
        #输出结果 
        if flag==0: 
            m2=m2%bb
            while m2<0: 
                m2+=abs(bb)
            return m2
        else:
            n2 = n2%aa
            while n2<0: 
                n2+=abs(aa)
            return n2


