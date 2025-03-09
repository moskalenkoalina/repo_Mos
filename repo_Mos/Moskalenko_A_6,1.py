class Matrix2D:
    def __init__(self, a11=0, a12=0, a21=0, a22=0):
        self.a11 = a11
        self.a12 = a12
        self.a21 = a21
        self.a22 = a22

    def input_from_file(self, line):
        try:
            self.a11, self.a12, self.a21, self.a22 = map(float, line.split())
            print(f"Matrix loaded: {self.a11}, {self.a12}, {self.a21}, {self.a22}")
        except ValueError as e:
            print(f"Error loading matrix: {e}")

    def output_to_console(self):
        print(f"{self.a11} {self.a12}")
        print(f"{self.a21} {self.a22}")

    def output_to_file(self, file):
        file.write(f"{self.a11} {self.a12}\n")
        file.write(f"{self.a21} {self.a22}\n")

    def determinant(self):
        return self.a11 * self.a22 - self.a12 * self.a21

    def is_degenerate(self):
        return self.determinant() == 0


class Vector2D:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

    def input_from_file(self, line):
        try:
            self.x, self.y = map(float, line.split())
            print(f"Vector loaded: {self.x}, {self.y}")
        except ValueError as e:
            print(f"Error loading vector: {e}")

    def output_to_console(self):
        print(f"{self.x} {self.y}")

    def output_to_file(self, file):
        file.write(f"{self.x} {self.y}\n")


class Solver:
    def solve(self, matrix, rhs):
        det_main = matrix.determinant()
        print(f"Main determinant: {det_main}")
        if det_main == 0:
            print("Matrix is degenerate.")
            return None


        matrix_x = Matrix2D(rhs.x, matrix.a12, rhs.y, matrix.a22)
        det_x = matrix_x.determinant()
        print(f"Determinant for x: {det_x}")


        matrix_y = Matrix2D(matrix.a11, rhs.x, matrix.a21, rhs.y)
        det_y = matrix_y.determinant()
        print(f"Determinant for y: {det_y}")


        x = det_x / det_main
        y = det_y / det_main
        return Vector2D(x, y)


def main():
    try:

        with open("matrix_coefficients.txt", "r") as matrix_file, \
                open("rhs_values.txt", "r") as rhs_file, \
                open("solutions1.txt", "w") as solution1_file:

            while True:

                matrix_line = matrix_file.readline()
                rhs_line = rhs_file.readline()


                if not matrix_line or not rhs_line:
                    print("End of file reached.")
                    break


                matrix = Matrix2D()
                matrix.input_from_file(matrix_line)

                rhs = Vector2D()
                rhs.input_from_file(rhs_line)


                solver = Solver()
                solution1 = solver.solve(matrix, rhs)

                
                if solution1:
                    solution1_file.write("Розв'язок:\n")
                    solution1.output_to_file(solution1_file)
                else:
                    solution1_file.write("Система не має розв'язку.\n")

    except FileNotFoundError as e:
        print(f"Error opening files: {e}")
    except Exception as e:
        print(f"Unexpected error: {e}")


if __name__ == "__main__":
    main()