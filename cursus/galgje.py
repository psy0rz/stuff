import random

galg=[
    #af
    """
          ######
          #    |
          #   \o/
          #    O
          #   / \\
          #
      ##########    
    """,
    """
          ######
          #    |
          #   \o/
          #    O
          #   / 
          #
      ##########    
    """,
    """
          ######
          #    |
          #   \o/
          #    O
          #    
          #
      ##########    
    """,
    """
          ######
          #    |
          #   \o/
          #    
          #    
          #
      ##########    
    """,
    """
          ######
          #    |
          #   \o
          #    
          #    
          #
      ##########    
    """,
    """
          ######
          #    |
          #    o
          #    
          #    
          #
      ##########    
    """,
    """
          ######
          #    |
          #   
          #     
          #    
          #
      ##########    
    """,

]

def speel():
    fh = open('nederlands.txt')
    antwoord = random.choice(fh.readlines()).rstrip()

    compleet = len(antwoord) * ["."]


    foute_letters=''
    beurten=6

    while True:
        print()
        print("-----------------------------------------------------")
        print()
        print(galg[beurten])

        if beurten==0:
            print("AF!")
            return

        print(f"Woord         : {''.join(compleet)}")
        print(f"Foute letters : {foute_letters}")
        print(f"{beurten} beurten")
        raad = input("Raad letter > ")

        if raad=='*':
            print(f"CHEAT MODE: {antwoord}")
            continue

        for letter_nr in range(0, len(antwoord)):
            if antwoord[letter_nr] == raad:
                compleet[letter_nr] = raad

        if raad not in antwoord:
            foute_letters=foute_letters+raad
            beurten=beurten-1

        if "".join(compleet)==antwoord:
            print(f"f{antwoord} is GOED!")
            return



speel()