import math

def task_a(x, k):
    return x**(2 * k) / math.factorial(2 * k)

def task_b(n):
    result = 1.0
    for k in range(1, n + 1):
        result *= (1 + 1 / (k ** 2))
    return result

def task_c(a, b, n):
    if n == 1:
        return a + b
    elif n == 2:
        return (a + b) ** 2 - a * b
    d_prev_prev = a + b
    d_prev = (a + b) ** 2 - a * b
    for i in range(3, n + 1):
        d_curr = (a + b) * d_prev - a * b * d_prev_prev
        d_prev_prev, d_prev = d_prev, d_curr
    return d_prev

def task_d(n):
    if n < 1:
        return 0.0
    a = [0, 1, 1, 1]
    for k in range(4, n + 1):
        a.append(a[k - 1] + a[k - 3])
    total = 0.0
    for k in range(1, n + 1):
        total += a[k] / (2 ** k)
    return total

def task_e(x, epsilon):
    if abs(x) >= 1:
        raise ValueError("|x| має бути < 1")
    result = 0.0
    k = 1
    while True:
        term = 2 * x**(2*k - 1) / (2*k - 1)
        result += term
        if abs(term) < epsilon:
            break
        k += 1
    exact = math.log((1 + x) / (1 - x))
    return result, exact, abs(result - exact)

def main():
    print("Задача a:")
    x = float(input("Введіть x для задачі a: "))
    k = int(input("Введіть k для задачі a: "))
    print(f"x_{k} = {task_a(x, k)}\n")

    print("Задача b:")
    n_b = int(input("Введіть n для задачі b: "))
    print(f"P_{n_b} = {task_b(n_b)}\n")

    print("Задача c:")
    a_c = float(input("Введіть a для задачі c: "))
    b_c = float(input("Введіть b для задачі c: "))
    n_c = int(input("Введіть порядок матриці n для задачі c: "))
    print(f"det(A_{n_c}) = {task_c(a_c, b_c, n_c)}\n")

    print("Задача d:")
    n_d = int(input("Введіть n для задачі d: "))
    print(f"S_{n_d} = {task_d(n_d)}\n")

    print("Задача e:")
    x_e = float(input("Введіть x для задачі e (|x| < 1): "))
    eps_e = float(input("Введіть точність ε для задачі e: "))
    try:
        approx, exact, diff = task_e(x_e, eps_e)
        print(f"Наближене значення: {approx}")
        print(f"Точне значення: {exact}")
        print(f"Різниця: {diff}")
    except ValueError as e:
        print("Помилка:", e)

if __name__ == "__main__":
    main()
