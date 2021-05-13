from cs50 import SQL
from sys import argv, exit

db = SQL("sqlite:///students.db")

if len(argv) != 2:
    print("Usage: python roster.py <house>")
    exit(1)
    
    
roster = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", argv[1])

for i in range(len(roster)):
    if roster[i]['middle']:
        print(roster[i]['first'], roster[i]['middle'], str(roster[i]['last']) + ', born', roster[i]['birth'])
    else:
        print(roster[i]['first'], str(roster[i]['last']) + ', born', roster[i]['birth'])