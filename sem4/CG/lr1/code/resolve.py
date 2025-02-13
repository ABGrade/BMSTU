import itertools

from classes import *

EPS = 1e-6
TRIANGLE_POINTS = 3

def read_points_from_file(filename : str) -> [Point]:
    points = []
    with open(filename, "r") as file:
        for i, line in enumerate(file, start=1):
            if not line.strip():
                continue
            try:
                xy = list(map(int, line.split()))
                if len(xy) != 2:
                    raise ValueError("Считалось более двух значений. Возможна ошибка в исходном файле")
                points.append(Point(xy[0], xy[1], i))
            except:
                print(f"Считывание прервано на {i} строке")
                continue
    return points

def calc_angle_between_line_and_x(p1 : Point, p2 : Point) -> float:
    dx = p2.x - p1.x
    dy = p2.y - p1.y
    if dx < EPS:
        angle = 90.0
    else:
        angle = math.degrees(math.atan(abs(dy / dx)))
    return angle


def find_best_combination(points):
    best_angle = None
    best_pair = None

    len_points = len(points)
    for tr1_indices in itertools.combinations(range(len_points), 3):
        try:
            tr1 = Triangle(points[tr1_indices[0]], points[tr1_indices[1]], points[tr1_indices[2]])
        except ValueError:
            continue
        remain = [i for i in range(len_points) if i not in tr1_indices]

        for tr2_indices in itertools.combinations(remain, 3):
            try:
                tr2 = Triangle(points[tr2_indices[0]], points[tr2_indices[1]], points[tr2_indices[2]])
            except ValueError:
                continue
            angle = calc_angle_between_line_and_x(tr1.inner_circle.coord, tr2.inner_circle.coord)
            if best_angle is None:
                best_angle = angle
                best_pair = (tr1, tr2)
            elif angle > best_angle:
                best_angle = angle
                best_pair = (tr1, tr2)
    return best_pair, best_angle