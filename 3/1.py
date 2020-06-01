import sys
import copy

def getAllP(tp, vis, lens,cur,res,tmp):
    if cur == lens:
        res.append(copy.deepcopy(tmp))
        return
    for i in range(lens):
        if vis[i] ==0:
            vis[i]=1
            tmp.append(tp[i])
            cur +=1
            getAllP(tp,vis,lens,cur,res,tmp)
            cur -=1
            vis[i] =0
            tmp.pop()

a=[1,2,3,4,5,6]
vis=[0,0,0,0,0,0]
lens=len(a)
res=[]
tmp=[]
getAllP(a,vis,lens,0,res,tmp)
print(res)
