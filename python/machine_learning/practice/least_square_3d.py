import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

# --- 데이터 생성 ---
np.random.seed(0)
X = np.random.rand(10, 3)  # 3차원 입력 데이터 (10개 샘플)
y = 3 * X[:, 0] + 2 * X[:, 1] + X[:, 2] + 5 + np.random.randn(10) * 0.5  # y = 3x1 + 2x2 + x3 + 5 + 노이즈

# --- 1. 수동 방식으로 최소제곱법 계산 ---
# X에 절편(bias) 항 추가
X_bias = np.hstack((np.ones((X.shape[0], 1)), X))  # 크기 (10, 4)

# 최소제곱 해 공식: W = (X^T X)^(-1) X^T y
XtX = np.dot(X_bias.T, X_bias)           # X^T X
XtX_inv = np.linalg.inv(XtX)             # (X^T X)^(-1)
Xty = np.dot(X_bias.T, y)                # X^T y
W_manual = np.dot(XtX_inv, Xty)          # 수동 계산 결과

# --- 2. numpy의 lstsq 함수로 계산 ---
W_numpy, residuals, rank, s = np.linalg.lstsq(X_bias, y, rcond=None)  # 최소제곱 계산

# --- 회귀평면 계산 ---
y_pred_manual = np.dot(X_bias, W_manual)  # 수동 계산 예측 값
y_pred_numpy = np.dot(X_bias, W_numpy)    # numpy 계산 예측 값

# --- 결과 출력 ---
print("계산된 회귀 계수 W:")
print("수동 방식:")
print(W_manual)
print("\nNumPy 내장 함수:")
print(W_numpy)

print("\n실제 출력 값 y, 수동 계산 y_pred, NumPy 계산 y_pred:")
for i in range(len(y)):
    print(f"y[{i}] = {y[i]:.3f}, y_pred_manual[{i}] = {y_pred_manual[i]:.3f}, y_pred_numpy[{i}] = {y_pred_numpy[i]:.3f}")

# --- 시각화 ---
fig = plt.figure(figsize=(12, 8))
ax = fig.add_subplot(111, projection='3d')

# 데이터 포인트 시각화
ax.scatter(X[:, 0], X[:, 1], y, color='blue', label='Data points')

# 회귀평면 생성 (수동 방식 사용)
x1_range = np.linspace(X[:, 0].min(), X[:, 0].max(), 10)
x2_range = np.linspace(X[:, 1].min(), X[:, 1].max(), 10)
x1_grid, x2_grid = np.meshgrid(x1_range, x2_range)
x3_grid = -(W_manual[1] * x1_grid + W_manual[2] * x2_grid + W_manual[0]) / W_manual[3]
ax.plot_surface(x1_grid, x2_grid, x3_grid, color='red', alpha=0.5, label='Regression Plane (Manual)')

# 오차 시각화
for i in range(len(X)):
    ax.plot([X[i, 0], X[i, 0]], [X[i, 1], X[i, 1]], [y[i], y_pred_manual[i]], color='gray', linestyle='--', linewidth=0.8)

# 축 레이블 및 제목
ax.set_xlabel('X1')
ax.set_ylabel('X2')
ax.set_zlabel('y')
plt.title('3D Least Squares Regression Plane with Errors')
plt.legend()
plt.show()

# --- 두 방식 결과 차이 확인 ---
print("\n두 방식의 계수 차이:")
print(np.abs(W_manual - W_numpy))
