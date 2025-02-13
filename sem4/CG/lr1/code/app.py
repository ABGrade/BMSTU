import tkinter as tk
from tkinter import ttk, Frame


class App(tk.Tk):
    def __init__(self):
        super().__init__()
        self.title("Приложение")
        self.geometry("1200x800")
        self.minsize(400, 300)

        self.columnconfigure(0, weight=0)
        self.columnconfigure(1, weight=1)
        self.columnconfigure(2, weight=1)
        self.rowconfigure(0, weight=1)

        self.init_buttons()
        self.init_sidebar()
        self.init_table()

    def init_buttons(self):
        buttons_frame = ttk.Frame(self, padding=10)
        buttons_frame.grid(row=1, column=0, sticky="nsew")

        self.open_file_btn = ttk.Button(buttons_frame, text="Открыть файл", width=20, command=self.open_file_command)
        self.open_file_btn.grid(row=0, column=0, padx=2, pady=2, sticky="we")

        self.clear_btn = ttk.Button(buttons_frame, text="Очистить", width=20, command=self.clear)
        self.clear_btn.grid(row=1, column=0, padx=2, pady=2, sticky="we")

        self.add_point_btn = ttk.Button(buttons_frame, text="Добавить точку", width=20, command=self.add_point)
        self.add_point_btn.grid(row=0, column=1, padx=2, pady=2, sticky="we")

        self.del_point_btn = ttk.Button(buttons_frame, text="Удалить точку", width=20, command=self.del_point)
        self.del_point_btn.grid(row=1, column=1, padx=2, pady=2, sticky="we")

    def init_sidebar(self):
        sidebar = ttk.Frame(self, padding=10)
        sidebar.columnconfigure(0, weight=1)
        sidebar.grid(row=0, column=0, sticky="nsew")

        self.toggle_table_btn = ttk.Button(sidebar, text=">>", width=10, command=self.toggle_table)
        self.toggle_table_btn.grid(row=0, column=0, padx=2, pady=2, sticky="n")

    def init_table(self):
        self.table_vision = False

        self.table_frame = ttk.Frame(self, padding=10)
        self.table_frame.columnconfigure(0, weight=1)
        self.table_frame.rowconfigure(0, weight=1)
        self.table_frame.grid(row=0, column=2, padx=5, pady=5, sticky="nsew")

        self.table = ttk.Treeview(self.table_frame, columns=("ID", "X", "Y"), show="headings")
        self.table.heading("ID", text="№")
        self.table.heading("X", text="X")
        self.table.heading("Y", text="Y")
        self.table.grid(row=0, column=0, sticky="nsew")

        scroll = ttk.Scrollbar(self.table_frame, orient="vertical", command=self.table.yview)
        self.table.configure(yscrollcommand=scroll.set)
        scroll.grid(row=0, column=1, sticky="ns")

    #  


    def open_file_command(self):
        print("Открыто")

    def clear(self):
        print("Очищено")

    def add_point(self):
        print("Добавлено")

    def del_point(self):
        print("Удалено")

    def toggle_table(self):
        if self.table_vision:
            print("Скрыто")
            self.table_vision = False
            self.table_frame.grid_remove()
            self.columnconfigure(2, weight=0)
            self.toggle_table_btn.config(text="<<")
        else:
            print("Открыто")
            self.table_vision = True
            self.table_frame.grid()
            self.columnconfigure(2, weight=1)
            self.toggle_table_btn.config(text=">>")