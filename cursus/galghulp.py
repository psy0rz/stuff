# compleet = "...e...."
# komt_niet_voor = "n"
import sys

compleet = sys.argv[1]
komt_niet_voor = sys.argv[2]










woordenlijst = open('nederlands.txt')

mogelijke_woorden = []

for woord in woordenlijst:

    woord = woord.strip()

    if len(woord) == len(compleet):
        goed = True

        # kijk of geraden letters overeen komen:
        for i in range(0, len(woord)):
            if (compleet[i] != '.' and compleet[i] != woord[i]):
                goed = False

        # kijk of alle letters die niet voorkwamen ook niet in dit woord zitten:
        for letter in komt_niet_voor:
            if letter in woord:
                goed = False

        if goed:
            mogelijke_woorden.append(woord)

print("\n".join(mogelijke_woorden))

# bepaal best te raden letter
letter_count = {}
for woord in mogelijke_woorden:
    gehad = []
    for letter in woord:
        if letter not in compleet and letter not in komt_niet_voor and letter not in gehad:
            gehad.append(letter)
            if letter not in letter_count:
                letter_count[letter] = 1
            else:
                letter_count[letter] = letter_count[letter] + 1

print(f"Geraden       : {compleet}")
print(f"Komt niet voor: {komt_niet_voor} ")
print("Beste opties:")

for (k, c) in sorted(letter_count.items(), key=lambda x: x[1]):
    print(f"{k}: {int(c * 100 / len(mogelijke_woorden))} %")
