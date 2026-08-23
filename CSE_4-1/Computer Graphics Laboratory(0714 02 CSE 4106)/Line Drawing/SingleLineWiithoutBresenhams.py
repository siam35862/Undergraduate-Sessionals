import time
import cv2
import numpy as np

line = np.zeros((800, 800, 3), dtype=np.uint8)

r = 0, 0, 255
g = 0, 255, 0
b = 255, 0, 0


def YMXC(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1

    m = dy / dx
    c = y1 - m * x1
    if (abs(dx) > abs(dy)):
        if dx >= 0:
            for x in range(x1, x2 + 1):
                y = m * x + c
                line[int(y)][int(x)] = b
        else:
            for x in range(x1, x2 - 1, -1):
                y = m * x + c
                line[int(y)][int(x)] = b
    else:
        if dy >= 0:
            for y in range(y1, y2 + 1):
                x = (y - c) / m
                line[int(y)][int(x)] = b
        else:
            for y in range(y1, y2 - 1, -1):
                x = (y - c) / m
                line[int(y)][int(x)] = b


def DDA(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1

    x_inc = dx / abs(dy)
    y_inc = dy / abs(dx)
    x = float(x1)
    y = float(y1)

    if x_inc >= 1.0:
        for x in range(x1, x2 + 1):
            line[int(y)][int(x)] = r
            y = y + y_inc
    elif x_inc <= -1.0:
        for x in range(x1, x2 - 1, -1):
            line[int(y)][int(x)] = r
            y = y + y_inc
    elif y_inc <= -1.0:
        for y in range(y1, y2 - 1, -1):
            line[int(y)][int(x)] = r
            x = x + x_inc
    elif y_inc >= 1.0:
        for y in range(y1, y2 + 1):
            line[int(y)][int(x)] = r
            x = x + x_inc


# 0 to 45 degree
# def Bresenhams(x1,y1,x2,y2):
#     x, y = x1, y1
#     dx, dy = (x2 - x1), (y2 - y1),
#     step_y = 1 if y1 < y2 else -1
#     step_x = 1 if x1 < x2 else -1
#
#     line[y, x] = g
#
#     dT, dS, d = 2 * (dy - dx), 2 * dy, 2 * dy - dx
#     for x in range(x1, x2 + 1, step_x):
#         if d < 0:
#             d = d + dS
#         else:
#             y, d = y + step_y, d + dT
#         line[y][x] = g

# -45 degree to 45 degree
# def Bresenhams(x1,y1,x2,y2):
#     x, y = x1, y1
#     dx, dy = abs(x2 - x1), abs(y2 - y1),
#     step_y = 1 if y1 < y2 else -1
#     step_x = 1 if x1 < x2 else -1
#
#     line[y, x] = g
#
#     dT, dS, d = 2 * (dy - dx), 2 * dy, 2 * dy - dx
#     for x in range(x1, x2 + 1, step_x):
#         if d < 0:d = d + dS
#         else:
#             y, d = y + step_y, d + dT
#         line[y][x] = g
# -180 to 180 degree
def Bresenhams(x1, y1, x2, y2):
    x, y = x1, y1
    dx, dy = abs(x2 - x1), abs(y2 - y1),
    step_y = 1 if y1 < y2 else -1
    step_x = 1 if x1 < x2 else -1

    line[y, x] = g
    if dx > dy:
        dT, dS, d = 2 * (dy - dx), 2 * dy, 2 * dy - dx
        for x in range(x1, x2 + 1, step_x):
            if d < 0:
                d = d + dS
            else:
                y, d = y + step_y, d + dT
            line[y][x] = g
    else:
        dT, dS, d = 2 * (dx - dy), 2 * dx, 2 * dx - dy
        for y in range(y1, y2 + 1, step_y):
            if d < 0:
                d = d + dS
            else:
                x, d = x + step_x, d + dT
            line[y][x] = g


if __name__ == "__main__":
    x1 = 50
    y1 = 50
    x2 = 580
    y2 = 650

    # for i in range(0,800):
    #     for j in range(0,800):
    #         line[i][j]=255,255,255

    # Measure execution times
    st1 = time.perf_counter()
    YMXC(x1, y1, x2, y2)
    en1 = time.perf_counter()

    st2 = time.perf_counter()
    DDA(x1, y1, x2, y2)
    en2 = time.perf_counter()

    # st3 = time.perf_counter()
    # Bresenhams(x1, y1, x2, y2)
    # en3 = time.perf_counter()

    time1 = 1000 * (en1 - st1)
    time2 = 1000 * (en2 - st2)
    # time3 = 1000 * (en3 - st3)

    print(f"Line 1: y=mx+c Method\nTime: {time1:.4f} ms")
    print(f"Line 2: DDA Method\nTime: {time2:.4f} ms")
    # print(f"Line 3: Bresenhams Method\nTime: {time3:.4f} ms")

    cv2.imshow("Red: DDA Method, Blue: Direct Method", line)
    cv2.waitKey(0)