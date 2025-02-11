"""
43. Дано множество точек. Найти такие два треугольника A и B, что прямая, проходящая через центры вписанных окружностей,
образует с осью абсцисс максимальный угол. Никакие две точки обоих треугольников не совпадают.
"""

from classes import *
from resolve import find_best_combination

def read_points_from_file(filename : str):
    points = []
    with open(filename, "r") as file:
        for i, line in enumerate(file, start=1):
            if not line.strip():
                continue
            try:
                xy = list(map(int, line.split()))
                if len(xy) != 2:
                    raise ValueError("Считалось более двух значений. Возможна ошибка в исходном файле")
                points.append(Point(xy[0], xy[1]))
            except:
                print(f"Считывание прервано на {i} строке")
                continue
    return points


if __name__ == "__main__":
    points = read_points_from_file("data.txt")
    pair, angle = find_best_combination(points)
    print(pair[0], pair[1])
    print(angle)