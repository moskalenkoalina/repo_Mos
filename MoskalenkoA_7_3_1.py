class RationalError(ZeroDivisionError):
    def __init__(self, message="Знаменник не може бути нулем"):
        super().__init__(message)

class RationalValueError(Exception):
    def __init__(self, message="Некоректні дані для раціонального числа"):
        super().__init__(message)



class Rational:
    def __init__(self, *args):
        if len(args) == 1 and isinstance(args[0], str):
            try:
                n, d = map(int, args[0].split('/'))
            except Exception:
                raise RationalValueError("Неправильний формат рядка, очікується 'чисельник/знаменник'")
        elif len(args) == 2:
            n, d = args
            if not all(isinstance(x, int) for x in (n, d)):
                raise RationalValueError("Чисельник і знаменник мають бути цілими числами")
        else:
            raise RationalValueError("Неправильні аргументи конструктора")

        if d == 0:
            raise RationalError()

        g = self._gcd(n, d)
        self.n = n // g
        self.d = d // g

        if self.d < 0:
            self.n = -self.n
            self.d = -self.d

    def _gcd(self, a, b):
        while b != 0:
            a, b = b, a % b
        return abs(a)

    def __add__(self, other):
        other = self._to_rational(other)
        return Rational(self.n * other.d + other.n * self.d, self.d * other.d)

    def __sub__(self, other):
        other = self._to_rational(other)
        return Rational(self.n * other.d - other.n * self.d, self.d * other.d)

    def __mul__(self, other):
        other = self._to_rational(other)
        return Rational(self.n * other.n, self.d * other.d)

    def __truediv__(self, other):
        other = self._to_rational(other)
        if other.n == 0:
            raise ZeroDivisionError("Ділення на нуль")
        return Rational(self.n * other.d, self.d * other.n)

    def _to_rational(self, value):
        if isinstance(value, Rational):
            return value
        elif isinstance(value, int):
            return Rational(value, 1)
        else:
            raise RationalValueError("Операція можлива лише з Rational або int")

    def __call__(self):
        return self.n / self.d

    def __getitem__(self, key):
        if key == 'n':
            return self.n
        elif key == 'd':
            return self.d
        else:
            raise KeyError("Ключ має бути 'n' або 'd'")

    def __setitem__(self, key, value):
        if not isinstance(value, int):
            raise RationalValueError("Значення має бути цілим числом")
        if key == 'n':
            self.n = value
        elif key == 'd':
            if value == 0:
                raise RationalError()
            self.d = value
        else:
            raise KeyError("Ключ має бути 'n' або 'd'")
        self._reduce()


    def _reduce(self):
        g = self._gcd(self.n, self.d)
        self.n //= g
        self.d //= g
        if self.d < 0:
            self.n = -self.n
            self.d = -self.d

    def __str__(self):
        return f"{self.n}/{self.d}"

    def __repr__(self):
        return f"Rational({self.n}, {self.d})"

def evaluate_expression(expr):
    expr = expr.replace(" ", "")
    operators = {'+', '-', '*', '/'}
    for op in operators:
        if op in expr:
            left, right = expr.split(op)
            left = Rational(left)
            right = Rational(right)
            if op == '+':
                return left + right
            elif op == '-':
                return left - right
            elif op == '*':
                return left * right
            elif op == '/':
                return left / right
    raise ValueError(f"Не вдалося розпізнати вираз: {expr}")

def process_file(filename):
    with open(filename, 'r') as file:
        for line in file:
            try:
                result = evaluate_expression(line.strip())
                print(f"{line.strip()} = {result} = {result()}")
            except Exception as e:
                print(f"Помилка при обчисленні виразу '{line.strip()}': {e}")


process_file('input01.txt')