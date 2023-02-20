# Print de eerste 10 tafels van iets
# Geef ook aan of de uitkomst even  is
def print_tafel_van(tafel):
   print (f"De tafel van {tafel} is: ")
   for i in range(1,11):
      if (i*tafel)%2 == 0:
          print( f"{i} * {tafel} = {i * tafel} EVEN")
      else:
          print( f"{i} * {tafel} = {i * tafel}")



print_tafel_van(5)

# Uitkomst moet zijn:

# De tafel van 5 is: 
# 1 * 5 = 5
# 2 * 5 = 10 EVEN
# 3 * 5 = 15
# 4 * 5 = 20 EVEN
# 5 * 5 = 25
# 6 * 5 = 30 EVEN
# 7 * 5 = 35
# 8 * 5 = 40 EVEN
# 9 * 5 = 45
# 10 * 5 = 50 EVEN