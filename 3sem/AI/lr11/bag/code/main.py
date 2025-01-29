"""
1) Необходимо взять тексты и сформировать n-граммы для генерации с помощью цепи Маркова текстов. Рассмотреть n=2,3,5.
    1.1) Сформировать тексты с различными начальными словами и оценить "человечность" написанного.
2) Провести исследование возможности генерации текста при наличии обучающей выборки, состоящей только из предложений:
"кошка съела мышку" "мышку съела кошка".
    2.1) Оценить опасность работы с нестрогим порядком слов (не обязательно SVO)
Генерацию текстов осуществить с помощью инструмента из "методички?".
Оценить получаемый порядок слов в предложениях для разных версий "qwen2.5"
"""

import random
import re

from collections import defaultdict
from pathlib import Path
from itertools import permutations

WORK_DIR = (Path.cwd() / "..").resolve()

TEXT_DIR = WORK_DIR / "тексты"
IMG_DIR = WORK_DIR / "report" / "images"
RES_DIR = WORK_DIR / "generated"

def read_texts(directory_path):
    """Считывает все тексты из указанной директории."""
    texts = []
    for filepath in directory_path.rglob("texts/*.txt"):
        with open(filepath, "r", encoding="utf-8") as file:
            texts.append(file.read())
    if not texts:
        raise ValueError(f"Тесты не найдены в {directory_path}")
    return texts

def generate_ngrams(tokens, n):
    """Генерирует n-граммы из списка токенов."""
    return [tuple(tokens[i:i+n]) for i in range(len(tokens) - n + 1)]

def build_markov_chain(texts, n):
    """Создаёт цепь Маркова из текстов."""
    chain = defaultdict(list)
    for text in texts:
        tokens = [word.lower() for word in re.split(r"[-–.,!?():;\n\t\r\s]+", text) if word]
        ngrams = generate_ngrams(tokens, n)
        for i in range(len(ngrams) - 1):
            chain[ngrams[i]].append(ngrams[i + 1][-1])
    return chain

def find_start_state(chain, start_word):
    # Поиск н-граммы оканчивающуюся на start_word
    for n_gram, next_states in chain.items():
        if n_gram[0] == start_word:
            return n_gram


def generate_text(chain, n, start_word, max_words=50):
    """Генерирует текст с использованием цепи Маркова."""
    current = find_start_state(chain, start_word)

    if not current:
        return ""

    generated = list(current)
    for _ in range(max_words - len(generated)):
        if current in chain:
            next_word = random.choice(chain[current])
            generated.append(next_word)
            current = tuple(generated[-n:])
        else:
            break
    return " ".join(generated)


def create_not_SVO(sentence):
    builded = set()
    words = sentence.split()
    for perm in permutations(words):
        builded.add(" ".join(perm))

    return builded

def experiment_with_sentences(n):
    """Проводит эксперимент с предложениями из задания."""
    sentences = ["кошка съела мышку", "мышку съела кошка"]
    builded = create_not_SVO(sentences[0])

    chain = build_markov_chain(sentences, n)
    start_words = ["кошка", "мышку", "съела"]

    for start_word in start_words:
        generated_text = generate_text(chain, n, start_word, max_words=50)
        output_file = RES_DIR / f"{n}_grams_SVO_generated.txt"
        with open(output_file, "a", encoding="utf-8") as file:
            text = f"Начальное слово \"{start_word}\"\n {generated_text}\n"
            file.write(text)

    chain = build_markov_chain(builded, n)
    for start_word in start_words:
        generated_text = generate_text(chain, n, start_word, max_words=50)
        output_file = RES_DIR / f"{n}_grams_NOT_SVO_generated.txt"
        with open(output_file, "a", encoding="utf-8") as file:
            text = f"Начальное слово \"{start_word}\"\n {generated_text}\n"
            file.write(text)

def most_popular_words(texts, top_n=3):
    all_words = defaultdict(int)
    for text in texts:
        words = [word.lower() for word in re.split(r"[-–.,!?():;\n\t\r\s]+", text) if word]
        for word in words:
            all_words[word] += 1
    all_words = sorted(all_words.items(), key=lambda item: item[1], reverse=True)
    # print(all_words)
    return [pair[0] for pair in all_words[:top_n]]


def first_task():
    # Задание 1: Генерация текстов из директории
    n_values = [2, 3, 5]

    texts = read_texts(TEXT_DIR)
    words = most_popular_words(texts, 50)

    for n in n_values:
        chain = build_markov_chain(texts, n)
        start_words_list = words[:3] + ["как", "химии", "веществ"]
        for start_word in start_words_list:
            generated_text = generate_text(chain, n, start_word, max_words=50)

            output_file = RES_DIR / f"{n}_grams_generated.txt"
            with open(output_file, "a", encoding="utf-8") as file:
                text = f"Начальное слово \"{start_word}\"\n {generated_text}\n"
                file.write(text)


def second_task():
    # Задание 2: Эксперимент с фиксированными предложениями
    n_values = [2]
    for n in n_values:
        experiment_with_sentences(n)
        print()
        

if __name__ == "__main__":
    if not RES_DIR.exists():
        RES_DIR.mkdir(parents=True, exist_ok=True)
    else:
        for file in RES_DIR.iterdir():
            file.unlink()

    first_task()
    second_task()

