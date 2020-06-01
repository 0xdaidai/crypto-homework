#test
import time
from primelist import *
from mod import *
from mod_normal import *
from ojld import *
from crt import *
from miller_rabin import *


while 1: 
    input_string=input("Please input your choice ->  ") 
    time_begin=time.time()
    if input_string=='exit':
        exit()

    elif input_string=='help': 
        print('options: primelist fast_mod mod_normal ojld crt miller_rabin')

    elif input_string =='primelist': 
        x=input("Please enter a number -> ")
        x=int(x)
        #print(prime_list(x))
        prime_list(x)

    elif input_string=='fast_mod':
        a,b,c=map(int,input("Please enter a b c -> ").split())
        print("output: "+str(fast_mod(a,b,c)))
    
    elif input_string=='mod_normal': 
        a,b,c=map(int,input("Please enter three number -> ").split())
        print("output: "+str(mod_normal(a,b,c)))
    
    elif input_string=='ojld':
        a,b=map(int,input("Please enter two number -> ").split())
        c=ojld(a,b)
    
    elif input_string=='crt':
        num_list=list(map(int,input("Please input a1 a2 a3 b1 b2 b3 -> ").split()))
        b=num_list[0:3]
        a=num_list[3:6]
        print("output: "+str(crt(a,b,3)))
    
    elif input_string=='miller_rabin':
        p,n=map(int,input("Please enter p and times -> ").split())
        for i in range(1,n+1):
            print("This is my "+str(i)+" try!")
            temp = miller_rabin(p)
            if(temp < 0): 
                print("Oh no")
                break
            else: 
                print("The result is "+str(temp))
    
    else:
        print("Wrong input!")

    time_end=time.time()
    print("Running time -> %.8f" %(time_end - time_begin))
