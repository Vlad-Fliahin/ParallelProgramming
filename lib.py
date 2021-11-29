import numpy as np  # Бібліотека для математики
from time import time  # Функція для виміру часу
from multiprocessing import Pool  # Конструкція для забезпечення паралельнох роботи

THREAD_COUNT = 2
MAX_VALUE = 101


def measure_time_spent(func, count=3):
    time_spent = float('inf')
    for _ in range(count):
        start = time()
        print(func())
        finish = time()
        time_spent = min(time_spent, finish - start)
    return time_spent


def integral_pi(n):
    h = 1.0 / n
    pi = 0
    x = h
    for i in range(n):
        pi += 4 / (1 + x*x)
        x += h
    return pi * h


def integral_pi_parallel(n):
    with Pool(processes=THREAD_COUNT) as pool:
        pi = pool.map(integral_pi, [n])
    return pi
