import math

class Figure:
    def dimention(self):
        return None

    def perimetr(self):
        return None

    def square(self):
        return None

    def squareSurface(self):
        return None

    def squareBase(self):
        return None

    def height(self):
        return None

    def volume(self):
        return self.square()

class Triangle(Figure):
    def __init__(self, a, b, c):
        if a + b <= c or a + c <= b or b + c <= a:
            raise ValueError("Такого трикутника не існує")
        self.a = a
        self.b = b
        self.c = c

    def dimention(self):
        return "2D"

    def perimetr(self):
        return self.a + self.b + self.c

    def square(self):
        s = self.perimetr() / 2
        return math.sqrt(s * (s - self.a) * (s - self.b) * (s - self.c))

class Rectangle(Figure):
    def __init__(self, a, b):
        self.a = a
        self.b = b

    def dimention(self):
        return "2D"

    def perimetr(self):
        return 2 * (self.a + self.b)

    def square(self):
        return self.a * self.b

class Circle(Figure):
    def __init__(self, r):
        self.r = r

    def dimention(self):
        return "2D"

    def perimetr(self):
        return 2 * math.pi * self.r

    def square(self):
        return math.pi * self.r ** 2

class Ball(Figure):
    def __init__(self, r):
        self.r = r

    def dimention(self):
        return "3D"

    def squareSurface(self):
        return 4 * math.pi * self.r ** 2

    def volume(self):
        return (4/3) * math.pi * self.r ** 3

class Cone(Circle):
    def __init__(self, r, h):
        super().__init__(r)
        self.h = h

    def dimention(self):
        return "3D"

    def squareSurface(self):
        l = math.sqrt(self.r**2 + self.h**2)
        return math.pi * self.r * l + super().square()

    def volume(self):
        return (1/3) * super().square() * self.h

def parse_figure(line):
    try:
        parts = line.split()
        name, params = parts[0], list(map(float, parts[1:]))
        classes = {
            "Triangle": Triangle,
            "Rectangle": Rectangle,
            "Circle": Circle,
            "Ball": Ball,
            "Cone": Cone
        }
        return classes.get(name)(*params) if name in classes else None
    except (ValueError, TypeError):
        return None

def find_largest_measurement(filename):
    with open(filename, 'r') as file:
        figures = [parse_figure(line) for line in file if parse_figure(line)]
    figures = [f for f in figures if f is not None]
    return max(figures, key=lambda f: f.volume() if f else 0) if figures else None


largest_figure = find_largest_measurement("input01.txt")
if largest_figure:
    print(f"Найбільша фігура: {largest_figure.__class__.__name__} з мірою {largest_figure.volume()}")
else:
    print("У файлі немає коректних фігур.")
