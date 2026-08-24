import time
import cv2
import numpy as np
import random
n,m=900,900
line=np.zeros((n+1,m+1,3),dtype=np.uint8)

r = 0, 0, 255
g = 0, 255, 0
b = 255, 0, 0
def round(x):
    return int(x+.5)
def YMXC(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1

    m = dy / dx
    c = y1 - m * x1
    if(abs(dx)>abs(dy)):
        if dx >= 0:
            for x in range(x1,x2+1):
                y = m * x + c
                line[n-round(y)][round(x)]=b
        else :
            for x in range(x1,x2-1,-1):
                y = m * x + c
                line[n-round(y)][round(x)]=b
    else:
        if dy>=0:
            for y in range(y1, y2 + 1):
                x = (y-c)/m
                line[n-round(y)][round(x)]=b
        else:
            for y in range(y1,y2-1,-1):
                x=(y-c)/m
                line[n-round(y)][round(x)]=b



def DDA(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1

    x_inc = dx / abs(dy)
    y_inc = dy / abs(dx)
    x = float(x1)
    y = float(y1)

    if x_inc >= 1.0:
        for x in range(x1, x2 + 1):
            line[n-round(y)][round(x)] = r
            y = y + y_inc

    elif x_inc <= -1.0:
        for x in range(x1, x2 - 1, -1):
            line[n-round(y)][round(x)] = r
            y = y + y_inc
    elif y_inc <= -1.0:
        for y in range(y1, y2 - 1, -1):
            line[n-round(y)][round(x)] = r
            x = x + x_inc
    elif y_inc >= 1.0:
        for y in range(y1, y2 + 1):
            line[n-round(y)][round(x)] = r
            x = x + x_inc

def Bresenhams(x1,y1,x2,y2):
    x,y=x1,y1
    dx,dy= abs(x2 - x1),abs(y2 - y1),
    step_y=1 if y1<y2 else -1
    step_x=1 if x1<x2 else -1

    line[n-y,x]=g
    if dx>dy:
        dT, dS, d = 2 * (dy - dx), 2 * dy, 2 * dy - dx
        for x in range(x1+step_x,x2+step_x,step_x):
            if d<0:d=d+dS
            else:y,d=y+step_y,d+dT
            line[n-y][x] = g
    else:
        dT, dS, d = 2 * (dx - dy), 2 * dx, 2 * dx - dy
        for y in range(y1+step_y,y2+step_y,step_y):
            if d<0:d=d+dS
            else: x,d=x+step_x,d+dT
            line[n-y][x]=g

def generate_lines(n):
    min_val = 0
    max_val = 900
    min_diff = 200
    lines = []

    while len(lines) < n:
        x1 = random.randint(min_val, max_val)
        y1 = random.randint(min_val, max_val)
        x2 = random.randint(min_val, max_val)
        y2 = random.randint(min_val, max_val)

        dx = abs(x2 - x1)
        dy = abs(y2 - y1)



        if dx >= min_diff and dy >= min_diff:
            lines.append((x1, y1, x2, y2))

    print("Random Lines Generated.")
    return lines

if __name__ == "__main__":
    total_line = 5000
    lines = generate_lines(total_line)
    time1 = 0
    time2 = 0
    time3 = 0

    st1 = time.perf_counter()
    for point in lines:
        YMXC(point[0], point[1], point[2], point[3])

    en1 = time.perf_counter()

    st2 = time.perf_counter()
    for point in lines:
        DDA(point[0], point[1], point[2], point[3])

    en2 = time.perf_counter()

    st3 = time.perf_counter()
    for point in lines:
        Bresenhams(point[0], point[1], point[2], point[3])

    en3 = time.perf_counter()

    time1 = 1000 * (en1 - st1)
    time2 = 1000 * (en2 - st2)
    time3 = 1000 * (en3 - st3)

    print(f"Line 1: y=mx+c Method\nTime: {time1:.4f} ms")
    print(f"Line 2: DDA Method\nTime: {time2:.4f} ms, Factor: {time1/time2:.4f}")

    print(f"Line 3: Bresenhams Method\nTime: {time3:.4f} ms, Factor: {time1/time3:.4f}")

    cv2.imshow("Red: DDA Method, Green: Bresenhams Method, Blue: Direct Method", line)
    cv2.waitKey(0)