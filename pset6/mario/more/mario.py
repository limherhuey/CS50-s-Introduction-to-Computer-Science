
from cs50 import get_int

while True:
    n = get_int("Height of pyramid: ")
    if n >= 1 and n <= 8:
        break

for i in range(n):
    for j in range(n - i - 1):
        print(" ", end="")
    for k in range(i + 1):
        print("#", end="")
    print("  ", end="")
    for l in range(i + 1):
        print("#", end="")
    print("")
