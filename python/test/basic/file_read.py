# 테스트 파일 읽기

f = open("test.txt", 'w')
f.write("hello")
f.close()

f = open("test.txt", 'r')
line = f.readline()
print(line)

f.close()
