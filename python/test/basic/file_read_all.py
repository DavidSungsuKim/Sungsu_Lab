# 테스트 파일 여러줄 읽기

f = open("test.txt", 'w')
f.write("hello ")
f.write("I am...")
f.close()

f = open("test.txt", 'r')
lines = f.readlines()

for line in lines:
    print(line)
f.close()
