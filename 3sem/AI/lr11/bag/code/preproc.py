import os
import re

from bs4 import BeautifulSoup
from docx import Document
from odf.opendocument import load
from odf.text import P

DIR = os.path.join(os.getcwd(), "..\тексты")
EXT = (".txt", ".docx", ".odt", ".html")

replacements = {
    r" ": " ",
    r"\s{2,}" : " "
}



def build_texts_recursively(directory):
    texts = list()
    file_order = list()
    for folder in os.listdir(directory):
        folder_path = os.path.join(directory, folder)
        texts_dir_path = os.path.join(folder_path, "texts")

        if not os.path.exists(texts_dir_path):
            os.mkdir(texts_dir_path)

        for filename in os.listdir(folder_path):
            file_path = os.path.join(folder_path, filename)
            if file_path.endswith(EXT):
                text = ext_choice(file_path)
                if text is None:
                    print(f"None pointer return for file: {file_path}\n")
                    continue
                file_order.append(file_path)
                texts.append(text)

    for i, file in enumerate(file_order):
        texts_dir_path = os.path.join(file, "..", "texts")
        write_into_file(texts[i], texts_dir_path, file.split("\\")[-1])


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


def write_into_file(text, vector_dir, file_name):
    file_name = os.path.splitext(file_name)[0] + ".txt"
    file_path = os.path.join(vector_dir, file_name)
    with open(file_path, "w", encoding="utf-8") as file:
        text = text.strip()
        for pattern, replacement in replacements.items():
            text = re.sub(pattern, replacement, text)
        file.write(f"{text}\n")


build_texts_recursively(DIR)
