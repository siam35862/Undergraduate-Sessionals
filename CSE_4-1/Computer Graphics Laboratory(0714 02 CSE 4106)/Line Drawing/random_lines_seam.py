import numpy as np
import cv2
from time import perf_counter
from single_line_seam import basic_equation, dda, dropout_cutoff

n = 600
k = 50
img1 = np.zeros((n, n), dtype=np.uint8)
img2 = np.zeros((n, n), dtype=np.uint8)
img3 = np.zeros((n, n), dtype=np.uint8)
lines = np.random.randint(50, n - 50, size=(k, 4))

start1 = perf_counter()
for x1, y1, x2, y2 in lines:
    basic_equation(x1, y1, x2, y2, img1)

end1 = perf_counter()

start2 = perf_counter()
for x1, y1, x2, y2 in lines:
    dda(x1, y1, x2, y2, img2)

end2 = perf_counter()

for x1, y1, x2, y2 in lines:
    dropout_cutoff(x1, y1, x2, y2, img3)

print(f"Basic Equation took {end1 - start1 : 0.6f}s")
print(f"DDA took {end2 - start2 : 0.6f}s")

cv2.imshow("Basic equation: y = mx + c", img1)
cv2.imshow("DDA: Ynext = Ycurr + m", img2)
cv2.imshow("Cutt-Off: Ynext = Ycurr + m", img3)
cv2.waitKey(0)