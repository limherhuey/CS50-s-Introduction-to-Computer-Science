
from cs50 import get_float

while True:
    n = get_float("Change owed: ")
    if n > 0:
        break
    
coins = 0
n = int(n * 100)

coins += int(n / 25)
n %= 25
    
coins += int(n / 10)
n %= 10
    
coins += int(n / 5)
n %= 5
    
coins += n

print(coins)