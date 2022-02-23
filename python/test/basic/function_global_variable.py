a_global = 0

def myFunc(a):
    global a_global
    a_global += a
    return

myFunc(1)
print(a_global)
