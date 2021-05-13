
from cs50 import get_int


n = get_int("Number: ")
n1, n2, sumd, length = n, int(n / 10), 0, len(str(n))

# Luhn's Algorithm
while n2 > 0:
    d2 = (n2 % 10) * 2
    d2 = d2 % 10 + int(d2 / 10)
    sumd += d2
    n2 = int(n2 / 100)

while n1 > 0:
    sumd += (n1 % 10)
    n1 = int(n1 / 100)
    
# conditions
if sumd % 10 == 0:
    if length == 15 and (int(n / 10**13) == 34 or int(n / 10**13) == 37):
        print("AMEX")
    elif length == 16 and (int(n / 10**14) == 51 or int(n / 10**14) == 52 or int(n / 10**14) == 53 or int(n / 10**14) == 54 or int(n / 10**14) == 55):
        print("MASTERCARD")
    elif (length == 13 and int(n / 10**12) == 4) or (length == 16 and int(n / 10**15) == 4):
        print("VISA")
    else:
        print("INVALID")
else:
    print("INVALID")