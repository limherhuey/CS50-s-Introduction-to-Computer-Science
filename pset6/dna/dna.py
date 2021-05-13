
from sys import argv, exit
from csv import DictReader, reader

if len(argv) != 3:
    print("Usage: python dna.py <csv file> <DNA sequence text file>")
    exit(1)

# Short Tandem Repeats (STRs) are short DNA sequences that tends to repeat consequtively
# read csv and store the STR sequences
with open(argv[1]) as database:
    datareader = reader(database)
    for row in datareader:
        strs = row
        strs.pop(0)
        break

# create a dictionary mapping each STR to its number of repeats
strcount = {}
for item in strs:
    strcount[item] = 0

# store DNA sequence from txt file as a string (is it necessary to close file???)
dnaobject = open(argv[2])
dna = dnaobject.read()
dnaobject.close()

# find longest consecutive repeats of each STR in the DNA sequence and store
for str in sorted(strcount):
    n = len(str)
    for i in range(len(dna)):
        repeats = 0
        while str == dna[i:i + n]:
            i += n
            repeats += 1
        if repeats > strcount[str]:
            strcount[str] = repeats

# compare STR of each person to that from the DNA sequence, find a match
with open(argv[1]) as database:
    datareader = DictReader(database)
    # iterate through each person
    for person in datareader:
        matched = 0
        # check for all STRs
        for str in sorted(strcount):
            if int(person[str]) != int(strcount[str]):
                # no point in iterating through all of someone's STRs if one already doesn't match
                break
            matched += 1
        # if all STRs match, person is found
        if matched == len(strcount):
            print(person['name'])
            exit(0)

print("No match")