class RechtHoek():

    def __init__(self, a, b):
        self.lengte=a
        self.breedte=b

    def laatzien(self):
        print(f"Deze rechthoek is {self.lengte} x {self.breedte}")

    def printOppervlakte(self):
        print(f"oppervlakte is {self.lengte * self.breedte}")
