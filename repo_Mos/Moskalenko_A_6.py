import math



class Figure:
    def get_perimeter(self):
        raise NotImplementedError("Метод get_perimeter() не реалізовано")

    def get_area(self):
        raise NotImplementedError("Метод get_area() не реалізовано")

    def get_name(self):
        raise NotImplementedError("Метод get_name() не реалізовано")



class Circle(Figure):
    def __init__(self, radius):
        self.radius = radius

    def get_perimeter(self):
        return 2 * math.pi * self.radius

    def get_area(self):
        return math.pi * self.radius ** 2

    def get_name(self):
        return "Circle"



class Rectangle(Figure):
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def get_perimeter(self):
        return 2 * (self.width + self.height)

    def get_area(self):
        return self.width * self.height

    def get_name(self):
        return "Rectangle"


class Triangle(Figure):
    def __init__(self, side1, side2, side3):
        self.side1 = side1
        self.side2 = side2
        self.side3 = side3

    def get_perimeter(self):
        return self.side1 + self.side2 + self.side3

    def get_area(self):

        s = self.get_perimeter() / 2
        area = math.sqrt(s * (s - self.side1) * (s - self.side2) * (s - self.side3))
        return area

    def get_name(self):
        return "Triangle"



class Parallelogram(Figure):
    def __init__(self, base, height):
        self.base = base
        self.height = height

    def get_perimeter(self):
        return 2 * (self.base + self.height)

    def get_area(self):
        return self.base * self.height

    def get_name(self):
        return "Parallelogram"



def write_to_file(figures, filename="output.txt"):
    with open(filename, "w") as file:
        for figure in figures:
            file.write(f"Figure: {figure.get_name()}\n")
            file.write(f"Perimeter: {figure.get_perimeter():.2f}\n")
            file.write(f"Area: {figure.get_area():.2f}\n\n")


def main():

    circle = Circle(radius=5)
    rectangle = Rectangle(width=10, height=5)
    triangle = Triangle(side1=3, side2=4, side3=5)
    parallelogram = Parallelogram(base=6, height=4)


    figures = [circle, rectangle, triangle, parallelogram]


    write_to_file(figures)

    print("Вивід результатів записано у файл 'output.txt'")



if __name__ == "__main__":
    main()
