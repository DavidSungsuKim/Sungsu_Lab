import numpy as np
import matplotlib.pyplot as plt

# --- 데이터 생성 ---
np.random.seed(0)
X = np.random.rand(10, 1)  # 2차원 데이터 (10개의 x 값)
y = 3 * X + 2 + np.random.randn(10, 1) * 0.5  # y = 3x + 2 + 노이즈

# --- 데이터 시각화 ---
plt.scatter(X, y, color='blue', label='Data points')
plt.xlabel('X')
plt.ylabel('y')
plt.title('Data points')
plt.legend()
plt.grid()
plt.show()

# --- 1. 선형대수 연산을 통한 최소제곱법 계산 (수동 방식) ---
# X에 절편(bias) 항 추가
X_bias = np.hstack((np.ones((X.shape[0], 1)), X))  # 1 추가 (10x2 행렬)

# 최소제곱 해 공식: W = (X^T X)^(-1) X^T y
XtX = np.dot(X_bias.T, X_bias)           # X^T X
XtX_inv = np.linalg.inv(XtX)             # (X^T X)^(-1)
Xty = np.dot(X_bias.T, y)                # X^T y
W_manual = np.dot(XtX_inv, Xty)          # W 계산

print("수동 계산으로 얻은 가중치 W (절편과 기울기):")
print(W_manual)

# --- 2. numpy의 lstsq 함수로 검증 ---
W_numpy, residuals, rank, s = np.linalg.lstsq(X_bias, y, rcond=None)  # 최소제곱 계산

print("\nNumpy lstsq로 얻은 가중치 W (절편과 기울기):")
print(W_numpy)

# --- 결과 시각화 ---
# 모델 예측 값 계산
y_pred_manual = np.dot(X_bias, W_manual)  # 수동 계산 예측 값
y_pred_numpy = np.dot(X_bias, W_numpy)    # numpy 계산 예측 값

# 데이터와 회귀선 시각화
plt.scatter(X, y, color='blue', label='Data points')  # 데이터 점
plt.plot(X, y_pred_manual, color='red', label='Manual Fit Line')  # 수동 계산 회귀선
plt.plot(X, y_pred_numpy, color='green', linestyle='--', label='Numpy Fit Line')  # numpy 회귀선
plt.xlabel('X')
plt.ylabel('y')
plt.title('Least Square Method')
plt.legend()
plt.grid()
plt.show()

# --- 결과 비교 ---
print("\n두 방식의 결과 차이 (오차):")
print(np.abs(W_manual - W_numpy))
