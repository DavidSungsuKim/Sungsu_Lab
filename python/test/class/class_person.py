class Person:
    def __init__(self):
        self.result = 0

    def test(self, a_a):
        self.a = a_a
        
    def name(self, a_name):
        self.name = a_name
        
    def address(self, a_address):
        self.address = a_address
        
    def showInfo(self):
        print("name:",self.name)
        print("address:",self.address)

#myself = Person()
#myself.name('David')
#myself.address('Calgary')
#myself.showInfo()
