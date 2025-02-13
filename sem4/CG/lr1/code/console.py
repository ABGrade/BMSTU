from resolve import find_best_combination, read_points_from_file

MENU = """
Выберите действие:
1 - прочитать из файла
0 - выход"""

def console_menu():
    points = []
    print(MENU)
    action = int(input(":"))
    while action:
        if action == 1:
            filename = input("Введите имя файла:")
            points = read_points_from_file(filename)
            pair, angle = find_best_combination(points)
            print(pair[0], pair[1])
            print(f"angle = {angle}")
        elif action == 0:
            break
        print(MENU)
        action = int(input(":"))
    print("Выходим...")
