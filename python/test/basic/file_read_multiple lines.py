# 테스트 파일 다 읽기

f = open("test.txt", 'w')
f.write("hello ")
f.write("I am...")
f.close()

f = open("test.txt", 'r')
all = f.read()

print(all)
f.close()
