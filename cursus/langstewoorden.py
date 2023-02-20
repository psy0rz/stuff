import sys

woordenfile = open('wordlist.txt')
gefilterde_woorden = open('nederlands.txt','w')

woorden = woordenfile.readlines()


def sorteersleutel(woord):
    return (len(woord))


woorden.sort(key=sorteersleutel, reverse=True)

print("".join(woorden[0:50]))

#### filter alleen letters

geldige_woorden = []

geldige_letters = "abcdefghijklmnopqrstuvwxyz"


for woord in woorden:
    geldig = True
    woord = woord.strip()
    for letter in woord:
        if letter not in geldige_letters:
            # ongeldig woord
            geldig = False

    # if len(woord)!=5:
    #     geldig=False

    if geldig:
        geldige_woorden.append(woord)
        gefilterde_woorden.write(woord+'\n')

# for woord in woorden:
#     woord = woord.strip()
#     for letter in woord:
#         if letter not in geldige_letters:
#             woord=""
#
#     if woord!="":
#         geldige_woorden.append(woord)


#### sorteer


print("------------------------")
geldige_woorden.sort(key=sorteersleutel, reverse=True)
print("\n".join(geldige_woorden[0:50]))

print(len(geldige_woorden))
