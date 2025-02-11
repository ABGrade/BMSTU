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


class Triangle:
    def __init__(self, a : Point, b : Point, c : Point):
        self.a = a
        self.b = b
        self.c = c
        if not self.is_exist():
            raise ValueError("Треугольник не может существовать с выбранными точками")
        self.incenter = self.find_incenter()

    def __str__(self):
        return (f"a = {self.a}\n"
                f"b = {self.b}\n"
                f"c = {self.c}\n"
                f"area = {self.area()}\n")

    def area(self) -> float:
        return 0.5 * abs((self.a.x * (self.b.y - self.c.y) +
                         self.b.x * (self.c.y - self.a.y) +
                         self.c.x * (self.a.y - self.b.y)))

    def is_exist(self) -> bool:
        return self.area() > 0


    def find_incenter(self) -> Point:
        la = Point.distance(self.b, self.c)
        lb = Point.distance(self.a, self.c)
        lc = Point.distance(self.a, self.b)
        ix = (la * self.a.x + lb * self.b.x + lc * self.c.x) / (la + lb + lc)
        iy = (la * self.a.y + lb * self.b.y + lc * self.c.y) / (la + lb + lc)
        return Point(ix, iy)

