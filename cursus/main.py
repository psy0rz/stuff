import random

def hangman(word):
    wrong = 0
    stages = ["",
              "________        ",
              "|      |        ",
              "|      O        ",
              "|     /|\       ",
              "|     / \       ",
              "|               "
              ]
    remaining_letters = list(word)
    board = ["__"] * len(word)
    win = False
    print("Welcome to Hangman")
    while wrong < len(stages) - 1:
        print("\n")
        print(remaining_letters)
        guess = input("Guess a letter: ")
        if guess in remaining_letters:
            character_index = remaining_letters.index(guess)
            board[character_index] = guess
            remaining_letters[character_index] = '$'
        else:
            wrong += 1
        print(" ".join(board))
        print("\n".join(stages[0: wrong + 1]))
        if "__" not in board:
            print("You win! The word was:")
            print(" ".join(board))
            win = True
            break
    if not win:
        print("\n".join(stages[0: wrong]))
        print("You lose! The word was {}.".format(word))


word_list = ["cat", "dog", "bird", "elephant", "lion", "tiger", "monkey", "snake", "fish", "rhino"]
word = random.choice(word_list)

hangman(word)
