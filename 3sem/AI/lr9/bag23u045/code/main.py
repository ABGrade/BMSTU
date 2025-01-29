"""
Разбиваем векторы документов на кластеры методами иерархической кластеризации, k-means, c-means.
Оценку кол-ва кластеров провести с помощью wscc (обязательно указать максимальное кол-во больше, чем разбито экспертом).
Определяем среднее внутрикластерное и межкластероное расстояние для каждого случая (возможно wscc поможет).
"""

import numpy as np

from pathlib import Path

from postproc import *
from visualization import *

TEXT_DIR = Path.cwd() / "тексты"
RES_DIR = Path.cwd() / "report" / "results"

ANAMNEZ = TEXT_DIR / "Анамнез"
BIOCHEMISTRY = TEXT_DIR / "Биохимия крови"
COMMENTATORS = TEXT_DIR / "Комментаторы"
NONORGANIC = TEXT_DIR / "Неорганическая химия"
ORGANIC = TEXT_DIR / "Органическая химия"
SPORT = TEXT_DIR / "Спортивная медицина"
XBET = TEXT_DIR / "Ставки на спорт"

DIRS = [ANAMNEZ, BIOCHEMISTRY, COMMENTATORS, NONORGANIC, ORGANIC, SPORT, XBET]


def load_vectors_from_directory(base_dir):
    vectors = []
    files = []
    for file_path in base_dir.rglob('vectors/*.txt'):
        files.append(file_path.name)
        vector = np.loadtxt(file_path)
        vectors.append(vector)
    return np.vstack(vectors), files


def do_analyze(vectors):
    wcss_object = kmeans_clustering_wcss_analyz(vectors)
    wcss_visualization(wcss_object, "kmeans")
    distance_object = kmeans_clustering_distance_analyze(vectors)
    distance_visualization(distance_object, "kmeans")
    wcss_object = cmeans_clustering_wcss_analyz(vectors)
    wcss_visualization(wcss_object, "cmeans")
    distance_object = cmeans_clustering_distance_analyze(vectors)
    distance_visualization(distance_object, "cmeans")
    hierarchical_clustering_analyze(vectors)
    analyze_cluster_distances(vectors)


def do_visualization_clusters(vectors):
    n_clusters = 7 # Определено экспертом
    kmeans_clustering(vectors, n_clusters, "expert")
    cmeans_clustering(vectors, n_clusters, "expert")
    hierarchical_clustering(vectors, n_clusters, "expert")
    n_clusters = 10 # Определено в ходе исследования
    kmeans_clustering(vectors, n_clusters, "student")
    n_clusters = 5 # Определено в ходе исследования
    cmeans_clustering(vectors, n_clusters, "student")
    n_clusters = 5 # Определено в ходе исследования
    hierarchical_clustering(vectors, n_clusters, "student")



if __name__ == "__main__":
    vectors, _ = load_vectors_from_directory(TEXT_DIR)
    do_analyze(vectors)
    do_visualization_clusters(vectors)

