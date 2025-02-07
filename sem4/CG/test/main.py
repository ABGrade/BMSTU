from PIL import Image, ImageDraw

FACE_COLOR = '#a7caef'
FACE_OUTLINE_COLOR = '#2d24b3'

SPOT_UPPER_COLOR = 'white'
SPOT_LOWER_COLOR = '#7f7cc1'

MOUTH_COLOR = 'black'
TEETH_COLOR = 'white'
DIRT_COLOR = '#3d4a15'


WIDTH = 600
LENGTH = 600

image = Image.new('RGB', (WIDTH, LENGTH), 'white')
draw = ImageDraw.Draw(image)


def relative_to_actual(relative_point):
    x = int(relative_point[0] * WIDTH)
    y = int(relative_point[1] * LENGTH)
    xy = (x, y)
    return xy


def draw_circle(p0_relative, p1_relative, width=0.02):
    p0 = relative_to_actual(p0_relative)
    p1 = relative_to_actual(p1_relative)
    draw.ellipse([p0, p1], fill=FACE_COLOR, outline=FACE_OUTLINE_COLOR, width=int(width * min(WIDTH, LENGTH)))


def draw_bezier_curve(p0_relative, p1_relative, p2_relative, p3_relative, fill_color='blue', width=5, steps=20):
    """
    Кубическая кривая Безье.

    p0, p3 - Начальные и конечные точки соответственно
    p1, p2 - Контрольные точки
    """
    p0 = relative_to_actual(p0_relative)
    p1 = relative_to_actual(p1_relative)
    p2 = relative_to_actual(p2_relative)
    p3 = relative_to_actual(p3_relative)
    points = []
    for i in range(steps + 1):
        t = i / steps
        x =  p0[0] * (1 - t)**3 + p1[0] * 3 * (1 - t)**2 * t + p2[0] * 3 * (1 - t) * t**2 + p3[0] * t**3
        y =  p0[1] * (1 - t)**3 + p1[1] * 3 * (1 - t)**2 * t + p2[1] * 3 * (1 - t) * t**2 + p3[1] * t**3
        points.append((x, y))
    draw.line(points, fill=fill_color, width=width)


def fill_bezier_curve(p0_upper, p1_upper, p2_upper, p3_upper,
                      p0_lower, p1_lower, p2_lower, p3_lower,
                      fill_color='blue', steps=20):
    points_upper = []
    for i in range(steps + 1):
        t = i / steps
        x = p0_upper[0] * (1 - t) ** 3 + p1_upper[0] * 3 * (1 - t) ** 2 * t + p2_upper[0] * 3 * (1 - t) * t ** 2 + p3_upper[0] * t ** 3
        y = p0_upper[1] * (1 - t) ** 3 + p1_upper[1] * 3 * (1 - t) ** 2 * t + p2_upper[1] * 3 * (1 - t) * t ** 2 + p3_upper[1] * t ** 3
        points_upper.append((x, y))

    points_lower = []
    for i in range(steps, -1, -1):
        t = i / steps
        x = p0_lower[0] * (1 - t) ** 3 + p1_lower[0] * 3 * (1 - t) ** 2 * t + p2_lower[0] * 3 * (1 - t) * t ** 2 + \
            p3_lower[0] * t ** 3
        y = p0_lower[1] * (1 - t) ** 3 + p1_lower[1] * 3 * (1 - t) ** 2 * t + p2_lower[1] * 3 * (1 - t) * t ** 2 + \
            p3_lower[1] * t ** 3
        points_lower.append((x, y))

    all_points = points_upper + points_lower + [p0_upper]
    draw.polygon(all_points, fill=fill_color)


def draw_bezier_points(p0_relative, p1_relative, p2_relative, p3_relative, fill_color='black'):
    p0 = relative_to_actual(p0_relative)
    p1 = relative_to_actual(p1_relative)
    p2 = relative_to_actual(p2_relative)
    p3 = relative_to_actual(p3_relative)
    draw.ellipse((p0[0] - 2, p0[1] - 2, p0[0] + 2, p0[1] + 2), fill=fill_color)
    draw.ellipse((p1[0] - 2, p1[1] - 2, p1[0] + 2, p1[1] + 2), fill=fill_color)
    draw.ellipse((p2[0] - 2, p2[1] - 2, p2[0] + 2, p2[1] + 2), fill=fill_color)
    draw.ellipse((p3[0] - 2, p3[1] - 2, p3[0] + 2, p3[1] + 2), fill=fill_color)


def draw_triangle(p0, p1, p2, fill_color=None, outline_color=None):
    """
    p0, p1, p2 - Вершины треугольника
    """
    draw.polygon([p0, p1, p2], fill=fill_color, outline=outline_color)


draw_circle((0.066, 0.066), (0.933, 0.933))


# base_points = [20, 20, 280, 280]
#
# draw.ellipse(base_points, fill=FACE_COLOR, outline=FACE_OUTLINE_COLOR, width=10)


p0 = (270, 170)
p1 = (250, 160)
p2 = (170, 275)
p3 = (130, 260)

p0 = (0.9, 0.566)
p1 = (0.833, 0.533)
p2 = (0.566, 0.916)
p3 = (0.433, 0.866)

# p0_l = (270, 190)
# p1_l = (250, 180)
# p2_l = (170, 295)
# p3_l = (130, 280)
#
draw_bezier_curve(p0, p1, p2, p3, width=2)
# fill_bezier_curve(p0, p1, p2, p3,
#                   p0_l, p1_l, p2_l, p3_l,
#                   fill_color='purple', steps=50)

draw_bezier_points(p0, p1, p2, p3)
#
# p0 = (130, 260)
# p1 = (110, 270)
# p2 = (75, 250)
# p3 = (70, 220)
#
# draw_bezier_curve(p0, p1, p2, p3, width=2)
# draw_bezier_points(p0, p1, p2, p3, fill_color='green')
#
# p0 = (70, 220)
# p1 = (85, 195)
# p2 = (85, 195)
# p3 = (65, 180)
#
# draw_bezier_curve(p0, p1, p2, p3, width=2)
# draw_bezier_points(p0, p1, p2, p3, fill_color='red')
#
# p0 = (65, 180)
# p1 = (45, 180)
# p2 = (45, 180)
# p3 = (40, 195)
#
# draw_bezier_curve(p0, p1, p2, p3, width=2)
# draw_bezier_points(p0, p1, p2, p3, fill_color='orange')

image.save('figure.png')
image.show()