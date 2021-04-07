
from ctypes import cdll

p = cdll.LoadLibrary('C:/git_repository/Sungsu_Lab/python/test/ctypes/test.dll')
print(p.add(1,2))
