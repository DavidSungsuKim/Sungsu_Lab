import numpy as np
import matplotlib.pyplot as plt

# 2차원 데이터 포인트 생성 (수동으로 10개 데이터)
x = np.array([1, 2, 3, 4, 6, 6, 5, 8, 9, 10])
y = np.array([2, 4, 6, 9, 9, 12, 14, 15, 19, 21])
z = np.array([2, 4, 6, 9, 9, 12, 14, 15, 19, 21])

data = np.column_stack((x, y, z))

cov_matrix_numpy = np.cov(data, rowvar=False)

print("NumPy를 사용하여 계산한 공분산 행렬:\n", cov_matrix_numpy)
print("NumPy를 사용하여 계산한 공분산:", cov_matrix_numpy[0, 1])

# 데이터 플로팅
plt.figure()
plt.scatter(x, y, c='b', marker='o')
plt.xlabel('X Label')
plt.ylabel('Y Label')
plt.title('2D Data Points with Noise')
plt.grid(True)

# 그래프 표시
plt.show()
