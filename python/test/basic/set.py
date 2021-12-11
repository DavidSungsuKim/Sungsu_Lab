set1 = set([1,2,3,3])
set2 = set([3,4,4,5])
print("set1=", set1)
print("set2=", set2)

set1 = set([1,2,3,3])
set2 = set([3,4,4,5])
print("set1 & set2=", set1 & set2)
print("set1 & set2=", set1.intersection(set2))

set1 = set([1,2,3,3])
set2 = set([3,4,4,5])
print("set1 | set2=", set1 | set2)
print("set1 & set2=", set1.union(set2))

set1 = set([1,2,3,3])
set2 = set([3,4,4,5])
print("set1 - set2=", set1 - set2)
print("set1 - set2=", set1.difference(set2))
