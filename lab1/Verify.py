import numpy as np

def read_matrices(filename):
    with open(filename) as f:
        n = int(f.readline())
        A = np.array([list(map(float, f.readline().split())) for _ in range(n)])
        B = np.array([list(map(float, f.readline().split())) for _ in range(n)])
    return n, A, B

def read_result(filename, n):
    C = []
    with open(filename) as f:
        f.readline() # пропускаем размер
        for _ in range(n):
            C.append(list(map(float, f.readline().split())))
    return np.array(C)

n, A, B = read_matrices("Input.txt")
C_ref = A @ B # Эталонное умножение
C_res = read_result("Output.txt", n)

diff = np.max(np.abs(C_ref - C_res))
print(f"Максимальная разница: {diff:.2e}")
print("Верификация пройдена" if diff < 1e-3 else "Ошибка!")