import numpy as np
import cv2

def basic_equation(x1, y1, x2, y2, img):
    if x1 == x2 and y1 == y2:
        img[x1, y1] = 255
        return

    n = len(img)

    if abs(x1 - x2) > abs(y1 - y2):
        l = min(x1, x2)
        r = max(x1, x2)
        m = (y1 - y2) / (x1 - x2)
        c = y1 - m * x1

        for x in range(l, r + 1):
            y = int(x * m + c)

            img[n - y, x] = 255
    else:
        l = min(y1, y2)
        r = max(y1, y2)
        m = (x1 - x2) / (y1 - y2)
        c = x1 - m * y1

        for y in range(l, r + 1):
            x = int(y * m + c)

            img[n - y, x] = 255

def dda(x1, y1, x2, y2, img):
    dx = x2 - x1
    dy = y2 - y1
    n = len(img)

    if abs(dx) >= abs(dy):
        m = dy / dx
        y = y1 if x1 < x2 else y2
        l = min(x1, x2)
        r = max(x1, x2)

        for x in range(l, r + 1):
            img[n - int(y), x] = 255

            y += m
    else:
        _m = dx / dy
        x = x1 if y1 < y2 else x2
        l = min(y1, y2)
        r = max(y1, y2)

        for y in range(l, r + 1):
            img[n - y, int(x)] = 255

            x += _m

def dropout_cutoff(x1, y1, x2, y2, img):
    dx = x2 - x1
    dy = y2 - y1
    n = len(img)

    if abs(dx) >= abs(dy):
        m = int(dy / dx)
        y = y1 if x1 < x2 else y2
        l = min(x1, x2)
        r = max(x1, x2)

        for x in range(l, r + 1):
            img[n - y, x] = 255

            y += m
    else:
        _m = int(dx / dy)
        x = x1 if y1 < y2 else x2
        l = min(y1, y2)
        r = max(y1, y2)

        for y in range(l, r + 1):
            img[n - y, x] = 255

            x += _m


if __name__ == "__main__":
    n = 600
    img1 = np.zeros((n, n), dtype=np.uint8)
    img2 = np.zeros((n, n), dtype=np.uint8)
    img3 = np.zeros((n, n), dtype=np.uint8)

    x1, y1 = 510, 10
    x2, y2 = 55, 500

    basic_equation(x1, y1, x2, y2, img1)
    dda(x1, y1, x2, y2, img2)
    dropout_cutoff(x1, y1, x2, y2, img3)

    cv2.imshow("Basic equation: y = mx + c", img1)
    cv2.imshow("DDA: Ynext = Ycurr + m", img2)
    cv2.imshow("Dropout Cutoff: Ynext = Ycurr + int(m)", img3)
    cv2.waitKey(0)