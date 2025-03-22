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
    parts = line.split()
    name = parts[0]
    params = list(map(float, parts[1:]))
    classes = {
        "Triangle": Triangle,
        "Rectangle": Rectangle,
        "Circle": Circle,
        "Ball": Ball,
        "Cone": Cone
    }
    if name in classes:
        try:
            return classes[name](*params)
        except (ValueError, TypeError):
            return None
    return None

def find_largest(filename):
    with open(filename, 'r') as file:
        return max(
            (parse_figure(line) for line in file),
            key=lambda f: f.volume() if f else 0,
            default=None
        )

def write_r(result, output_filename):
    with open(output_filename, 'w') as file:
        if result:
            file.write(f"Найбільша фігура: {result.__class__.__name__} з мірою {result.volume()}\n")
        else:
            file.write("У файлі немає коректних фігур.\n")

def main():
    largest_f = find_largest("input01.txt")
    write_r(largest_f, "output.txt")

if __name__ == "__main__":
    main()
