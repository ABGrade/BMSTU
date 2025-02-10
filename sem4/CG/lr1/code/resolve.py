import math
import itertools

from classes import *

EPS = 1e-6
TRIANGLE_POITS = 3

def calc_angle_between_line_and_x(p1 : Point, p2 : Point) -> float:
    dx = p2.x - p1.x
    dy = p2.y - p1.y
    if dx < EPS:
        angle = 90.0
    else:
        angle = math.degrees(math.atan(abs(dy / dx)))
    return angle


def find_best_combination(points : [Point]):
    best_angle = None
    best_pairs = None
    for pairs in itertools.combinations(points, TRIANGLE_POITS):
