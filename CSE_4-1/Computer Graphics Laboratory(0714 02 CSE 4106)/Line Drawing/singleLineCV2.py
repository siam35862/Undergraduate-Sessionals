import time
import cv2
import numpy as np


line=np.zeros((800,800,3),dtype=np.uint8)


r=0,0,255
g=0,255,0
b=255,0,0

def DDA(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1



    x_inc = dx / abs(dy)
    y_inc = dy / abs(dx)
    x = float(x1)
    y = float(y1)
    

    if x_inc >= 1.0:
        for x in range(x1, x2 + 1):
            line[int(x)][int(y)]=r
            y = y + y_inc
    elif x_inc <= -1.0:
        for x in range(x1, x2 - 1, -1):
            line[int(x)][int(y)]=r
            y = y + y_inc
    elif y_inc <= -1.0:
        for y in range(y1, y2 - 1,-1):
            line[int(x)][int(y)]=r
            x = x + x_inc
    elif y_inc >= 1.0:
        for y in range(y1, y2 + 1):
            line[int(x)][int(y)]=r
            x = x + x_inc

def DDA2(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1



    x_inc = int(dx / abs(dy))
    y_inc = int(dy / abs(dx))

    x=x1
    y=y1
    if x_inc == 1:
        for x in range(x1, x2 + 1):
            line[int(x)][int(y)]=g
            y+=y_inc

    elif x_inc == -1:
        for x in range(x1,x2-1,-1):
            line[int(x)][int(y)]=g
            y += y_inc

    elif y_inc==-1:
        for y in range(y1, y2 - 1,-1):
            line[int(x)][int(y)]=g
            x = x + x_inc

    elif y_inc==1:
        for y in range(y1, y2 + 1):
            line[int(x)][int(y)]=g
            x = x + x_inc



def YMXC(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1

    m = dy / dx
    c = y1 - m * x1
    if(abs(dx)>abs(dy)):
        if dx >= 0:
            for x in range(x1,x2+1):
                y = m * x + c
                line[int(x)][int(y)]=b
        else :
            for x in range(x1,x2-1,-1):
                y = m * x + c
                line[int(x)][int(y)]=b
    else:
        if dy>=0:
            for y in range(y1, y2 + 1):
                x = (y-c)/m
                line[int(x)][int(y)]=b
        else:
            for y in range(y1,y2-1,-1):
                x=(y-c)/m
                line[int(x)][int(y)]=b

if __name__ == "__main__":
    x1 = 50
    y1 = 750
    x2 = 700
    y2 = 50

    for i in range(0,800):
        for j in range(0,800):
            line[i][j]=255,255,255
    

    # Measure execution times
    st1 = time.perf_counter()
    YMXC(x1, y1, x2, y2)
    en1 = time.perf_counter()

    st2 = time.perf_counter()
    DDA(x1, y1, x2, y2)
    en2 = time.perf_counter()

    st3 = time.perf_counter()
    # DDA2(x1, y1, x2, y2)
    en3 = time.perf_counter()

    time1 = 1000 * (en1 - st1)
    time2 = 1000 * (en2 - st2)
    time3 = 1000 * (en3 - st3)

    print(f"Line 1: y=mx+c Method\nTime: {time1:.4f} ms")
    print(f"Line 2: DDA Method\nTime: {time2:.4f} ms")
    print(f"Line 3: DDA Integer Method\nTime: {time3:.4f} ms")

    cv2.imshow("Red: DDA Method Green: DDA Cutoff Blue: Direct Method",line)
    cv2.waitKey(0)