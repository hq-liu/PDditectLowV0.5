# -*- coding: gbk -*-  
  
import wx  
  
import numpy as np  
  
import matplotlib  
  
# matplotlib采用WXAgg为后台,将matplotlib嵌入wxPython中  
matplotlib.use("WXAgg")  
  
from matplotlib.backends.backend_wxagg import FigureCanvasWxAgg as FigureCanvas  
from matplotlib.backends.backend_wxagg import NavigationToolbar2WxAgg as NavigationToolbar  
from matplotlib.ticker import MultipleLocator, FuncFormatter  
  
import pylab  
from matplotlib import pyplot
import random
import threading
import time
import sys
import binascii
from socket import *



lineDx=[]
lineDy=[]
lineALL=[]

realtimeX=[]
realtimeY=[]
  
class LineBuilder:
    def __init__(self, line,NavigationToolbarButton):
        self.line = line
        self.xs = list(line.get_xdata())
        self.ys = list(line.get_ydata())
        self.cid = line.figure.canvas.mpl_connect('button_press_event', self)
        self.NavigationToolbarButton=NavigationToolbarButton
        self.count=-1
        
       
    def __call__(self, event):
        #print('click', event)
        if self.NavigationToolbarButton._active != None: return
        if event.inaxes!=self.line.axes: return
        self.xs.append(event.xdata)
        self.ys.append(event.ydata)
        if(self.xs[0]==0):
            del(self.xs[0])
            del(self.ys[0])
            lineDx.append([])
            lineDy.append([])
            self.count=self.count+1
        self.line.set_data(self.xs, self.ys)
        self.line.figure.canvas.draw()
        lineDx[self.count].append(event.xdata)
        lineDy[self.count].append(event.ydata)
        print lineDx
        print lineDy
  
  
######################################################################################  
class MPL_Panel_base(wx.Panel):  
    ''''' #MPL_Panel_base面板,可以继承或者创建实例'''  
    def __init__(self,parent):  
        wx.Panel.__init__(self,parent=parent, id=-1)  
  
        self.Figure = matplotlib.figure.Figure(figsize=(4,3))  
        self.axes = self.Figure.add_axes([0.1,0.1,0.8,0.8])
        self.FigureCanvas = FigureCanvas(self,-1,self.Figure)
        self.NavigationToolbar = NavigationToolbar(self.FigureCanvas)
        self.line, = self.axes.plot([0], [0])
        self.linebuilder=LineBuilder(self.line,self.NavigationToolbar)

        #self.line2, = self.axes.plot([0], [0])
        #self.linebuilder2=LineBuilder(self.line2,self.NavigationToolbar)


        #self.FigureCanvas.Bind(wx.EVT_MOTION, self.OnPaintMotion)
        
        
        #print self.NavigationToolbar.buttonStatus
        #print '**********'
  
        self.StaticText = wx.StaticText(self,-1,label='Show Help String')  
  
        self.SubBoxSizer = wx.BoxSizer(wx.HORIZONTAL)  
        self.SubBoxSizer.Add(self.NavigationToolbar,proportion =0, border = 2,flag = wx.ALL | wx.EXPAND)  
        self.SubBoxSizer.Add(self.StaticText,proportion =-1, border = 2,flag = wx.ALL | wx.EXPAND)  
  
        self.TopBoxSizer = wx.BoxSizer(wx.VERTICAL)  
        self.TopBoxSizer.Add(self.SubBoxSizer,proportion =-1, border = 2,flag = wx.ALL | wx.EXPAND)  
        self.TopBoxSizer.Add(self.FigureCanvas,proportion =-10, border = 2,flag = wx.ALL | wx.EXPAND)  
  
        self.SetSizer(self.TopBoxSizer)

        self.xlim(x_min=0,x_max=80000)
        self.ylim(y_min=-100,y_max=500)
  
        ###方便调用  
        self.pylab=pylab  
        self.pl=pylab  
        self.pyplot=pyplot  
        self.numpy=np  
        self.np=np  
        self.plt=pyplot

        
    def OnPaintMotion(self,event):
        #self.StaticText.SetLabel(u"鼠标位置：" + str(event.GetPositionTuple()))
        #print self.NavigationToolbar._active
        pass
        
  
    def UpdatePlot(self):  
        '''''#修改图形的任何属性后都必须使用self.UpdatePlot()更新GUI界面 '''  
        self.FigureCanvas.draw()  
  
  
    def plot(self,*args,**kwargs):  
        '''''#最常用的绘图命令plot '''  
        self.axes.plot(*args,**kwargs)  
        self.UpdatePlot()  
  
  
    def semilogx(self,*args,**kwargs):  
        ''''' #对数坐标绘图命令 '''  
        self.axes.semilogx(*args,**kwargs)  
        self.UpdatePlot()  
  
    def semilogy(self,*args,**kwargs):  
        ''''' #对数坐标绘图命令 '''  
        self.axes.semilogy(*args,**kwargs)  
        self.UpdatePlot()  
  
    def loglog(self,*args,**kwargs):  
        ''''' #对数坐标绘图命令 '''  
        self.axes.loglog(*args,**kwargs)  
        self.UpdatePlot()  
  
  
    def grid(self,flag=True):  
        ''''' ##显示网格  '''  
        if flag:  
            self.axes.grid()  
        else:  
            self.axes.grid(False)  
  
  
    def title_MPL(self,TitleString="wxMatPlotLib Example In wxPython"):  
        ''''' # 给图像添加一个标题   '''  
        self.axes.set_title(TitleString)  
  
  
    def xlabel(self,XabelString="X"):  
        ''''' # Add xlabel to the plotting    '''  
        self.axes.set_xlabel(XabelString)  
  
  
    def ylabel(self,YabelString="Y"):  
        ''''' # Add ylabel to the plotting '''  
        self.axes.set_ylabel(YabelString)  
  
  
    def xticker(self,major_ticker=1.0,minor_ticker=0.1):  
        ''''' # 设置X轴的刻度大小 '''  
        self.axes.xaxis.set_major_locator( MultipleLocator(major_ticker) )  
        self.axes.xaxis.set_minor_locator( MultipleLocator(minor_ticker) )  
  
  
    def yticker(self,major_ticker=1.0,minor_ticker=0.1):  
        ''''' # 设置Y轴的刻度大小 '''  
        self.axes.yaxis.set_major_locator( MultipleLocator(major_ticker) )  
        self.axes.yaxis.set_minor_locator( MultipleLocator(minor_ticker) )  
  
  
    def legend(self,*args,**kwargs):  
        ''''' #图例legend for the plotting  '''  
        self.axes.legend(*args,**kwargs)  
  
  
    def xlim(self,x_min,x_max):  
        ''''' # 设置x轴的显示范围  '''  
        self.axes.set_xlim(x_min,x_max)  
  
  
    def ylim(self,y_min,y_max):  
        ''''' # 设置y轴的显示范围   '''  
        self.axes.set_ylim(y_min,y_max)  
  
  
    def savefig(self,*args,**kwargs):  
        ''''' #保存图形到文件 '''  
        self.Figure.savefig(*args,**kwargs)  
  
  
    def cla(self):  
        ''''' # 再次画图前,必须调用该命令清空原来的图形  '''
        self.axes.clear()  
        #self.Figure.set_canvas(self.FigureCanvas)  
        self.UpdatePlot()  
          
    def ShowHelpString(self,HelpString="Show Help String"):  
        ''''' #可以用它来显示一些帮助信息,如鼠标位置等 '''  
        self.StaticText.SetLabel(HelpString)  
  
################################################################  
  
###############################################################################  
#  MPL_Frame添加了MPL_Panel的1个实例  
###############################################################################  
class MPL_Frame(wx.Frame):  
    """MPL_Frame可以继承,并可修改,或者直接使用"""  
    def __init__(self,title="MPL_Frame Example In wxPython",size=(800,500)):  
        wx.Frame.__init__(self,parent=None,title = title,size=size)  
  
        self.MPL = MPL_Panel_base(self)  
  
        #创建FlexGridSizer  
        self.FlexGridSizer=wx.FlexGridSizer( rows=9, cols=1, vgap=5,hgap=5)  
        self.FlexGridSizer.SetFlexibleDirection(wx.BOTH)  
  
        self.RightPanel = wx.Panel(self,-1)

        self.algorithm=[]

        #下拉框
        #self.Choice1=wx.Choice(self.RightPanel, -1, choices=self.algorithm, size=(100,40),pos=(10,10))
        #self.Bind(wx.EVT_CHOICE,self.chooseScoreFunc, self.Choice1)

  
        #测试按钮1  
        self.Btn_SET = wx.Button(self.RightPanel,-1,"SET",size=(100,40),pos=(10,10))  
        self.Btn_SET.Bind(wx.EVT_BUTTON,self.Btn_SET_Event)  
  
        #测试按钮2  
        self.Btn_RUN = wx.Button(self.RightPanel,-1,"RUN",size=(100,40),pos=(10,10))  
        self.Btn_RUN.Bind(wx.EVT_BUTTON,self.Btn_RUN_Event)

        #测试按钮3  
        self.Button3 = wx.Button(self.RightPanel,-1,"OFF",size=(100,40),pos=(10,10))  
        #self.Button3.Bind(wx.EVT_BUTTON,self.Button3Event)

        #测试按钮4  
        self.Button4 = wx.Button(self.RightPanel,-1,"DEUBG",size=(100,40),pos=(10,10))  
        #self.Button4.Bind(wx.EVT_BUTTON,self.Button3Event)

        #测试按钮5  
        self.Button5 = wx.Button(self.RightPanel,-1,"GET ALL",size=(100,40),pos=(10,10))  
        #self.Button4.Bind(wx.EVT_BUTTON,self.Button3Event) 
  
        #加入Sizer中
        
        self.FlexGridSizer.Add(self.Btn_SET,proportion =0, border = 5,flag = wx.ALL | wx.EXPAND)
        #self.FlexGridSizer.Add(self.Choice1,proportion =0, border = 5,flag = wx.ALL | wx.EXPAND)
        self.FlexGridSizer.Add(self.Btn_RUN,proportion =0, border = 5,flag = wx.ALL | wx.EXPAND)
        self.FlexGridSizer.Add(self.Button3,proportion =0, border = 5,flag = wx.ALL | wx.EXPAND)
        self.FlexGridSizer.Add(self.Button4,proportion =0, border = 5,flag = wx.ALL | wx.EXPAND)  
        self.FlexGridSizer.Add(self.Button5,proportion =0, border = 5,flag = wx.ALL | wx.EXPAND)  
        


        
        self.RightPanel.SetSizer(self.FlexGridSizer)  
          
        self.BoxSizer=wx.BoxSizer(wx.HORIZONTAL)  
        self.BoxSizer.Add(self.MPL,proportion =-10, border = 1,flag = wx.ALL | wx.EXPAND)  
        self.BoxSizer.Add(self.RightPanel,proportion =0, border = 1,flag = wx.ALL | wx.EXPAND)  
          
        self.SetSizer(self.BoxSizer)      
  
        #状态栏  
        self.StatusBar()  
  
        #MPL_Frame界面居中显示  
        self.Centre(wx.BOTH)  
  
    def chooseScoreFunc(self,event):
        pass
  
    #按钮事件,用于测试
    def Btn_SET_Event(self,event):
        del self.MPL.linebuilder.xs[:]
        del self.MPL.linebuilder.ys[:]
        self.MPL.linebuilder.xs.append(0)
        self.MPL.linebuilder.ys.append(0)
        #self.MPL.line, = self.MPL.axes.plot([0], [0])
        #self.MPL.linebuilder=LineBuilder(self.MPL.line,self.MPL.NavigationToolbar)
        algorithm_count=len(lineDx)/2
        if algorithm_count>0 and algorithm_count%2==0:
            self.algorithm.append("参数"+str(algorithm_count))
        print self.algorithm
       
  
    def Btn_RUN_Event(self,event):  
        #self.AboutDialog()
        frush=threading.Thread(target=self.Refrush)
        frush.setDaemon(False)
        frush.start()
        
        
    def Refrush1(self):
        while(True):
            self.MPL.cla()#必须清理图形,才能显示下一幅图
            self.MPL.xlim(x_min=0,x_max=80000)
            self.MPL.ylim(y_min=-100,y_max=500)
            x=np.arange(0,80)  
            y=np.cos(x)*100*random.random()+100 
            self.MPL.plot(x,y,'--*g')
            self.MPL.plot(self.MPL.linebuilder.xs,self.MPL.linebuilder.ys)
            print self.MPL.plot
            #self.MPL.xticker(2.0,0.5)  
            #self.MPL.yticker(0.5,0.1)  
            #self.MPL.title_MPL("MPL1")  
            self.MPL.ShowHelpString("You Can Show MPL Helpful String Here !")  
            #self.MPL.grid()
            self.MPL.UpdatePlot()#必须刷新才能显示
            time.sleep(1)

    def Refrush(self):
        for i in range(len(lineDx)):
            self.MPL.line,=self.MPL.axes.plot(lineDx[i],lineDy[i])
            lineALL.append(self.MPL.line)
        self.MPL.grid()
        i=0
        while(True):
            #realtimeX=np.arange(40,60)
            #realtimeY=np.cos(realtimeX)*100*random.random()+100 
            self.MPL.line,=self.MPL.axes.plot(realtimeX,realtimeY)
            lineALL.append(self.MPL.line)
            for line in lineALL:
                line.figure.canvas.draw()
            lineALL[len(lineALL)-1].remove()
            del lineALL[len(lineALL)-1]
            #print lineD
            time.sleep(0.03)
            i=i+1
            #self.MPL.StaticText.SetLabel(str(i))

        
  
  
    #打开文件,用于测试  
    def DoOpenFile(self):  
        wildcard = r"Data files (*.dat)|*.dat|Text files (*.txt)|*.txt|ALL Files (*.*)|*.*"  
        open_dlg = wx.FileDialog(self,message='Choose a file',wildcard = wildcard, style=wx.OPEN|wx.CHANGE_DIR)  
        if open_dlg.ShowModal() == wx.ID_OK:  
            path=open_dlg.GetPath()  
            try:  
                file = open(path, 'r')  
                text = file.read()  
                file.close()  
            except IOError, error:  
                dlg = wx.MessageDialog(self, 'Error opening file\n' + str(error))  
                dlg.ShowModal()  
  
        open_dlg.Destroy()  
  
  
  
    #自动创建状态栏  
    def StatusBar(self):  
        self.statusbar = self.CreateStatusBar()  
        self.statusbar.SetFieldsCount(3)  
        self.statusbar.SetStatusWidths([-2, -2, -1])  
  
  
    #About对话框  
    def AboutDialog(self):  
        dlg = wx.MessageDialog(self, '\twxMatPlotLib\t\nMPL_Panel_base,MPL_Panel,MPL_Frame and MPL2_Frame \n Created by Wu Xuping\n Version 1.0.0 \n 2012-02-01',  
                                'About MPL_Frame and MPL_Panel', wx.OK | wx.ICON_INFORMATION)  
        dlg.ShowModal()  
        dlg.Destroy()  

HOST = ''
PORT = 5000
BUFSIZE = 2048
ADDR =(HOST,PORT)

#创建服务端udp socket
udpServer = socket(AF_INET,SOCK_DGRAM)
#绑定服务器 socket
udpServer.bind(ADDR)

def udpRecv():
    while True:
        global realtimeX
        global realtimeY
        data,addr = udpServer.recvfrom(BUFSIZE)
        sdata= binascii.b2a_hex(data)
        l=[sdata[i:i+4] for i in xrange(0,len(sdata),4)]
        
        l=map(lambda x:int(x,16),l)
        print len(l)
        realtimeY=map(lambda x:float(x)/16383*416,l)
        #print realtimeY
        realtimeX=[x for x in range(10000,10000+200*len(realtimeY),200)]
        print len(realtimeX)
        time.sleep(0.05)

  
th_udpRecv=threading.Thread(target=udpRecv) 
#主程序测试  
if __name__ == '__main__':
    th_udpRecv.setDaemon(True)
    th_udpRecv.start()
    app = wx.PySimpleApp()  
    #frame = MPL2_Frame()  
    frame =MPL_Frame()  
    frame.Center()  
    frame.Show()  
    app.MainLoop()  
