mydic = {0:'Canada', 1:'Alberta', 2:'Calgary'}
print("country?", mydic[0])

mydic = {'country':'Canada','province':'Alberta','city':'Calgary'}
print("country?", mydic['country'])

mydic = {0:'Canada', 1:'Alberta', 2:'Calgary'}
del mydic[0]
print("mydic=",mydic)

print(mydic.keys())
print(mydic.values())
