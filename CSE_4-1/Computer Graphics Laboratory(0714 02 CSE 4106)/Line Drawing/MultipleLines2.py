import time
import random
import numpy as np
from matplotlib import pyplot as plt

plt.rcParams['agg.path.chunksize'] = 100000

x_coorinates1 = []
y_coorinates1 = []
x_coorinates2 = []
y_coorinates2 = []
x_coorinates3 = []
y_coorinates3 = []
x_coorinates4 = []
y_coorinates4 = []

# --- YOUR ORIGINAL FUNCTIONS (UNTOUCHED) ---
def DDA(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    steps = max(abs(dx), abs(dy))
    x_inc = dx / steps
    y_inc = dy / steps
    x = float(x1)
    y = float(y1)
    for i in range(steps):
        x_coorinates2.append(x)
        y_coorinates2.append(y)
        x = x + x_inc
        y = y + y_inc

def DDA2(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    steps = max(abs(dx), abs(dy))
    x_inc = int(dx / steps)
    y_inc = int(dy / steps)
    x = float(x1)
    y = float(y1)
    for i in range(steps):
        x_coorinates3.append(x)
        y_coorinates3.append(y)
        x = x + x_inc
        y = y + y_inc

def YMXC(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    m = dy / dx
    c = y1 - m * x1
    if(abs(dx)>abs(dy)):
        for x in range(min(x1,x2), max(x2,x1) + 1):
            y = m * x + c
            x_coorinates1.append(x)
            y_coorinates1.append(y)
    else:
        for y in range(min(y1,y2), max(y2,y1) + 1):
            x = (y-c)/m
            x_coorinates1.append(x)
            y_coorinates1.append(y)

# --- NEW: VECTORIZED DDA USING NUMPY ---
def DDA_numpy(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    steps = max(abs(dx), abs(dy))
    if steps == 0:
        x_coorinates4.append(float(x1))
        y_coorinates4.append(float(y1))
        return

    t = np.arange(steps)
    x_vals = x1 + t * (dx / steps)
    y_vals = y1 + t * (dy / steps)

    x_coorinates4.append(x_vals)
    y_coorinates4.append(y_vals)


def generate_lines(n):
    min_val = 0
    max_val = 10000
    min_diff = 5000
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
    return lines


if __name__ == "__main__":
    n = 1000
    lines = generate_lines(n)

    st1 = time.perf_counter()
    for line in lines:
        YMXC(line[0], line[1], line[2], line[3])
    en1 = time.perf_counter()

    st2 = time.perf_counter()
    for line in lines:
        DDA(line[0], line[1], line[2], line[3])
    en2 = time.perf_counter()

    st3 = time.perf_counter()
    for line in lines:
        DDA2(line[0], line[1], line[2], line[3])
    en3 = time.perf_counter()

    st4 = time.perf_counter()
    for line in lines:
        DDA_numpy(line[0], line[1], line[2], line[3])
    # concatenate all per-line arrays into one big array at the end,
    # rather than paying list-append/extend overhead per line
    x_coorinates4_flat = np.concatenate(x_coorinates4)
    y_coorinates4_flat = np.concatenate(y_coorinates4)
    en4 = time.perf_counter()

    time1 = 1000 * (en1 - st1)
    time2 = 1000 * (en2 - st2)
    time3 = 1000 * (en3 - st3)
    time4 = 1000 * (en4 - st4)

    print(f"Line 1: y=mx+c Method\nTime: {time1:.4f} ms")
    print(f"Line 2: DDA Method\nTime: {time2:.4f} ms")
    print(f"Line 3: DDA Integer Method\nTime: {time3:.4f} ms")
    print(f"Line 4: DDA NumPy (vectorized) Method\nTime: {time4:.4f} ms")

    fig, (ax1, ax2, ax3, ax4) = plt.subplots(1, 4, figsize=(24, 6))

    ax1.plot(x_coorinates1, y_coorinates1, marker="o", markersize=1, markerfacecolor="red")
    ax1.set_title(f"Line 1: y=mx+c Method\nTime: {time1:.4f} ms \n Total Lines: {len(lines)}")

    ax2.plot(x_coorinates2, y_coorinates2, marker="o", markersize=1, markerfacecolor="yellow")
    ax2.set_title(f"Line 2: DDA Method\nTime: {time2:.4f} ms\n Total Lines: {len(lines)}")

    ax3.plot(x_coorinates3, y_coorinates3, marker="o", markersize=1, markerfacecolor="black")
    ax3.set_title(f"Line 3: DDA Integer Method\nTime: {time3:.4f} ms\n Total Lines: {len(lines)}")

    ax4.plot(x_coorinates4_flat, y_coorinates4_flat, marker="o", markersize=1, markerfacecolor="blue")
    ax4.set_title(f"Line 4: DDA NumPy Method\nTime: {time4:.4f} ms\n Total Lines: {len(lines)}")

    plt.tight_layout()
    plt.show()