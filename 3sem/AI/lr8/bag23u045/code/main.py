import os

import matplotlib.pyplot as plt
import numpy as np
import seaborn as sea
from sklearn.metrics.pairwise import cosine_similarity

import metrics

DIR = os.path.join(os.getcwd(), "../тексты")
RES_DIR = os.path.join(os.getcwd(), "../results")


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

    v1_transformed, v2_transformed, v1s_values, v2s_values = metrics.call_pca(vectors, norm_vectors)
    create_heatmaps(v1_transformed, v2_transformed, v1s_values, v2s_values, files)


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


def get_corr_matrix(vectors, trans=None):
    vecs_len = len(vectors)
    na = np.zeros((vecs_len, vecs_len))
    nb = np.zeros((vecs_len, vecs_len))
    nc = np.zeros((vecs_len, vecs_len))
    for i in range(vecs_len):
        for j in range(vecs_len):
            na[i][j] = metrics.pearson_metric(vectors[i], vectors[j])
            nb[i][j] = metrics.cosine_metric(vectors[i], vectors[j])
            nc[i][j] = metrics.jac_metric(vectors[i], vectors[j], trans)
    return na, nb, nc


def create_heatmaps(vec, n_vec, old_vec, old_n_vec, files):
    matrix_pears, matrix_cosine, matrix_jac = get_corr_matrix(vec, 1)
    matrix_pears_2, matrix_cosine_2, matrix_jac_2 = get_corr_matrix(old_vec)
    print("Степень различие результатов для обычных векторов, где 1 - идентичные, 0 - непохожие, -1 - противоположные")
    print("Косинусная мера близости, обычные вектора:",
          cosine_similarity([matrix_cosine.flatten()], [matrix_cosine_2.flatten()])[0][0])
    print("Корреляция Пирсона, обычные вектора:",
          cosine_similarity([matrix_pears.flatten()], [matrix_pears_2.flatten()])[0][0])
    print("Жаккард, обычные вектора:",
          cosine_similarity([matrix_jac.flatten()], [matrix_jac_2.flatten()])[0][0])

    # plt.figure(figsize=(19.2, 10.8))
    # sea.heatmap(matrix_cosine, xticklabels=files, yticklabels=files, square=True, linewidths=.3, cmap="YlGnBu",
    #             annot=True, annot_kws={"size": 4}, fmt=".2f")
    # plt.savefig(os.path.join(RES_DIR, "HeatmapCos.png"), dpi=100)
    # plt.clf()
    #
    # plt.figure(figsize=(19.2, 10.8))
    # sea.heatmap(matrix_pears, xticklabels=files, yticklabels=files, square=True, linewidths=.3, cmap="YlGnBu",
    #             annot=True, annot_kws={"size": 4}, fmt=".2f")
    # plt.savefig(os.path.join(RES_DIR, "HeatmapPearson.png"), dpi=100)
    # plt.clf()
    #
    # plt.figure(figsize=(19.2, 10.8))
    # sea.heatmap(matrix_jac, xticklabels=files, yticklabels=files, square=True, linewidths=.3, cmap="YlGnBu",
    #             annot=True, annot_kws={"size": 4}, fmt=".2f")
    # plt.savefig(os.path.join(RES_DIR, "HeatmapJac.png"), dpi=100)
    # plt.clf()

    matrix_pears, matrix_cosine, matrix_jac = get_corr_matrix(n_vec, 1)
    matrix_pears_2, matrix_cosine_2, matrix_jac_2 = get_corr_matrix(old_n_vec)
    print("Косинусная мера близости, нормализированные вектора:",
          cosine_similarity([matrix_cosine.flatten()], [matrix_cosine_2.flatten()])[0][0])
    print("Корреляция Пирсона, нормализированные вектора:",
          cosine_similarity([matrix_pears.flatten()], [matrix_pears_2.flatten()])[0][0])
    print("Жаккард, нормализированные вектора:",
          cosine_similarity([matrix_jac.flatten()], [matrix_jac_2.flatten()])[0][0])

    # plt.figure(figsize=(19.2, 10.8))
    # sea.heatmap(matrix_cosine, xticklabels=files, yticklabels=files, square=True, linewidths=.3, cmap="YlGnBu",
    #             annot=True, annot_kws={"size": 4}, fmt=".2f")
    # plt.savefig(os.path.join(RES_DIR, "HeatmapCosNorm.png"), dpi=100)
    # plt.clf()
    #
    # plt.figure(figsize=(19.2, 10.8))
    # sea.heatmap(matrix_pears, xticklabels=files, yticklabels=files, square=True, linewidths=.3, cmap="YlGnBu",
    #             annot=True, annot_kws={"size": 4}, fmt=".2f")
    # plt.savefig(os.path.join(RES_DIR, "HeatmapPearsonNorm.png"), dpi=100)
    # plt.clf()
    #
    # plt.figure(figsize=(19.2, 10.8))
    # sea.heatmap(matrix_jac, xticklabels=files, yticklabels=files, square=True, linewidths=.3, cmap="YlGnBu",
    #             annot=True, annot_kws={"size": 4}, fmt=".2f")
    # plt.savefig(os.path.join(RES_DIR, "HeatmapJacNorm.png"), dpi=100)
    # plt.clf()


if not os.path.exists(RES_DIR):
    os.mkdir(RES_DIR)

build_heatmaps_recursive(DIR)
