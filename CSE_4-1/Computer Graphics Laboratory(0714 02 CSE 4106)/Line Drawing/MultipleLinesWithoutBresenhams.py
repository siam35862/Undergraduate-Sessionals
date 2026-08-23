import time
import cv2
import numpy as np
import random
line = np.zeros((910, 910, 3), dtype=np.uint8)

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
    n = 5000
    lines = generate_lines(n)
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



    time1 = 1000 * (en1 - st1)
    time2 = 1000 * (en2 - st2)


    print(f"Line 1: y=mx+c Method\nTime: {time1:.4f} ms")
    print(f"Line 2: DDA Method\nTime: {time2:.4f} ms")


    cv2.imshow("Red: DDA Method, Blue: Direct Method", line)
    cv2.waitKey(0)