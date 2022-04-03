
import numpy as np
import pandas as pd
import matplotlib.pyplot as plt
import sklearn
import mglearn
from IPython.display import display
from sklearn.datasets import load_iris
from sklearn.model_selection import train_test_split
from sklearn.neighbors import KNeighborsClassifier


print("version scikit-learn: ", sklearn.__version__)

iris_dataset = load_iris()

print("keys: ", iris_dataset.keys())

print(iris_dataset['DESCR'][:193] + "\n...")

"""
print("filename: \n", iris_dataset['filename'])
print("target_names: \n", iris_dataset['target_names'])
print("feature names: \n", iris_dataset['feature_names'])
print("type of target: \n", type(iris_dataset['target']))
print("size of type: \n", iris_dataset['target'].shape)
print("target: \n", iris_dataset['target'])
print("type of data: \n", type(iris_dataset['data']))
print("size of data: \n", iris_dataset['data'].shape)
print("data: \n", iris_dataset['data'])
"""

X_train, X_test, y_train, y_test = train_test_split(iris_dataset['data'], iris_dataset['target'], random_state=0)

print("size of X_train: \n", X_train.shape)
print("size of y_train: \n", y_train.shape)

print("size of X_test: \n", X_test.shape)
print("size of y_test: \n", y_test.shape)

iris_dataframe = pd.DataFrame(X_train, columns=iris_dataset.feature_names)
pd.plotting.scatter_matrix(iris_dataframe, c=y_train, figsize=(15,15), marker='o', hist_kwds={'bins':20}, s=60, alpha=.8, cmap=mglearn.cm3)

knn = KNeighborsClassifier(n_neighbors=1)
knn.fit(X_train, y_train)

# prediction
X_new = np.array([[5, 2.9, 1, 0.2]])
print("X_new.shape: ", X_new.shape)

prediction = knn.predict(X_new)
print("prediction: ", prediction)
print("name predicted: ", iris_dataset['target_names'][prediction])

# model evaluation
y_pred = knn.predict(X_test)
print("prediction for the test set: ", y_pred)
print("accuracy in prediction for the test set: {:.2f}".format(np.mean(y_pred == y_test)))
print("accuracy in prediction for the test set: {:.2f}".format(knn.score(X_test, y_test)))

print("program...done")