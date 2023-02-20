
compleet="on..i..e.in...e.i...e"
fout_geraden_letters="pudb"





woord="test"
geldige_letters = "abcdefghijklmnopqrstuvwxyz"

for letter in woord:
    print(f"letter is {letter}")

    if letter not in geldige_letters:
        woord=""


