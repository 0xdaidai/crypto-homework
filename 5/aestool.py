#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
from tkinter import *
import tkinter.messagebox
import aes

class Application(Frame):
    def __init__(self, master=None):
        Frame.__init__(self, master)
        self.pack()
        self.createWidgets()

    def createWidgets(self):
        self.pInput = Entry(self)
        self.cInput = Entry(self)
        self.kInput = Entry(self)
        self.pLabel = Label(self,text = 'p')
        self.cLabel = Label(self,text = 'c')
        self.kLabel = Label(self,text = 'key')
        self.encodeButton = Button(self, text='encode', command=self.ui_en)
        self.decodeButton = Button(self, text='decode', command=self.ui_de)
        self.refreshButton = Button(self,text='refresh',command=self.f_re)


        self.pLabel.grid(row=0,column = 0)
        self.cLabel.grid(row=1,column = 0)
        self.kLabel.grid(row=2,column = 0)
        self.pInput.grid(row=0,column =1,columnspan=15)
        self.cInput.grid(row=1,column =1,columnspan=15)
        self.kInput.grid(row=2,column =1,columnspan=15)
        self.encodeButton.grid(row = 3,column=2,columnspan=3)
        self.decodeButton.grid(row = 3,column=8,columnspan=3)
        self.refreshButton.grid(row = 3,column=14,columnspan=3)

    def ui_en(self):
        p = self.pInput.get().strip()
        k = self.kInput.get().strip()
        self.cInput.delete(0,END)
        lp = len(p)
        lk = len(k)
        if lp!=32: 
            tkinter.messagebox.showwarning('Waring','The length of p is wrong!')
        else: 
            if lk == 32:
                c = aes.encode(p,k)
                self.cInput.insert(10,str(c))
            elif lk ==48:
                c = aes.encode(p,k,6,12)
                self.cInput.insert(10,str(c))
            elif lk ==64: 
                c = aes.encode(p,k,8,14)
                self.cInput.insert(10,str(c))
            else: 
                tkinter.messagebox.showwarning('Waring','The length of key is wrong!')

    def ui_de(self):
        c = self.cInput.get().strip()
        k = self.kInput.get().strip()
        self.pInput.delete(0,END)
        lc = len(c)
        lk = len(k)
        if lp!=32: 
            tkinter.messagebox.showwarning('Waring','The length of c is wrong!')
        else: 
            if lk == 32:
                p = aes.decode(c,k)
                self.pInput.insert(10,str(p))
            elif lk ==48:
                p = aes.encode(p,k,6,12)
                self.pInput.insert(10,str(p))
            elif lk ==64: 
                p = aes.encode(p,k,8,14)
                self.pInput.insert(10,str(p))
            else: 
                tkinter.messagebox.showwarning('Waring','The length of key is wrong!')

    def f_re(self):
        self.pInput.delete(0,END)
        self.cInput.delete(0,END)
        self.kInput.delete(0,END)

app = Application()
app.master.title('AEStool')
app.mainloop()
