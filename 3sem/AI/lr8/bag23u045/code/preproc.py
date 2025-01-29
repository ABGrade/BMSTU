import os
import re
import pymorphy3

from collections import defaultdict

from bs4 import BeautifulSoup
from docx import Document
from odf.opendocument import load
from odf.text import P

DIR = os.path.join(os.getcwd(), "../тексты")
EXT = (".txt", ".docx", ".odt", ".html")


def build_vectors_recursively(directory):
    for folder in os.listdir(directory):
        folder_path = os.path.join(directory, folder)
        vectors_dir_path = os.path.join(folder_path, "vectors")
        norm_vectors_dir_path = os.path.join(folder_path, "norm_vectors")

        if not os.path.exists(vectors_dir_path):
            os.mkdir(vectors_dir_path)

        if not os.path.exists(norm_vectors_dir_path):
            os.mkdir(norm_vectors_dir_path)

        for filename in os.listdir(folder_path):
            file_path = os.path.join(folder_path, filename)
            if file_path.endswith(EXT):
                text = ext_choice(file_path)
                if text is None:
                    print(f"None pointer return for file: {file_path}\n")
                    continue
                vec, vec_norm = build_vectors(text)
                write_into_file(vec, vectors_dir_path, filename)
                write_into_file(vec_norm, norm_vectors_dir_path, filename)


def build_vectors(text):
    morph = pymorphy3.MorphAnalyzer()
    words = list(word.lower() for word in re.split(r"[ -.,!?():;\n\t\r]+", text) if word)
    word_count = defaultdict(int)
    word_count_norm = defaultdict(int)
    for word in words:
        word_count[word] += 1
        word_count_norm[morph.normal_forms(word)[0]] += 1
    return word_count, word_count_norm


def extract_txt(file_path):
    try:
        with open(file_path, "r", encoding="utf-8") as file:
            text = file.read()
        return text
    except UnicodeDecodeError:
        try:
            with open(file_path, "r", encoding="windows-1251") as file:
                text = file.read()
            return text
        except Exception as e:
            print(f"Возникла ошибка во время считывания файла 1! {e}\n")
            return None
    except Exception as e:
        print(f"Возникла ошибка во время считывания файла 2! {e}\n")
        return None


def extract_docx(file_path):
    try:
        doc = Document(file_path)
        text = "".join(parag.text for parag in doc.paragraphs)
        return text
    except Exception as e:
        print(f"Возникла ошибка во время считывания файла! {e}\n")
        return None


def extract_odt(file_path):
    try:
        doc = load(file_path)
        text = "".join(str(parag) for parag in doc.getElementsByType(P))
        return text
    except Exception as e:
        print(f"Возникла ошибка во время считывания файла! {e}\n")
        return None


def extract_html(file_path):
    try:
        with open(file_path, "r", encoding="utf-8") as file:
            soup = BeautifulSoup(file, "html.parser")
            text = soup.get_text()
        return text
    except Exception as e:
        print(f"Возникла ошибка во время считывания файла! {e}\n")
        return None


def ext_choice(file_path):
    if file_path.endswith(".txt"):
        return extract_txt(file_path)
    elif file_path.endswith(".docx"):
        return extract_docx(file_path)
    elif file_path.endswith(".odt"):
        return extract_odt(file_path)
    elif file_path.endswith(".html"):
        return extract_html(file_path)
    else:
        print("Неподдерживаемый формат файла")
        return None


def write_into_file(vector, vector_dir, file_name):
    file_name = os.path.splitext(file_name)[0] + ".txt"
    file_path = os.path.join(vector_dir, file_name)
    with open(file_path, "w", encoding="utf-8") as file:
        for key, amount in vector.items():
            file.write(f"{key} {amount}\n")


build_vectors_recursively(DIR)