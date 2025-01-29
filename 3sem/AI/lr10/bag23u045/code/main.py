# Необходимо построить вектора документов для черепашьего набора данных.
# Для параметров, определяющих вектор, необходимо определить:
#   - дисперсию (по признаку. Если имеет низкую дисперсию, то это значит признак встречается одинаково, а значит не несет полезной информации)
#   - математическое ожидание (по признаку. Если среднее значение близко к нулю, то оно не несет полезной  информации и его можно удалить)
#   - ковариацию (по всем признакам. Если ковариация между признаками большая, то они могут нести одну и ту же информацию, следовательно один из них можно удалить)
#   - корреляции (по всем признакам. Если признаки близки к предельным (1 или -1), то это может свидетельствоать о высокой взаимосвязи между ними, следовательно один из них лишний)
#       + Пирсона
#       + Спирмена
# для:
#   - всего набора документов
#   - каждой выделенной тематической группы.

# Вариант: Какие параметры пространства можно удалить? Почему?

"""
Низкая диспресия считатается та, что близка к среднему. Близкое к среднему будем считать то, которое находится в пределах 1 стандартного отклонения (возможно бредик). Те признаки, что находятся в рамках 1o будем обнулять.
Низким мат ожиданием будем считать то, которое меньше 1о (стандартного отклонения).
Ковариацию необходимо привести к корреляции пирсона, для объективного оценивания ковариации двух признаков,
т.к. ковариация зависит от масштаба данных (возмодно бредик).
"""

from pathlib import Path

import matplotlib.pyplot as plt
import seaborn as sea
import numpy as np
import pandas as pd

import determining
import metrics

from sklearn.cluster import KMeans
from sklearn.preprocessing import StandardScaler

TEXT_DIR = Path.cwd() / ".." / "тексты"
RES_DIR = Path.cwd() / ".." / "report" / "results"

ANAMNEZ = TEXT_DIR / "Анамнез"
BIOCHEMYSTRI = TEXT_DIR / "Биохимия крови"
COMMENTATORS = TEXT_DIR / "Комментаторы"
NONORGANIC = TEXT_DIR / "Неорганическая химия"
ORGANIC = TEXT_DIR / "Органическая химия"
SPORT = TEXT_DIR / "Спортивная медицина"
XBET = TEXT_DIR / "Ставки на спорт"

DIRS = [ANAMNEZ, BIOCHEMYSTRI, COMMENTATORS, NONORGANIC, ORGANIC, SPORT, XBET]

def load_vectors_from_directory(base_dir):
    vectors = []
    files = []

    for file_path in base_dir.rglob('vectors/*.txt'):
        files.append(file_path.name)
        vector = np.loadtxt(file_path)
        vectors.append(vector)

    return np.vstack(vectors), files


def get_corr_matrix(vectors):
    vecs_len = len(vectors)
    na = np.zeros((vecs_len, vecs_len))
    nb = np.zeros((vecs_len, vecs_len))
    nc = np.zeros((vecs_len, vecs_len))
    for i in range(vecs_len):
        for j in range(vecs_len):
            na[i][j] = metrics.pearson_metric(vectors[i], vectors[j])
            nb[i][j] = metrics.cosine_metric(vectors[i], vectors[j])
            nc[i][j] = metrics.jac_metric(vectors[i], vectors[j])
    return na, nb, nc


def build_heatmap(data, savepath, xlables, ylabels):
    plt.figure(figsize=(19.2, 10.8))
    sea.heatmap(data, xticklabels=xlables, yticklabels=ylabels, square=True, linewidths=.3, cmap="YlGnBu")
    plt.savefig(savepath, dpi=100)
    plt.clf()


if not Path.is_dir(TEXT_DIR):
    exit(1)

if not Path.is_dir(RES_DIR):
    Path.mkdir(RES_DIR)

# "Для каждой тематической группы"
# for i in range(len(DIRS)):
#     print(f"Просмотр {DIRS[i]}")
#     vectors, _ = load_vectors_from_directory(DIRS[i])
#     df = pd.DataFrame(vectors)
#     print("По дисперсии: ")
#     to_drop_by_dispersion = determining.to_del_by_dispersion(df)
#     print("По среднему: ")
#     to_drop_by_mean = determining.to_del_by_mean(df)
#     print("По ковариации: ")
#     to_drop_by_covariance = determining.to_del_by_covariance(df)
#     print("По Пирсону: ")
#     to_drop_by_pearson = determining.to_del_by_pearson(df)
#     print("По Спирмену: ")
#     to_drop_by_spearman = determining.to_del_by_spearman(df)


vectors, files = load_vectors_from_directory(TEXT_DIR)

df = pd.DataFrame(vectors)

to_drop_by_dispersion = determining.to_del_by_dispersion(df)
# vectors_updated_dispersion = np.delete(vectors, list(to_drop_by_dispersion), axis=1)
# pearson_matrix, cosine_matrix, jac_matrix = get_corr_matrix(vectors_updated_dispersion)
# build_heatmap(pearson_matrix, RES_DIR / "Heatmap_dispersion_pearson.png", files, files)
# build_heatmap(cosine_matrix, RES_DIR / "Heatmap_dispersion_cosine.png", files, files)
# build_heatmap(jac_matrix, RES_DIR / "Heatmap_dispersion_jac.png", files, files)
#
to_drop_by_mean = determining.to_del_by_mean(df)
# vectors_updated_mean = np.delete(vectors, list(to_drop_by_mean), axis=1)
# pearson_matrix, cosine_matrix, jac_matrix = get_corr_matrix(vectors_updated_mean)
# build_heatmap(pearson_matrix, RES_DIR / "Heatmap_mean_pearson.png", files, files)
# build_heatmap(cosine_matrix, RES_DIR / "Heatmap_mean_cosine.png", files, files)
# build_heatmap(jac_matrix, RES_DIR / "Heatmap_mean_jac.png", files, files)
#
to_drop_by_covariance = determining.to_del_by_covariance(df)
# vectors_updated_covariance = np.delete(vectors, list(to_drop_by_covariance), axis=1)
# pearson_matrix, cosine_matrix, jac_matrix = get_corr_matrix(vectors_updated_covariance)
# build_heatmap(pearson_matrix, RES_DIR / "Heatmap_covariance_pearson.png", files, files)
# build_heatmap(cosine_matrix, RES_DIR / "Heatmap_covariance_cosine.png", files, files)
# build_heatmap(jac_matrix, RES_DIR / "Heatmap_covariance_jac.png", files, files)
#
to_drop_by_pearson = determining.to_del_by_pearson(df)
# vectors_updated_pearson = np.delete(vectors, list(to_drop_by_pearson), axis=1)
# pearson_matrix, cosine_matrix, jac_matrix = get_corr_matrix(vectors_updated_pearson)
# build_heatmap(pearson_matrix, RES_DIR / "Heatmap_pearson_pearson.png", files, files)
# build_heatmap(cosine_matrix, RES_DIR / "Heatmap_pearson_cosine.png", files, files)
# build_heatmap(jac_matrix, RES_DIR / "Heatmap_pearson_jac.png", files, files)
#
to_drop_by_spearman = determining.to_del_by_spearman(df)
# vectors_updated_spearman = np.delete(vectors, list(to_drop_by_spearman), axis=1)
# pearson_matrix, cosine_matrix, jac_matrix = get_corr_matrix(vectors_updated_spearman)
# build_heatmap(pearson_matrix, RES_DIR / "Heatmap_spearman_pearson.png", files, files)
# build_heatmap(cosine_matrix, RES_DIR / "Heatmap_spearman_cosine.png", files, files)
# build_heatmap(jac_matrix, RES_DIR / "Heatmap_spearman_jac.png", files, files)