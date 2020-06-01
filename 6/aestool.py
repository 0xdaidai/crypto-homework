#!/usr/bin/python
# -*- coding: UTF-8 -*-
 
from tkinter import *
from tkinter import ttk
import tkinter.messagebox
import aes
import aes_ecb
import aes_cbc
import aes_cfb
import aes_ofb
import aes_ctr


class Application():
    def __init__(self):
        self.tk = Tk()
        self.tk.title('AEStool')
        self.tk.geometry('750x600')
        self.createWidgets()

    def createWidgets(self):
        self.pInput = Text(self.tk,height = 18,width=100)
        self.cInput = Text(self.tk,height = 18,width=100)
        self.kInput = Entry(self.tk)
        self.IVLabel = Label(self.tk,text='IV',font=('Arial',12))
        self.IVInput = Entry(self.tk)
        self.kLabel = Label(self.tk,text='key',font=('Arial',12))
        self.encodeButton = Button(self.tk, text='encode', command=self.ui_en)
        self.decodeButton = Button(self.tk, text='decode', command=self.ui_de)
        self.outButton = Button(self.tk, text='out', command=self.ui_put)
        self.opt1 = StringVar()
        self.opt2 = StringVar()
        self.padd = ttk.Combobox(self.tk,width=4,state='readonly',font=('Helvetica',12),textvariable = self.opt1)
        self.f_t = ttk.Combobox(self.tk,width=4,state='readonly',font=('Helvetica',12),textvariable = self.opt2)
        self.padd['values'] = ('ECB','CBC','CFB','OFB','CTR')
        self.f_t['values'] = ('Text','File')
        self.padd.current(0)
        self.f_t.current(0)

        self.pInput.place(x=20,y=0)
        self.cInput.place(x=20,y=320)
        self.encodeButton.place(x=540,y=280)
        self.decodeButton.place(x=620,y=280)
        self.kInput.place(x=40,y=282)
        self.kLabel.place(x=10,y=282)
        self.IVInput.place(x=250,y=282)
        self.IVLabel.place(x=220,y=282)
        self.padd.place(x=420,y=282)
        self.f_t.place(x=480,y=282)
        self.outButton.place(x=700,y=280)

    def ui_en(self):
        p = self.pInput.get(0.0,'end').strip()
        k = self.kInput.get().strip()
        IV = self.IVInput.get().strip()
        pat = self.opt1.get()
        self.cInput.delete(0.0,'end')
        if self.opt2.get() == 'Text':
            self.c= self.encode(bytes(p,encoding='utf'),k,IV,pat)
        else:
            try:
                fp = open(p,'rb')
            except IOError:
                tkinter.messagebox.showerror('Error','Wrong file name!')
            else:
                pb = fp.read()
                self.c = self.encode(pb,k,IV,pat)
        self.cInput.insert('insert',self.c)

    def ui_de(self):
        p = self.cInput.get(0.0,'end').strip()
        k = self.kInput.get().strip()
        IV = self.IVInput.get().strip()
        pat = self.opt1.get()
        self.pInput.delete(0.0,'end')
        if self.opt2 == 'Text':
            self.c= self.decode(bytes(p,encoding='utf'),k,IV,pat)
        else:
            try:
                fp = open(p,'rb')
            except IOError:
                tkinter.messagebox.showerror('Error','Wrong file name!')
            else:
                pb = fp.read()
                self.c = self.decode(pb,k,IV,pat)
        self.pInput.insert('insert',self.c)

    def ui_put(self):
        fr = open('a.out','wb')
        fr.write(self.c)
        tkinter.messagebox.showinfo('Output','You can take your a.out!')

    def encode(self,p,k,IV,pat):
        key = bytes.fromhex(k)
        if len(key)!=16:
            tkinter.messagebox.showerror('Error','Wrong key length!')
            c = p
        else:
            if pat == 'ECB':
                c=aes_ecb.encode(key.hex(),p)
            elif pat == 'CBC':
                IV_b = bytes.fromhex(IV)
                if len(IV_b)!=16:
                    tkinter.messagebox.showerror('Error','Wrong IV length!')
                else:
                    c=aes_cbc.encode(key.hex(),p,IV_b.hex())
            elif pat == 'CFB':
                IV_b = bytes.fromhex(IV)
                if len(IV_b)!=16:
                    tkinter.messagebox.showerror('Error','Wrong IV length!')
                else:
                    c=aes_cfb.encode(key.hex(),p,IV_b.hex())
            elif pat == 'OFB':
                IV_b = bytes.fromhex(IV)
                if len(IV_b)!=16:
                    tkinter.messagebox.showerror('Error','Wrong IV length!')
                else:
                    c=aes_ofb.encode(key.hex(),p,IV_b.hex())
            elif pat == 'CTR':
                IV_b = bytes.fromhex(IV)
                if len(IV_b)!=16:
                    tkinter.messagebox.showerror('Error','Wrong IV length!')
                else:
                    c=aes_ctr.encode(key.hex(),p,IV_b.hex())
            else:
                c = p
        return c
        

    def decode(self,p,k,IV,pat):
        key = bytes.fromhex(k)
        if len(key)!=16:
            tkinter.messagebox.showerror('Error','Wrong key length!')
            c = p
        else:
            if pat == 'ECB':
                c=aes_ecb.decode(key.hex(),p)
            elif pat == 'CBC':
                IV_b = bytes.fromhex(IV)
                if len(IV_b)!=16:
                    tkinter.messagebox.showerror('Error','Wrong IV length!')
                else:
                    c=aes_cbc.decode(key.hex(),p,IV_b.hex())
            elif pat == 'CFB':
                IV_b = bytes.fromhex(IV)
                if len(IV_b)!=16:
                    tkinter.messagebox.showerror('Error','Wrong IV length!')
                else:
                    c=aes_cfb.decode(key.hex(),p,IV_b.hex())
            elif pat == 'OFB':
                IV_b = bytes.fromhex(IV)
                if len(IV_b)!=16:
                    tkinter.messagebox.showerror('Error','Wrong IV length!')
                else:
                    c=aes_ofb.decode(key.hex(),p,IV_b.hex())
            elif pat == 'CTR':
                IV_b = bytes.fromhex(IV)
                if len(IV_b)!=16:
                    tkinter.messagebox.showerror('Error','Wrong IV length!')
                else:
                    c=aes_ctr.decode(key.hex(),p,IV_b.hex())
            else:
                c = p
        return c

    def mainloop(self):
        self.tk.mainloop()

app = Application()
app.mainloop()
