import os
import matplotlib.pyplot as plt
from sklearn.linear_model import LinearRegression
from sklearn.preprocessing import PolynomialFeatures
import numpy as np


def plot_time():
    time = []
    pq_size = []

    os.chdir("experiments")
    f = open('timeAnalysis.txt', 'r')
    i = 0
    for row in f:
        if (i % 20 == 0):
            row = row.split(' ')
            time.append(row[0])
            pq_size.append(int(row[1]))
        i = i+1

    plt.plot(time[:100], pq_size[:100], color='g', label='pq size')

    plt.xlabel('time')
    plt.ylabel('pq size')

    plt.title('Time Analysis')
    plt.legend()
    plt.show()


def poly_regression():

    nums = []
    pq_size = []

    os.chdir("experiments")
    f = open('sizeAnalysis.txt', 'r')
    for row in f:
        row = row.split(' ')
        nums.append(row[0])
        pq_size.append(int(row[1]))

    n = len(nums)
    nums = np.array(nums, ndmin=2)
    nums = np.reshape(nums, (n, 1)).astype(np.float64)
    poly = PolynomialFeatures(degree=2)
    X_poly = poly.fit_transform(nums)
    poly.fit(X_poly, pq_size)
    lin = LinearRegression()
    lin.fit(X_poly, pq_size)
    print(lin.coef_)
    plt.scatter(nums, pq_size, color='blue')
    plt.plot(nums, lin.predict(X_poly), color='red')
    plt.show()

# [  0.         -35.31039136   0.12281416]
#  pq_size = 0.1228 * x^2 - 35.31 *x


def plot_size():
    nums = []
    pq_size = []

    os.chdir("experiments")
    f = open('sizeAnalysis.txt', 'r')
    for row in f:
        row = row.split(' ')
        nums.append(row[0])
        pq_size.append(int(row[1]))

    plt.plot(nums, pq_size, color='g', label='pq size')

    plt.xlabel('n')
    plt.ylabel('pq size')

    plt.title('Size Analysis')
    plt.legend()
    plt.show()


poly_regression()
