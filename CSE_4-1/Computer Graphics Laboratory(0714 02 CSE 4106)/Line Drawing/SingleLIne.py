import time
from matplotlib import pyplot as plt

x_coorinates1 = []
y_coorinates1 = []
x_coorinates2 = []
y_coorinates2 = []
x_coorinates3 = []
y_coorinates3 = []


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


# --- MAIN EXECUTION WITH DYNAMIC TITLE TIMING ---
if __name__ == "__main__":
    x1 = 50
    y1 = 860
    x2 = 850
    y2 = 70

    # Measure execution times
    st1 = time.perf_counter()
    YMXC(x1, y1, x2, y2)
    en1 = time.perf_counter()

    st2 = time.perf_counter()
    DDA(x1, y1, x2, y2)
    en2 = time.perf_counter()

    st3 = time.perf_counter()
    DDA2(x1, y1, x2, y2)
    en3 = time.perf_counter()

    time1 = 1000 * (en1 - st1)
    time2 = 1000 * (en2 - st2)
    time3 = 1000 * (en3 - st3)

    print(f"Line 1: y=mx+c Method\nTime: {time1:.4f} ms")
    print(f"Line 2: DDA Method\nTime: {time2:.4f} ms")
    print(f"Line 3: DDA Integer Method\nTime: {time3:.4f} ms")

    fig, (ax1, ax2, ax3) = plt.subplots(1, 3, figsize=(12, 4))

    # Display plot 1 with timing
    ax1.plot(
        x_coorinates1,
        y_coorinates1,
        marker="o",
        markersize=1,
        markerfacecolor="green",
    )
    ax1.set_title(f"Line 1: y=mx+c Method\nTime: {time1:.4f} ms")

    # Display plot 2 with timing
    ax2.plot(
        x_coorinates2,
        y_coorinates2,
        marker="o",
        markersize=1,
        markerfacecolor="green",
    )
    ax2.set_title(f"Line 2: DDA Method\nTime: {time2:.4f} ms")

    # Display plot 3 with timing
    ax3.plot(
        x_coorinates3,
        y_coorinates3,
        marker="o",
        markersize=1,
        markerfacecolor="green",
    )
    ax3.set_title(f"Line 3: DDA Integer Method\nTime: {time3:.4f} ms")

    plt.tight_layout()
    plt.show()