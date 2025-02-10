#!/usr/bin/env python3
# -*- coding: utf-8 -*-

"""
Решение геометрической задачи:
Дано множество точек. Нужно выбрать два треугольника (на 6 различных точек),
для которых прямая, проходящая через центры вписанных окружностей, образует с осью X максимально возможный угол.
Также реализованы функции отрисовки: точки, оси, треугольники, вписанные окружности и прямая, соединяющая инценры.
Реализована возможность редактирования точек (добавление, удаление, изменение координат).
Есть CLI‑и GUI‑версии приложения.
"""

import math, itertools
from tkinter import *
from tkinter import simpledialog, messagebox

# ==========================
# Геометрические примитивы
# ==========================

class Point:
    def __init__(self, x, y, index=None):
        self.x = x
        self.y = y
        self.index = index  # номер точки для подписи

    def __repr__(self):
        return f"Point({self.index}: {self.x}, {self.y})"

def distance(p1, p2):
    return math.hypot(p2.x - p1.x, p2.y - p1.y)

def triangle_area(A, B, C):
    return abs(A.x*(B.y - C.y) + B.x*(C.y - A.y) + C.x*(A.y - B.y)) / 2

def is_collinear(A, B, C, eps=1e-9):
    return triangle_area(A, B, C) < eps

class Triangle:
    def __init__(self, A, B, C):
        self.A = A
        self.B = B
        self.C = C
        self.points = (A, B, C)
        if is_collinear(A, B, C):
            raise ValueError("Треугольник вырожден: точки коллинеарны")
        self.incenter = self.compute_incenter()
        self.inradius = self.compute_inradius()

    def compute_incenter(self):
        # Стороны: a = |BC|, b = |AC|, c = |AB|
        a = distance(self.B, self.C)
        b = distance(self.A, self.C)
        c = distance(self.A, self.B)
        sum_lengths = a + b + c
        x = (self.A.x * a + self.B.x * b + self.C.x * c) / sum_lengths
        y = (self.A.y * a + self.B.y * b + self.C.y * c) / sum_lengths
        return Point(x, y)

    def compute_inradius(self):
        a = distance(self.B, self.C)
        b = distance(self.A, self.C)
        c = distance(self.A, self.B)
        s = (a + b + c) / 2
        A_area = triangle_area(self.A, self.B, self.C)
        return A_area / s

    def __repr__(self):
        return f"Triangle({self.A}, {self.B}, {self.C})"

def line_angle_with_x(p1, p2):
    """
    Вычисляет угол (в градусах) между прямой, соединяющей точки p1 и p2, и осью X.
    Если dx ~ 0, то угол считается равным 90°.
    """
    dx = p2.x - p1.x
    dy = p2.y - p1.y
    if abs(dx) < 1e-9:
        return 90.0
    angle = math.degrees(math.atan(abs(dy/dx)))
    return angle

# ==========================
# Алгоритм перебора
# ==========================

def find_best_triangle_pair(points):
    """
    Из заданного списка точек перебирает все пары треугольников (на 6 различных точек),
    вычисляет угол между прямой, проходящей через их инценры, и осью X,
    и возвращает пару треугольников с максимальным углом и само значение угла.
    """
    best_angle = -1
    best_pair = None
    n = len(points)
    # Перебор трёх точек для первого треугольника
    for tri1_indices in itertools.combinations(range(n), 3):
        A, B, C = points[tri1_indices[0]], points[tri1_indices[1]], points[tri1_indices[2]]
        try:
            tri1 = Triangle(A, B, C)
        except ValueError:
            continue  # вырожденный треугольник, пропускаем
        # Оставшиеся точки для второго треугольника
        remaining_indices = [i for i in range(n) if i not in tri1_indices]
        for tri2_indices in itertools.combinations(remaining_indices, 3):
            A2, B2, C2 = points[tri2_indices[0]], points[tri2_indices[1]], points[tri2_indices[2]]
            try:
                tri2 = Triangle(A2, B2, C2)
            except ValueError:
                continue
            angle = line_angle_with_x(tri1.incenter, tri2.incenter)
            if angle > best_angle:
                best_angle = angle
                best_pair = (tri1, tri2)
    return best_pair, best_angle

# ==========================
# Модуль отрисовки
# ==========================

class CanvasRenderer:
    def __init__(self, canvas, width, height):
        self.canvas = canvas
        self.width = width
        self.height = height
        # epsilon (поле) – 10% от минимального размера холста
        self.margin = 0.1 * min(width, height)
        self.set_bounds([])

    def set_bounds(self, extra_points):
        """
        Вычисляет границы по всем координатам, учитывая:
         - точки extra_points (список кортежей (x,y)),
         - центр координат (0,0).
        Эти границы используются для преобразования декартовых координат в координаты холста.
        """
        all_x = [p[0] for p in extra_points]
        all_y = [p[1] for p in extra_points]
        all_x.append(0)
        all_y.append(0)
        if all_x:
            self.xmin = min(all_x)
            self.xmax = max(all_x)
        else:
            self.xmin, self.xmax = -10, 10
        if all_y:
            self.ymin = min(all_y)
            self.ymax = max(all_y)
        else:
            self.ymin, self.ymax = -10, 10
        if abs(self.xmax - self.xmin) < 1e-9:
            self.xmax += 1; self.xmin -= 1
        if abs(self.ymax - self.ymin) < 1e-9:
            self.ymax += 1; self.ymin -= 1
        self.dx = (self.width - 2*self.margin) / (self.xmax - self.xmin)
        self.dy = (self.height - 2*self.margin) / (self.ymax - self.ymin)

    def to_canvas_coords(self, x, y):
        """Преобразует декартовы координаты (x,y) в координаты холста."""
        cx = self.margin + (x - self.xmin) * self.dx
        # Учёт того, что ось Y холста направлена вниз, а нам нужно – вверх:
        cy = self.height - self.margin - (y - self.ymin) * self.dy
        return (cx, cy)

    def draw_point(self, point, color="black"):
        cx, cy = self.to_canvas_coords(point.x, point.y)
        r = 3
        self.canvas.create_oval(cx - r, cy - r, cx + r, cy + r, fill=color)
        # Отрисовка подписи: номер и координаты
        self.canvas.create_text(cx + 10, cy, text=f"{point.index}({point.x:.2f},{point.y:.2f})",
                                anchor="w", fill=color)

    def draw_line(self, p1, p2, color="blue"):
        c1 = self.to_canvas_coords(p1.x, p1.y)
        c2 = self.to_canvas_coords(p2.x, p2.y)
        self.canvas.create_line(c1[0], c1[1], c2[0], c2[1], fill=color, width=2)

    def draw_polygon(self, points, outline="black", fill="", width=1):
        coords = []
        for pt in points:
            c = self.to_canvas_coords(pt.x, pt.y)
            coords.extend(c)
        self.canvas.create_polygon(coords, outline=outline, fill=fill, width=width)

    def draw_circle(self, center, radius, outline="red", width=1):
        c_center = self.to_canvas_coords(center.x, center.y)
        # Преобразуем радиус: используем среднее масштабирование по x и y.
        scale = (self.dx + self.dy) / 2
        r_canvas = radius * scale
        self.canvas.create_oval(c_center[0] - r_canvas, c_center[1] - r_canvas,
                                c_center[0] + r_canvas, c_center[1] + r_canvas,
                                outline=outline, width=width)

# ==========================
# Модель данных (точки)
# ==========================

class GeometryModel:
    def __init__(self, points=None):
        if points is None:
            points = []
        self.points = points
        # Присваиваем номера точкам, если не заданы
        for i, p in enumerate(self.points):
            p.index = i

    def add_point(self, x, y):
        index = len(self.points)
        p = Point(x, y, index)
        self.points.append(p)

    def remove_point(self, index):
        self.points = [p for p in self.points if p.index != index]
        # Перенумеруем точки
        for i, p in enumerate(self.points):
            p.index = i

    def update_point(self, index, x, y):
        for p in self.points:
            if p.index == index:
                p.x = x
                p.y = y
                break

    def get_all_points(self):
        return self.points

# ==========================
# CLI‑приложение
# ==========================

def run_cli(model):
    print("CLI‑приложение для решения геометрической задачи")
    while True:
        print("\nТекущий набор точек:")
        for p in model.get_all_points():
            print(f"{p.index}: ({p.x}, {p.y})")
        print("Доступные команды: add, remove, update, solve, exit")
        cmd = input("Введите команду: ").strip().lower()
        if cmd == "add":
            try:
                x = float(input("Введите x: "))
                y = float(input("Введите y: "))
                model.add_point(x, y)
            except Exception as e:
                print("Ошибка:", e)
        elif cmd == "remove":
            try:
                idx = int(input("Введите номер точки для удаления: "))
                model.remove_point(idx)
            except Exception as e:
                print("Ошибка:", e)
        elif cmd == "update":
            try:
                idx = int(input("Введите номер точки для изменения: "))
                x = float(input("Введите новое x: "))
                y = float(input("Введите новое y: "))
                model.update_point(idx, x, y)
            except Exception as e:
                print("Ошибка:", e)
        elif cmd == "solve":
            if len(model.points) < 6:
                print("Недостаточно точек для построения двух треугольников (не менее 6 точек)")
            else:
                best_pair, best_angle = find_best_triangle_pair(model.points)
                if best_pair is None:
                    print("Не удалось найти корректную пару треугольников")
                else:
                    tri1, tri2 = best_pair
                    print("Найдена пара треугольников:")
                    print("Треугольник A:", [(p.index, p.x, p.y) for p in tri1.points])
                    print("Инцентр A: ({:.2f}, {:.2f})".format(tri1.incenter.x, tri1.incenter.y))
                    print("Треугольник B:", [(p.index, p.x, p.y) for p in tri2.points])
                    print("Инцентр B: ({:.2f}, {:.2f})".format(tri2.incenter.x, tri2.incenter.y))
                    print("Угол между прямой, соединяющей инценры, и осью X: {:.2f}°".format(best_angle))
        elif cmd == "exit":
            break
        else:
            print("Неизвестная команда.")

# ==========================
# GUI‑приложение (Tkinter)
# ==========================

class GeometryGUI:
    def __init__(self, root, model):
        self.root = root
        self.model = model
        self.root.title("Решатель геометрической задачи")
        self.canvas = Canvas(root, bg="white")
        self.canvas.pack(fill=BOTH, expand=YES)
        # При изменении размера окна вызываем перерисовку
        self.canvas.bind("<Configure>", self.on_resize)
        self.selected_point = None
        self.canvas.bind("<Button-1>", self.on_canvas_click)

        # Панель управления
        frame = Frame(root)
        frame.pack(side=TOP, fill=X)
        self.add_button = Button(frame, text="Добавить точку", command=self.add_point)
        self.add_button.pack(side=LEFT)
        self.remove_button = Button(frame, text="Удалить точку", command=self.remove_point)
        self.remove_button.pack(side=LEFT)
        self.update_button = Button(frame, text="Изменить точку", command=self.update_point)
        self.update_button.pack(side=LEFT)
        self.solve_button = Button(frame, text="Найти решение", command=self.solve)
        self.solve_button.pack(side=LEFT)
        self.info_label = Label(frame, text="Выберите точку щёлчком")
        self.info_label.pack(side=LEFT)

        self.redraw()

    def on_resize(self, event):
        self.redraw()

    def on_canvas_click(self, event):
        # Определяем, какая точка (если есть) была выбрана
        x, y = event.x, event.y
        renderer = self.get_renderer()
        for p in self.model.get_all_points():
            cx, cy = renderer.to_canvas_coords(p.x, p.y)
            if abs(cx - x) < 5 and abs(cy - y) < 5:
                self.selected_point = p
                self.info_label.config(text=f"Выбрана точка {p.index}: ({p.x:.2f}, {p.y:.2f})")
                return
        self.selected_point = None
        self.info_label.config(text="Точка не выбрана")

    def get_renderer(self):
        # Собираем все координаты: точки модели и центр координат (0,0)
        pts = [(p.x, p.y) for p in self.model.get_all_points()]
        pts.append((0, 0))
        renderer = CanvasRenderer(self.canvas, self.canvas.winfo_width(), self.canvas.winfo_height())
        renderer.set_bounds(pts)
        return renderer

    def redraw(self):
        self.canvas.delete("all")
        renderer = self.get_renderer()
        # Отрисовка осей
        x_axis_start = Point(renderer.xmin, 0)
        x_axis_end = Point(renderer.xmax, 0)
        renderer.draw_line(x_axis_start, x_axis_end, color="gray")
        y_axis_start = Point(0, renderer.ymin)
        y_axis_end = Point(0, renderer.ymax)
        renderer.draw_line(y_axis_start, y_axis_end, color="gray")
        # Отрисовка всех точек
        for p in self.model.get_all_points():
            renderer.draw_point(p, color="black")
        # Если достаточно точек, находим решение и отрисовываем выделенные фигуры
        if len(self.model.points) >= 6:
            best_pair, best_angle = find_best_triangle_pair(self.model.points)
            if best_pair is not None:
                tri1, tri2 = best_pair
                # Треугольники (контур)
                renderer.draw_polygon(tri1.points, outline="blue", fill="", width=2)
                renderer.draw_polygon(tri2.points, outline="green", fill="", width=2)
                # Вписанные окружности
                renderer.draw_circle(tri1.incenter, tri1.inradius, outline="blue", width=2)
                renderer.draw_circle(tri2.incenter, tri2.inradius, outline="green", width=2)
                # Прямая, соединяющая инценры
                renderer.draw_line(tri1.incenter, tri2.incenter, color="red")
                # Информация об угле
                self.canvas.create_text(10, 10, anchor="nw",
                                        text=f"Угол: {best_angle:.2f}°", fill="red")

    def add_point(self):
        try:
            x = float(simpledialog.askstring("Ввод", "Введите x:", parent=self.root))
            y = float(simpledialog.askstring("Ввод", "Введите y:", parent=self.root))
            self.model.add_point(x, y)
            self.redraw()
        except Exception as e:
            messagebox.showerror("Ошибка", str(e))

    def remove_point(self):
        if self.selected_point is None:
            messagebox.showinfo("Информация", "Точка не выбрана")
            return
        self.model.remove_point(self.selected_point.index)
        self.selected_point = None
        self.redraw()

    def update_point(self):
        if self.selected_point is None:
            messagebox.showinfo("Информация", "Точка не выбрана")
            return
        try:
            x = float(simpledialog.askstring("Ввод", "Введите новое x:", parent=self.root))
            y = float(simpledialog.askstring("Ввод", "Введите новое y:", parent=self.root))
            self.model.update_point(self.selected_point.index, x, y)
            self.redraw()
        except Exception as e:
            messagebox.showerror("Ошибка", str(e))

    def solve(self):
        if len(self.model.points) < 6:
            messagebox.showinfo("Информация", "Недостаточно точек (требуется не менее 6)")
        else:
            best_pair, best_angle = find_best_triangle_pair(self.model.points)
            if best_pair is None:
                messagebox.showinfo("Информация", "Не удалось построить корректные треугольники")
            else:
                tri1, tri2 = best_pair
                info = "Треугольник A: " + ", ".join([f"{p.index}({p.x:.2f},{p.y:.2f})" for p in tri1.points]) + "\n"
                info += "Инцентр A: " + f"({tri1.incenter.x:.2f}, {tri1.incenter.y:.2f})" + "\n"
                info += "Треугольник B: " + ", ".join([f"{p.index}({p.x:.2f},{p.y:.2f})" for p in tri2.points]) + "\n"
                info += "Инцентр B: " + f"({tri2.incenter.x:.2f}, {tri2.incenter.y:.2f})" + "\n"
                info += f"Угол между прямой, соединяющей инценры, и осью X: {best_angle:.2f}°"
                messagebox.showinfo("Результат", info)
            self.redraw()

# ==========================
# Основная программа
# ==========================

if __name__ == "__main__":
    import sys
    # Пример начального набора точек
    initial_points = [
        Point(10, 10),
        Point(100, 20),
        Point(50, 80),
        Point(20, 90),
        Point(80, 60),
        Point(60, 30),
        Point(30, 50)
    ]
    model = GeometryModel(initial_points)
    # Если запущено с параметром "cli", то запускается консольное приложение,
    # иначе – графический интерфейс.
    if len(sys.argv) > 1 and sys.argv[1] == "cli":
        run_cli(model)
    else:
        root = Tk()
        app = GeometryGUI(root, model)
        root.mainloop()
