odd = [1,3,5,7,9]

for i in odd:
    print("i=",i)

odd = [1,3,5,7,9]
index = 0

print("a[",index,"]=",odd[index])


mylist = [[1,2,3],['a','b','c']]

print("data=", mylist[0][0])
print("data=", mylist[0][1])
print("data=", mylist[1][0])
print("data=", mylist[1][1])


mylist = [0,1,2,3,4,5]

print("subset=", mylist[-1])
print("subset=", mylist[1:2])
print("subset=", mylist[0:])
print("subset=", mylist[:-1])


mylist = [0,1,2,3,4,5]
print("mylist=", mylist)
del mylist[1]
print("mylist=", mylist)
