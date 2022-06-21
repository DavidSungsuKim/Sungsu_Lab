from IPython.display import display
import sklearn
import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import mglearn

x,y = mglearn.datasets.make_forge()

mglearn.discrete_scatter(x[:,0], x[:,1], y)
plt.legend(["class 0", "class 1"], loc=4)
plt.xlabel("attribute #1")
plt.ylabel("attribute #2")

print("x(data): \n", x)
print("y(label): \n", y)
print("x.shape: ", x.shape)
print("y.shape: ", y.shape)

plt.show()

from sklearn.model_selection import train_test_split
x,y = mglearn.datasets.make_forge()
print("x(data): \n", x)
print("y(label): \n", y)
print("x.shape: ", x.shape)
print("y.shape: ", y.shape)

x_train, x_test, y_train, y_test = train_test_split(x,y,random_state=0)

from sklearn.neighbors import KNeighborsClassifier
clf = KNeighborsClassifier(n_neighbors=3)
clf.fit(x_train, y_train)
print("prediction on the test set: ", clf.predict(x_test))
print("prediction accuracy: {:.2f}".format(clf.score(x_test, y_test)))

"""
#wave dataset
x,y = mglearn.datasets.make_wave(n_samples=40)
plt.plot(x,y, 'o')
plt.ylim(-3,3)
plt.xlabel("attributes")
plt.ylabel("target")
plt.show()
"""

"""
#brast cancer dataset
from sklearn.datasets import load_breast_cancer
cancer = load_breast_cancer()
print("cancer.keys(): \n", cancer.keys())
print("type of brast cancer data: \n", cancer.data.shape)
print("the number of data: ", cancer.data.shape[0])
print("the number of classes: ", cancer.data.shape[1])
print("the number of samples per class: \n", {n: v for n, v in zip(cancer.target_names, np.bincount(cancer.target))})
print("feature names: \n", cancer.feature_names)
"""

"""
#boston housig price dataset
from sklearn.datasets import fetch_california_housing
california = fetch_california_housing()
print("type of data: ", california.data.shape)
"""
