import os

import matplotlib.pyplot as plt
import numpy as np
import seaborn as sea

import metrics

DIR = os.path.join(os.getcwd(), "тексты")
RES_DIR = os.path.join(os.getcwd(), "results")


def build_heatmaps_recursive(directory):
    vectors = list()
    norm_vectors = list()
    files = list()
    for folder in os.listdir(directory):
        folder_path = os.path.join(directory, folder)
        vectors_dir_path = os.path.join(folder_path, "vectors")
        norm_vectors_dir_path = os.path.join(folder_path, "norm_vectors")
        for filename in os.listdir(vectors_dir_path):
            files.append(filename)
            file_path = os.path.join(vectors_dir_path, filename)
            file_path_norm = os.path.join(norm_vectors_dir_path, filename)
            vectors.append(get_vectors(file_path))
            norm_vectors.append(get_vectors(file_path_norm))

    matrix_jacquard, matrix_cosine = get_corr_matrix(vectors)
    plt.figure(figsize=(19.2, 10.8))
    sea.heatmap(matrix_jacquard, xticklabels=files, yticklabels=files, square=True, linewidths=.3, cmap="YlGnBu")
    plt.savefig(os.path.join(RES_DIR, "HeatmapJac.png"), dpi=100)
    plt.clf()

    plt.figure(figsize=(19.2, 10.8))
    sea.heatmap(matrix_cosine, xticklabels=files, yticklabels=files, square=True, linewidths=.3, cmap="YlGnBu")
    plt.savefig(os.path.join(RES_DIR, "HeatmapCos.png"), dpi=100)
    plt.clf()

    plt.figure(figsize=(19.2, 10.8))
    matrix_jacquard, matrix_cosine = get_corr_matrix(norm_vectors)
    sea.heatmap(matrix_jacquard, xticklabels=files, yticklabels=files, square=True, linewidths=.3, cmap="YlGnBu")
    plt.savefig(os.path.join(RES_DIR, "HeatmapJacNorm.png"), dpi=100)
    plt.clf()

    plt.figure(figsize=(19.2, 10.8))
    sea.heatmap(matrix_cosine, xticklabels=files, yticklabels=files, square=True, linewidths=.3, cmap="YlGnBu")
    plt.savefig(os.path.join(RES_DIR, "HeatmapCosNorm.png"), dpi=100)


def get_vectors(file_path):
    vectors = {}
    with open(file_path, "r", encoding="utf-8") as file:
        lines = file.read().splitlines()
        for i in range(len(lines)):
            try:
                item, amount = lines[i].split(" ")
                vectors[item] = int(amount)
            except Exception as e:
                print(f"Ошибка считывания на {file_path} line#{i + 1}\n {e}\n")
    return vectors


def get_corr_matrix(vectors):
    vecs_len = len(vectors)
    na = np.zeros((vecs_len, vecs_len))
    nb = np.zeros((vecs_len, vecs_len))
    for i in range(vecs_len):
        for j in range(vecs_len):
            na[i][j] = metrics.jacquard_similarity(vectors[i], vectors[j])
            nb[i][j] = metrics.cosine_metric(vectors[i], vectors[j])
    return na, nb


if not os.path.exists(RES_DIR):
    os.mkdir(RES_DIR)

build_heatmaps_recursive(DIR)