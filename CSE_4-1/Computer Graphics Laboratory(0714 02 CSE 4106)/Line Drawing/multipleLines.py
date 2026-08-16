import time
from matplotlib import pyplot as plt
import random
plt.rcParams['agg.path.chunksize'] = 100000
x_coorinates1 = []
y_coorinates1 = []
x_coorinates2 = []
y_coorinates2 = []
x_coorinates3 = []
y_coorinates3 = []


def DDA(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1

    x_inc = dx / abs(dy)
    y_inc = dy / abs(dx)
    x = float(x1)
    y = float(y1)

    if x_inc >= 1:
        for x in range(x1, x2 + 1):
            x_coorinates2.append(x)
            y_coorinates2.append(y)
            y = y + y_inc
    elif x_inc >= -1:
        for x in range(x1, x2 - 1, -1):
            x_coorinates2.append(x)
            y_coorinates2.append(y)
            y = y + y_inc
    elif y_inc >= -1:
        for y in range(y1, y2 - 1, -1):
            x_coorinates2.append(x)
            y_coorinates2.append(y)
            x = x + x_inc
    elif y_inc >= 1:
        for y in range(y1, y2 + 1):
            x_coorinates2.append(x)
            y_coorinates2.append(y)
            x = x + x_inc


def DDA2(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1

    x_inc = int(dx / abs(dy))
    y_inc = int(dy / abs(dx))

    x = x1
    y = y1
    print(x_inc, y_inc)
    if x_inc == 1:
        for x in range(x1, x2 + 1):
            x_coorinates3.append(x)
            y_coorinates3.append(y)

    elif x_inc == -1:
        for x in range(x1, x2 - 1, -1):
            x_coorinates3.append(x)
            y_coorinates3.append(y)

    elif y_inc == -1:
        for y in range(y1, y2 - 1, -1):
            x_coorinates3.append(x)
            y_coorinates3.append(y)

    elif y_inc == 1:
        for y in range(y1, y2 + 1):
            x_coorinates3.append(x)
            y_coorinates3.append(y)


def YMXC(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1

    m = dy / dx
    c = y1 - m * x1
    if (abs(dx) > abs(dy)):
        if dx >= 0:
            for x in range(x1, x2 + 1):
                y = m * x + c
                x_coorinates1.append(x)
                y_coorinates1.append(y)
        else:
            for x in range(x1, x2 - 1, -1):
                y = m * x + c
                x_coorinates1.append(x)
                y_coorinates1.append(y)
    else:
        if dy >= 0:
            for y in range(y1, y2 + 1):
                x = (y - c) / m
                x_coorinates1.append(x)
                y_coorinates1.append(y)
        else:
            for y in range(y1, y2 - 1, -1):
                x = (y - c) / m
                x_coorinates1.append(x)
                y_coorinates1.append(y)



def generate_lines(n):
    min_val = 0
    max_val = 1000
    min_diff = 500
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


    n=2
    lines = generate_lines(n)
    time1=0
    time2=0
    time3=0

    st1 = time.perf_counter()
    # for line in lines:
    #     YMXC(line[0], line[1], line[2], line[3])

    en1 = time.perf_counter()

    st2 = time.perf_counter()
    # for line in lines:
    #     DDA(line[0], line[1], line[2], line[3])

    en2 = time.perf_counter()

    st3 = time.perf_counter()
    for line in lines:
        DDA2(line[0], line[1], line[2], line[3])

    en3 = time.perf_counter()

    time1 = 1000 * (en1 - st1)
    time2 = 1000 * (en2 - st2)
    time3 = 1000 * (en3 - st3)

    print(f"Line 1: y=mx+c Method\nTime: {time1:.4f} ms")
    print(f"Line 2: DDA Method\nTime: {time2:.4f} ms")
    print(f"Line 3: DDA Integer Method\nTime: {time3:.4f} ms")

    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(18, 6))

    # Display plot 1 with timing
    ax1.plot(
        x_coorinates1,
        y_coorinates1,
        marker="o",
        markersize=1,
        markerfacecolor="green",
    )
    ax1.set_title(f"Line 1: y=mx+c Method\nTime: {time1:.4f} ms \n Total Lines: {len(lines)}")

    # Display plot 2 with timing
    ax2.plot(
        x_coorinates2,
        y_coorinates2,
        marker="o",
        markersize=1,
        markerfacecolor="green",
    )
    ax2.set_title(f"Line 2: DDA Method\nTime: {time2:.4f} ms\n Total Lines: {len(lines)}")

    # Display plot 3 with timing
    ax3.plot(
        x_coorinates3,
        y_coorinates3,
        marker="o",
        markersize=1,
        markerfacecolor="green",
    )
    ax3.set_title(f"Line 3: DDA Integer Method\nTime: {time3:.4f} ms\n Total Lines: {len(lines)}")

    plt.tight_layout()
    plt.show()