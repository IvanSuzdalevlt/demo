#!/usr/bin/python3
# -*- coding: utf-8 -*-

import sys, random, math, numpy
from PyQt5.QtWidgets import QWidget, QApplication
from PyQt5.QtGui import QPainter, QColor, QPen
from PyQt5.QtCore import Qt


class Compas(QWidget):

    cx = 160  #X center coord
    cy = 160  #Y center coord
    
    compasSize = 100 #comas size

    course = 0
    targetCourse = 120
    def __init__(self):
        super().__init__()

        self.initUI()


    def initUI(self):
        
        p = self.palette() #Berem palettu widgeta
        p.setColor(self.backgroundRole(), Qt.black) #Ustanavlivaem palettu na cernyj
        self.setPalette(p) #Meniaem cvet

        self.setGeometry(300, 300, 400, 400)
        self.setWindowTitle('Points')
        self.show()


    def paintEvent(self, e):

        qp = QPainter()
        qp.begin(self)
        self.drawCompas(qp)
        qp.end()

    print(Qt.blue)
    
    def drawCompas(self, qp):

        
        size = self.size()
        #Vnesnee kolco
        for i in numpy.arange(0,360,1):

            pen = QPen(QColor(0, 100 , 0), 2, Qt.SolidLine)
            qp.setPen(pen)
            
            r= self.compasSize;
            x = self.cx + r*math.sin(i*3.1415/180)
            y = self.cy + r*math.cos(i*3.1415/180)
            
            qp.drawPoint(x, y)
        #Vnutrenee kolco
        for i in numpy.arange(0,360,1):
            pen = QPen(QColor(0, 100 , 0), 2, Qt.SolidLine)
            qp.setPen(pen)
     
            r= self.compasSize/1.2;
            x = self.cx + r*math.sin(i*3.1415/180)
            y = self.cy + r*math.cos(i*3.1415/180)
            
            qp.drawPoint(x, y)
            
        #Gradacii melkie
        for i in numpy.arange(0,360,45):

            bW = 180+self.course
            
            pen = QPen(QColor(0, 100 , 0), 2, Qt.SolidLine)
            qp.setPen(pen)

                
            r= self.compasSize/1.50;

            x1 = self.cx + r*math.sin((i-bW)*3.1415/180)
            y1 = self.cy + r*math.cos((i-bW)*3.1415/180)

            r= self.compasSize/2.3;
            x2 = self.cx + r*math.sin((i-bW)*3.1415/180)
            y2 = self.cy + r*math.cos((i-bW)*3.1415/180)
            
            qp.drawLine(x1, y1, x2, y2)

        for i in numpy.arange(0,360,15):

            bW = 180+self.course
            
            pen = QPen(QColor(0, 100 , 0), 2, Qt.SolidLine)
            qp.setPen(pen)

                
            r= self.compasSize/1.50;

            x1 = self.cx + r*math.sin((i-bW)*3.1415/180)
            y1 = self.cy + r*math.cos((i-bW)*3.1415/180)

            r= self.compasSize/1.8;
            x2 = self.cx + r*math.sin((i-bW)*3.1415/180)
            y2 = self.cy + r*math.cos((i-bW)*3.1415/180)
            
            qp.drawLine(x1, y1, x2, y2)
            
        #Oboznacenija
   
        for i in numpy.arange(0,360,90):

            bW = 90+self.course
            
            if i==0:
                pen = QPen(QColor(200, 0 , 0), 5, Qt.SolidLine)
            if i==90:
                pen = QPen(QColor(200, 200 , 0), 5, Qt.SolidLine)
            if i==180:
                pen = QPen(QColor(0, 0 , 200), 5, Qt.SolidLine)
            if i==270:
                pen = QPen(QColor(200, 0 ,200), 5, Qt.SolidLine)
            qp.setPen(pen)

                
            r= self.compasSize/1.50;

            x1 = self.cx + r*math.sin((i-bW)*3.1415/180)
            y1 = self.cy + r*math.cos((i-bW)*3.1415/180)

            r= self.compasSize/1.8;
            x2 = self.cx + r*math.sin((i-bW)*3.1415/180)
            y2 = self.cy + r*math.cos((i-bW)*3.1415/180)
            
            qp.drawLine(x1, y1, x2, y2)
        

            


        #Target

        bW = 180-self.targetCourse+self.course            
        pen = QPen(QColor(150, 0 , 0), 7, Qt.SolidLine)
        qp.setPen(pen)
                
        r= self.compasSize/1.12;

        x1 = self.cx + r*math.sin((bW)*3.1415/180)
        y1 = self.cy + r*math.cos((bW)*3.1415/180)

        r= self.compasSize/1.07;
        x2 = self.cx + r*math.sin((bW)*3.1415/180)
        y2 = self.cy + r*math.cos((bW)*3.1415/180)
            
        qp.drawLine(x1, y1, x2, y2)

        #arrow
        pen = QPen(QColor(150, 0 , 0), 1, Qt.SolidLine)
        qp.setPen(pen)

        r= self.compasSize*0.6;


        x1 = self.cx
        y1 = self.cy + r*0.7

        x2 = self.cx
        y2 = self.cy - r*0.7

        qp.drawLine(x1, y1, x2, y2)

        x1 = self.cx
        y1 = self.cy - r*0.7

        x2 = self.cx + r*0.1
        y2 = self.cy - r*0.5

        qp.drawLine(x1, y1, x2, y2)

        x1 = self.cx
        y1 = self.cy - r*0.7

        x2 = self.cx - r*0.1
        y2 = self.cy - r*0.5

        qp.drawLine(x1, y1, x2, y2)

        #rect

        pen = QPen(QColor(255, 255 , 255), 1, Qt.SolidLine)
        qp.setPen(pen)
                

        x1 = self.cx - 120
        y1 = self.cy - 120
        
        x2 = self.cx - 120
        y2 = self.cy +120
            
        qp.drawLine(x1, y1, x2, y2)

        x1 = self.cx + 120
        y1 = self.cy - 120
        
        x2 = self.cx + 120
        y2 = self.cy +120
            
        qp.drawLine(x1, y1, x2, y2)

        x1 = self.cx - 120
        y1 = self.cy - 120
        
        x2 = self.cx + 120
        y2 = self.cy -120
            
        qp.drawLine(x1, y1, x2, y2)

        x1 = self.cx - 120
        y1 = self.cy + 120
        
        x2 = self.cx + 120
        y2 = self.cy +120
            
        qp.drawLine(x1, y1, x2, y2)

        
        

if __name__ == '__main__':

    app = QApplication(sys.argv)
    compas = Compas()
    sys.exit(app.exec_())
