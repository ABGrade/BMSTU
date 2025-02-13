import math

class Point:
    def __init__(self, x=0.0, y=0.0, index=None):
        self.x = x
        self.y = y
        self.index = index

    def __eq__(self, other : "Point") -> bool:
        return self.x == other.x and self.y == other.y

    def __str__(self):
        return f"x = {self.x}, y = {self.y}, ind = {self.index}"

    @staticmethod
    def distance(p1 : "Point", p2 : "Point") -> float:
        dx = p1.x - p2.x
        dy = p1.y - p2.y
        return math.sqrt((dx ** 2 + dy ** 2))


class Circle:
    def __init__(self, point : Point, r=0.0):
        self.coord = point
        self.r = r

    def __str__(self):
        return f"{self.coord}, r = {self.r}"


class Triangle:
    def __init__(self, a : Point, b : Point, c : Point):
        self.a = a
        self.b = b
        self.c = c

        self.area = self.calc_area()

        if not self.is_exist():
            raise ValueError("Треугольник не может существовать с выбранными точками")
        self.inner_circle = self.find_incenter()

    def __str__(self):
        return (f"a = {self.a}\n"
                f"b = {self.b}\n"
                f"c = {self.c}\n"
                f"area = {self.area}\n")

    def calc_area(self) -> float:
        return 0.5 * abs((self.a.x * (self.b.y - self.c.y) +
                         self.b.x * (self.c.y - self.a.y) +
                         self.c.x * (self.a.y - self.b.y)))


    def is_exist(self) -> bool:
        return self.area > 0


    def find_incenter(self) -> Circle:
        l_bc = Point.distance(self.b, self.c)
        l_ac = Point.distance(self.a, self.c)
        l_ab = Point.distance(self.a, self.b)
        x = (l_bc * self.a.x + l_ac * self.b.x + l_ab * self.c.x) / (l_bc + l_ac + l_ab)
        y = (l_bc * self.a.y + l_ac * self.b.y + l_ab * self.c.y) / (l_bc + l_ac + l_ab)

        p = 0.5 * (l_bc + l_ac + l_ab)
        r = self.area * p
        return Circle(Point(x, y), r)

