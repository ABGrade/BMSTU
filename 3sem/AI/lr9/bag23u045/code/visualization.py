import matplotlib.pyplot as plt
import seaborn as sea

from pathlib import Path
from scipy.cluster.hierarchy import dendrogram, linkage, fcluster

import numpy as np
from scipy.spatial.distance import pdist, cdist

from sklearn.decomposition import PCA
from sklearn.cluster import KMeans, AgglomerativeClustering
from fcmeans import FCM



REP_DIR = (Path.cwd() / "report").resolve()

IMG_DIR = REP_DIR / "images"

WCSS_DIR = IMG_DIR / "wcss"
DISTANCE = IMG_DIR / "distance"
CLUSTERS = IMG_DIR / "cluster"


def wcss_visualization(wcss_object, method):
    plt.plot(range(wcss_object[1], wcss_object[2]), wcss_object[0])
    plt.title("Оценка количества кластеров методом локтя")
    plt.xlabel("Количество кластеров")
    plt.ylabel("Квадрат расстояния от объектов до центра кластера")

    output_path = WCSS_DIR / f"wcss_{method}.png"
    output_path.parent.mkdir(exist_ok=True, parents=True)

    plt.savefig(output_path)
    plt.clf()


def distance_visualization(distance_object, method):
    plt.plot(range(distance_object[2], distance_object[3]), distance_object[0], label="Внутрикластерное расстояние")
    plt.plot(range(distance_object[2], distance_object[3]), distance_object[1], label="Межкластерное расстояние")
    plt.title("Среднее кластерное рассотояние")
    plt.xlabel("Количество кластеров")
    plt.ylabel("Среднее расстояние до центров")
    plt.legend()

    output_path = DISTANCE / f"distance_{method}.png"
    output_path.parent.mkdir(exist_ok=True, parents=True)

    plt.savefig(output_path)
    plt.clf()


def hierarchical_clustering_analyze(vectors, method='ward'):
    # Вычисляем матрицу связей
    linkage_matrix = linkage(vectors, method=method)
    
    # Строим дендрограмму
    plt.figure(figsize=(10, 7))
    dendrogram(linkage_matrix)
    plt.title("Дендрограмма иерархической класеризации")
    plt.xlabel("Векторы")
    plt.ylabel("Расстояние")

    output_path = WCSS_DIR / f"dendrogramm_{method}.png"
    output_path.parent.mkdir(exist_ok=True, parents=True)

    plt.savefig(output_path)
    plt.clf()
    return linkage_matrix



def analyze_cluster_distances(vectors, method='ward', metric='euclidean', max_clusters=10):
    # Вычисляем матрицу связей
    linkage_matrix = linkage(vectors, method=method, metric=metric)

    # Списки для хранения средних расстояний
    intra_cluster_distances = []
    inter_cluster_distances = []

    # Цикл по количеству кластеров
    for num_clusters in range(2, max_clusters + 1):
        # Разбиение на кластеры
        cluster_labels = fcluster(linkage_matrix, num_clusters, criterion='maxclust')

        # Внутрикластерное расстояние
        intra_dist = []
        for cluster_id in np.unique(cluster_labels):
            cluster_points = vectors[cluster_labels == cluster_id]
            if len(cluster_points) > 1:  # Если в кластере больше одной точки
                intra_dist.append(pdist(cluster_points, metric=metric).mean())
        intra_cluster_distances.append(np.mean(intra_dist) if intra_dist else 0)

        # Межкластерное расстояние
        inter_dist = []
        unique_clusters = np.unique(cluster_labels)
        for i, cluster_i in enumerate(unique_clusters):
            for j, cluster_j in enumerate(unique_clusters):
                if i < j:  # Рассматриваем только пары разных кластеров
                    cluster_i_points = vectors[cluster_labels == cluster_i]
                    cluster_j_points = vectors[cluster_labels == cluster_j]
                    # Среднее расстояние между точками двух кластеров
                    inter_dist.append(cdist(cluster_i_points, cluster_j_points, metric=metric).mean())
        inter_cluster_distances.append(np.mean(inter_dist) if inter_dist else 0)

    # Визуализация
    plt.figure(figsize=(10, 6))
    plt.plot(range(2, max_clusters + 1), intra_cluster_distances, marker='o', label='Внутрикластерное расстояние')
    plt.plot(range(2, max_clusters + 1), inter_cluster_distances, marker='o', label='Межкластерное расстояние')
    plt.title("Кластерное расстояние")
    plt.xlabel("Кол-во кластеров")
    plt.ylabel("Среднее расстояние")
    plt.legend()
    plt.grid(True)

    # Сохраняем график
    output_path = DISTANCE / f"cluster_distance_analysis_{method}.png"
    output_path.parent.mkdir(exist_ok=True, parents=True)
    plt.savefig(output_path)


def visualize_clusters(data, labels, title, name):
    pca = PCA(n_components=2)
    reduced_data = pca.fit_transform(data)
    
    plt.figure(figsize=(10, 8))
    sea.scatterplot(x=reduced_data[:, 0], y=reduced_data[:, 1], hue=labels, palette="viridis", s=50)
    plt.title(title)
    plt.xlabel("PCA Component 1")
    plt.ylabel("PCA Component 2")
    plt.legend()
    plt.grid(True)
    
    output_path = CLUSTERS / f"{title} {name}.png"
    output_path.parent.mkdir(exist_ok=True, parents=True)
    plt.savefig(output_path)


def kmeans_clustering(data, n_clusters, text):
    kmeans = KMeans(n_clusters=n_clusters, random_state=1)
    labels = kmeans.fit_predict(data)
    visualize_clusters(data, labels, title="KMeans Clustering", name=text)

def cmeans_clustering(data, n_clusters, text):
    fcm = FCM(n_clusters=n_clusters)
    fcm.fit(data)
    labels = fcm.predict(data)
    visualize_clusters(data, labels, title="CMeans Clustering", name=text)

def hierarchical_clustering(data, n_clusters, text):
    hierarchical = AgglomerativeClustering(n_clusters=n_clusters)
    labels = hierarchical.fit_predict(data)
    visualize_clusters(data, labels, title="Hierarchical Clustering", name=text)