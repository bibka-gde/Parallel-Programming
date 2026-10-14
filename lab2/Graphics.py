import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Чтение данных
df = pd.read_csv('benchmark_results.csv')

# График 1: Время выполнения
plt.figure(figsize=(10, 6))
for threads in [1, 2, 4, 8]:
    subset = df[df['Threads'] == threads]
    plt.plot(subset['Matrix_Size'], subset['Time_Seconds'], 
             marker='o', label=f'{threads} threads')

plt.xlabel('Matrix Size')
plt.ylabel('Time (seconds)')
plt.title('Matrix Multiplication Performance')
plt.legend()
plt.grid(True)
plt.savefig('graphs/time_vs_size.png', dpi=300)
plt.show()

# График 2: Ускорение
plt.figure(figsize=(10, 6))
sizes = [200, 400, 800, 1200, 1600, 2000]
for size in sizes:
    subset = df[df['Matrix_Size'] == size]
    plt.plot(subset['Threads'], subset['Speedup'], 
             marker='o', label=f'Size {size}')

plt.xlabel('Number of Threads')
plt.ylabel('Speedup')
plt.title('Parallel Speedup')
plt.legend()
plt.grid(True)
plt.savefig('graphs/speedup.png', dpi=300)
plt.show()