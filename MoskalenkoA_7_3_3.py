from MoskalenkoA5_3_2 import Rational

class RationalValueError(Exception):
    def __init__(self, message="Некоректні дані для списку раціональних чисел"):
        super().__init__(message)


class RationalList:
    def __init__(self, initial=None):
        self.data = []
        if initial is not None:
            for item in initial:
                self.append(item)

    def append(self, value):
        if isinstance(value, int):
            self.data.append(Rational(value, 1))
        elif isinstance(value, Rational):
            self.data.append(value)
        else:
            raise RationalValueError("Можна додавати лише Rational або int")

    def __getitem__(self, index):
        return self.data[index]

    def __setitem__(self, index, value):
        if isinstance(value, int):
            self.data[index] = Rational(value, 1)
        elif isinstance(value, Rational):
            self.data[index] = value
        else:
            raise RationalValueError("Елемент має бути Rational або int")

    def __len__(self):
        return len(self.data)

    def __add__(self, other):
        new_list = RationalList(self.data)
        if isinstance(other, RationalList):
            for item in other.data:
                new_list.append(item)
        elif isinstance(other, (int, Rational)):
            new_list.append(other)
        else:
            raise RationalValueError("Можна додавати RationalList, Rational або int")
        return new_list

    def __iadd__(self, other):
        if isinstance(other, RationalList):
            for item in other.data:
                self.append(item)
        elif isinstance(other, (int, Rational)):
            self.append(other)
        else:
            raise RationalValueError("Можна додавати RationalList, Rational або int")
        return self

    def sum(self):
        if not self.data:
            return Rational(0, 1)
        total = self.data[0]
        for r in self.data[1:]:
            total += r
        return total

    def __iter__(self):
        sorted_data = sorted(self.data, key=lambda r: (-r.d, -r.n))
        return iter(sorted_data)

    def __str__(self):
        return "[" + ", ".join(str(r) for r in self.data) + "]"


    def __add__(self, other):
        new_list = RationalList(self.data)
        if isinstance(other, RationalList):
            for item in other.data:
                new_list.append(item)
        elif isinstance(other, (int, Rational)):
            new_list.append(other)
        else:
            raise TypeError("Можна додавати RationalList, Rational або int")
        return new_list

    def __iadd__(self, other):
        if isinstance(other, RationalList):
            for item in other.data:
                self.append(item)
        elif isinstance(other, (int, Rational)):
            self.append(other)
        else:
            raise TypeError("Можна додавати RationalList, Rational або int")
        return self

    def sum(self):
        if not self.data:
            return Rational(0, 1)
        total = self.data[0]
        for r in self.data[1:]:
            total += r
        return total

    def __str__(self):
        return "[" + ", ".join(str(r) for r in self.data) + "]"

def read_rational_list_from_file(filename):
    rlist = RationalList()
    with open(filename, 'r') as file:
        for line in file:
            tokens = line.strip().split()
            for token in tokens:
                if '/' in token:
                    rlist.append(Rational(token))
                else:
                    rlist.append(int(token))
    return rlist

fname = ('input03 (1).txt')
print(f"\nФайл: {fname}")
rlist = read_rational_list_from_file(fname)
print(f"Список: {rlist}")
total = rlist.sum()
print(f"Сума: {total} = {total()}")

print("\nЕлементи списку при ітерації (за спаданням знаменника, потім чисельника):")
for r in rlist:
    print(r, end=" ")
print()
