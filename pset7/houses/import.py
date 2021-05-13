from cs50 import SQL
from sys import argv, exit
from csv import DictReader

# new variable 'db' to give access to this database, students.db
db = SQL("sqlite:///students.db")

if len(argv) != 2:
    print("Usage: python import.py <csv file>")
    exit(1)


with open(argv[1]) as database:
    reader = DictReader(database)
    for person in reader:

        # split name; if only 2, is first and last name; if 3, is first, middle, last
        name = person['name'].split()
        if len(name) == 2:
            db.execute("INSERT INTO students (first, middle, last) VALUES(?, ?, ?)", name[0], None, name[1])
        else:
            db.execute("INSERT INTO students (first, middle, last) VALUES(?, ?, ?)", name[0], name[1], name[2])

        # insert the rest of the values into the same row
        db.execute("UPDATE students SET house = ?, birth = ? WHERE first = ?", person['house'], person['birth'], name[0])
