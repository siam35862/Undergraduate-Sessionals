import time
import random
import numpy as np
from matplotlib import pyplot as plt

plt.rcParams['agg.path.chunksize'] = 100000

# ============================================================
# ORIGINAL (pure-Python, loop-based) IMPLEMENTATIONS
# ============================================================
x_coorinates1, y_coorinates1 = [], []   # YMXC
x_coorinates2, y_coorinates2 = [], []   # DDA
x_coorinates3, y_coorinates3 = [], []   # DDA2 (integer increments)


def YMXC(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    m = dy / dx
    c = y1 - m * x1
    if abs(dx) > abs(dy):
        for x in range(min(x1, x2), max(x2, x1) + 1):
            y = m * x + c
            x_coorinates1.append(x)
            y_coorinates1.append(y)
    else:
        for y in range(min(y1, y2), max(y2, y1) + 1):
            x = (y - c) / m
            x_coorinates1.append(x)
            y_coorinates1.append(y)


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


# ============================================================
# OPTIMIZED (vectorized NumPy) IMPLEMENTATIONS
# Each returns (x_array, y_array) for ONE line instead of
# appending to a global Python list one point at a time.
# The Python-level "for i in range(steps)" loop is replaced
# by a single call into NumPy's C loop.
# ============================================================
def YMXC_np(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    m = dy / dx
    c = y1 - m * x1
    if abs(dx) > abs(dy):
        xs = np.arange(min(x1, x2), max(x1, x2) + 1)
        ys = m * xs + c
    else:
        ys = np.arange(min(y1, y2), max(y1, y2) + 1)
        xs = (ys - c) / m
    return xs, ys


def DDA_np(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    steps = max(abs(dx), abs(dy))
    if steps == 0:
        return np.array([float(x1)]), np.array([float(y1)])
    t = np.arange(steps)
    xs = x1 + t * (dx / steps)
    ys = y1 + t * (dy / steps)
    return xs, ys


def DDA2_np(x1, y1, x2, y2):
    dx = x2 - x1
    dy = y2 - y1
    steps = max(abs(dx), abs(dy))
    if steps == 0:
        return np.array([float(x1)]), np.array([float(y1)])
    x_inc = int(dx / steps)
    y_inc = int(dy / steps)
    t = np.arange(steps)
    xs = x1 + t * x_inc      # stays float, matching original's float(x1) + int increments
    ys = y1 + t * y_inc
    return xs.astype(float), ys.astype(float)


def run_vectorized(func, lines):
    """Run a *_np function over all lines and concatenate into one array pair."""
    xs_list, ys_list = [], []
    for (x1, y1, x2, y2) in lines:
        xs, ys = func(x1, y1, x2, y2)
        xs_list.append(xs)
        ys_list.append(ys)
    return np.concatenate(xs_list), np.concatenate(ys_list)


def generate_lines(n):
    min_val = 0
    max_val = 100000
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


def sanity_check(lines, sample=5):
    """Confirm the vectorized versions produce the same points as the originals."""
    for x1, y1, x2, y2 in lines[:sample]:
        xs_o, ys_o = [], []
        dx, dy = x2 - x1, y2 - y1
        m = dy / dx
        c = y1 - m * x1
        if abs(dx) > abs(dy):
            for x in range(min(x1, x2), max(x2, x1) + 1):
                xs_o.append(x)
                ys_o.append(m * x + c)
        else:
            for y in range(min(y1, y2), max(y2, y1) + 1):
                ys_o.append(y)
                xs_o.append((y - c) / m)
        xs_v, ys_v = YMXC_np(x1, y1, x2, y2)
        assert np.allclose(xs_o, xs_v) and np.allclose(ys_o, ys_v), "YMXC mismatch"

        steps = max(abs(dx), abs(dy))
        x, y = float(x1), float(y1)
        x_inc, y_inc = dx / steps, dy / steps
        xs_o2, ys_o2 = [], []
        for _ in range(steps):
            xs_o2.append(x); ys_o2.append(y)
            x += x_inc; y += y_inc
        xs_v2, ys_v2 = DDA_np(x1, y1, x2, y2)
        assert np.allclose(xs_o2, xs_v2) and np.allclose(ys_o2, ys_v2), "DDA mismatch"

        x_inc_i, y_inc_i = int(dx / steps), int(dy / steps)
        x, y = float(x1), float(y1)
        xs_o3, ys_o3 = [], []
        for _ in range(steps):
            xs_o3.append(x); ys_o3.append(y)
            x += x_inc_i; y += y_inc_i
        xs_v3, ys_v3 = DDA2_np(x1, y1, x2, y2)
        assert np.allclose(xs_o3, xs_v3) and np.allclose(ys_o3, ys_v3), "DDA2 mismatch"
    print("Sanity check passed: vectorized outputs match original outputs.\n")


if __name__ == "__main__":
    n = 100
    lines = generate_lines(n)

    sanity_check(lines)

    # ---------------- ORIGINAL (pure Python loops) ----------------
    st1 = time.perf_counter()
    for line in lines:
        YMXC(*line)
    en1 = time.perf_counter()

    st2 = time.perf_counter()
    for line in lines:
        DDA(*line)
    en2 = time.perf_counter()

    st3 = time.perf_counter()
    for line in lines:
        DDA2(*line)
    en3 = time.perf_counter()

    # ---------------- OPTIMIZED (vectorized NumPy) ----------------
    st4 = time.perf_counter()
    x_coorinates1_np, y_coorinates1_np = run_vectorized(YMXC_np, lines)
    en4 = time.perf_counter()

    st5 = time.perf_counter()
    x_coorinates2_np, y_coorinates2_np = run_vectorized(DDA_np, lines)
    en5 = time.perf_counter()

    st6 = time.perf_counter()
    x_coorinates3_np, y_coorinates3_np = run_vectorized(DDA2_np, lines)
    en6 = time.perf_counter()

    time1, time2, time3 = 1000*(en1-st1), 1000*(en2-st2), 1000*(en3-st3)
    time4, time5, time6 = 1000*(en4-st4), 1000*(en5-st5), 1000*(en6-st6)

    print(f"Line 1: y=mx+c Method (original)\nTime: {time1:.4f} ms")
    print(f"Line 2: DDA Method (original)\nTime: {time2:.4f} ms")
    print(f"Line 3: DDA Integer Method (original)\nTime: {time3:.4f} ms\n")

    print(f"Line 1: y=mx+c Method (NumPy vectorized)\nTime: {time4:.4f} ms  ({time1/time4:.1f}x faster)")
    print(f"Line 2: DDA Method (NumPy vectorized)\nTime: {time5:.4f} ms  ({time2/time5:.1f}x faster)")
    print(f"Line 3: DDA Integer Method (NumPy vectorized)\nTime: {time6:.4f} ms  ({time3/time6:.1f}x faster)")

    # Plotting millions of individual markers can exhaust memory / freeze
    # the renderer. The *computation and timing* above already used every
    # point; for the plot alone we subsample so it stays responsive.
    MAX_PLOT_POINTS = 200_000

    def subsample(xs, ys, cap=MAX_PLOT_POINTS):
        xs = np.asarray(xs)
        ys = np.asarray(ys)
        if len(xs) > cap:
            idx = np.linspace(0, len(xs) - 1, cap).astype(int)
            return xs[idx], ys[idx]
        return xs, ys

    fig, axes = plt.subplots(2, 3, figsize=(20, 11))
    (ax1, ax2, ax3), (ax4, ax5, ax6) = axes

    px, py = subsample(x_coorinates1, y_coorinates1)
    ax1.plot(px, py, marker="o", markersize=1, markerfacecolor="red", linestyle="None")
    ax1.set_title(f"y=mx+c (original)\n{time1:.2f} ms")

    px, py = subsample(x_coorinates2, y_coorinates2)
    ax2.plot(px, py, marker="o", markersize=1, markerfacecolor="orange", linestyle="None")
    ax2.set_title(f"DDA (original)\n{time2:.2f} ms")

    px, py = subsample(x_coorinates3, y_coorinates3)
    ax3.plot(px, py, marker="o", markersize=1, markerfacecolor="black", linestyle="None")
    ax3.set_title(f"DDA Integer (original)\n{time3:.2f} ms")

    px, py = subsample(x_coorinates1_np, y_coorinates1_np)
    ax4.plot(px, py, marker="o", markersize=1, markerfacecolor="red", linestyle="None")
    ax4.set_title(f"y=mx+c (NumPy)\n{time4:.2f} ms  ({time1/time4:.1f}x)")

    px, py = subsample(x_coorinates2_np, y_coorinates2_np)
    ax5.plot(px, py, marker="o", markersize=1, markerfacecolor="orange", linestyle="None")
    ax5.set_title(f"DDA (NumPy)\n{time5:.2f} ms  ({time2/time5:.1f}x)")

    px, py = subsample(x_coorinates3_np, y_coorinates3_np)
    ax6.plot(px, py, marker="o", markersize=1, markerfacecolor="black", linestyle="None")
    ax6.set_title(f"DDA Integer (NumPy)\n{time6:.2f} ms  ({time3/time6:.1f}x)")

    for ax in axes.flat:
        ax.set_xlabel("x")
        ax.set_ylabel("y")

    fig.suptitle(f"Line Drawing Algorithms — Original vs. NumPy-Vectorized  (n={len(lines)} lines)", fontsize=14)
    plt.tight_layout(rect=[0, 0, 1, 0.96])
    plt.savefig("line_algorithms_comparison.png", dpi=130)  # saved next to the script
    print("\nSaved plot to line_algorithms_comparison.png")
    plt.show()