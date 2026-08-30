import time
import cv2
import numpy as np
n=800
m=800
circle = np.zeros((n+1, m+1, 3), dtype=np.uint8)

r = 0, 0, 255
g = 0, 255, 0
b = 255, 0, 0
def round(x):
    return int(x+.5)
def draw_symmetric_points(h, k, x, y,color):
    points = [
        (h + x, k + y),
        (h - x, k + y),
        (h + x, k - y),
        (h - x, k - y),
        (h + y, k + x),
        (h - y, k + x),
        (h + y, k - x),
        (h - y, k - x)
    ]

    for px, py in points:
        if 0 <= py < m and 0 <= px < n:
            circle[m-py][px] = color
def directMethod(h, k, radius):
    x=0
    y=radius
    while x <= y:
        draw_symmetric_points(h, k, x, round(y), b)
        x += 1
        y=np.sqrt(abs(radius**2-x**2))
        # print(x,y)


def bresenhamsCircle(h, k, radius):

    x = 0
    y = radius
    d = 3 - 2 * radius

    while x <= y:
        draw_symmetric_points(h, k, x, y, g)
        if d < 0:
            d = d + (x<<2) + 6
        else:
            d = d +  ((x - y)<<2) + 10
            y -= 1
        x+=1

       

def midPointCircle(h, k, radius):
    x=0
    y=radius
    p=1-radius
    while x <= y:
        draw_symmetric_points(h, k, x, y, r)
        if p<0:
            p=p+(x<<1)+3
        else:
            p=p+((x-y)<<1)+5
            y-=1
        x+=1






if __name__ == "__main__":
    h=400
    k=400
    radius=300

    st1 = time.perf_counter()
    directMethod(h, k, radius)
    en1 = time.perf_counter()

    st2 = time.perf_counter()
    bresenhamsCircle(h,k,radius)

    en2 = time.perf_counter()

    st3 = time.perf_counter()
    midPointCircle(h, k, radius)

    en3 = time.perf_counter()



    time1 = 1000 * (en1 - st1)
    time2 = 1000 * (en2 - st2)
    time3 = 1000 * (en3 - st3)

    print(f"Circle 1: Direct Method\nTime: {time1:.4f} ms")
    print(f"Circle 2: Bresenhams\nTime: {time2:.4f} ms, {time1/time2:.4f}x faster than Direct Method" )
    print(f"Circle 3: Mid point\nTime: {time3:.4f} ms, {time1/time3:.4f}x faster than Direct Method")


    cv2.imshow("Blue:Direct Method, Green: Bresenhams Method, Red: Mid Point Method", circle)
    cv2.waitKey(0)