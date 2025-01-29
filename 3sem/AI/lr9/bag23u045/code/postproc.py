import numpy as np

from sklearn.cluster import KMeans
from fcmeans import FCM

MIN_CLUSTERS = 2
MAX_CLUSTERS = 11


def kmeans_clustering_wcss_analyz(vectors):
    wcss = []
    for i in range(MIN_CLUSTERS, MAX_CLUSTERS):
        kmeans = KMeans(n_clusters=i, init="k-means++", random_state=1)
        kmeans.fit(vectors)
        wcss.append(kmeans.inertia_)
    return [wcss, MIN_CLUSTERS, MAX_CLUSTERS]


def kmeans_clustering_distance_analyze(vectors):
    inner_distances = []  # Внутрикластерные расстояния
    outer_distances = []  # Межкластерные расстояния
    
    for i in range(MIN_CLUSTERS, MAX_CLUSTERS):
        kmeans = KMeans(n_clusters=i, init="k-means++", random_state=1)
        kmeans.fit(vectors)
        
        # Внутрикластерное расстояние
        intra_distance_list = []
        for j in range(i):
            cluster_points = vectors[kmeans.labels_ == j]
            if len(cluster_points) > 0:  # Проверка на пустые кластеры
                intra_distance_list.append(
                    np.linalg.norm(cluster_points - kmeans.cluster_centers_[j], axis=1).mean()
                )
        if intra_distance_list:
            inner_distances.append(np.mean(intra_distance_list))
        else:
            inner_distances.append(0)
        
        # Межкластерное расстояние
        pairwise_distances = np.linalg.norm(
            kmeans.cluster_centers_[:, np.newaxis] - kmeans.cluster_centers_, axis=2
        )
        if np.any(pairwise_distances > 0):
            inter_distance = np.mean(pairwise_distances[pairwise_distances > 0])
        else:
            inter_distance = 0
        outer_distances.append(inter_distance)
    
    return [inner_distances, outer_distances, MIN_CLUSTERS, MAX_CLUSTERS]


def cmeans_clustering_wcss_analyz(vectors):
    wcss = []
    for i in range(MIN_CLUSTERS, MAX_CLUSTERS):
        fcm = FCM(n_clusters=i, m=2.0, max_iter=150, error=1e-6, random_state=1)
        fcm.fit(vectors)
        
        # Вычисление взвешенной суммы расстояний
        centers = fcm.centers
        membership = fcm.u  # Матрица принадлежности
        distance = np.linalg.norm(vectors[:, np.newaxis] - centers, axis=2)  # Евклидово расстояние
        weighted_distance = np.sum(membership ** 2 * distance ** 2)
        wcss.append(weighted_distance)
    
    return [wcss, MIN_CLUSTERS, MAX_CLUSTERS]



def cmeans_clustering_distance_analyze(vectors, min_clusters=MIN_CLUSTERS, max_clusters=MAX_CLUSTERS):
    inner_distances = []  # Внутрикластерные расстояния
    outer_distances = []  # Межкластерные расстояния

    for i in range(min_clusters, max_clusters):
        fcm = FCM(n_clusters=i, m=2.0, max_iter=150, error=1e-6, random_state=1)
        fcm.fit(vectors)

        centers = fcm.centers
        membership = fcm.u  # Матрица принадлежности

        # Внутрикластерное расстояние
        intra_distance_list = []
        for j in range(i):
            cluster_weights = membership[:, j]
            cluster_points = vectors[cluster_weights > 0.1]  # Фильтрация точек, сильно принадлежащих кластеру
            if len(cluster_points) > 0:
                distances = np.linalg.norm(cluster_points - centers[j], axis=1)
                intra_distance_list.append(np.average(distances, weights=cluster_weights[cluster_weights > 0.1]))
        if intra_distance_list:
            inner_distances.append(np.mean(intra_distance_list))
        else:
            inner_distances.append(0)

        # Межкластерное расстояние
        pairwise_distances = np.linalg.norm(
            centers[:, np.newaxis] - centers, axis=2
        )
        if np.any(pairwise_distances > 0):
            inter_distance = np.mean(pairwise_distances[np.triu_indices(i, k=1)])
        else:
            inter_distance = 0
        outer_distances.append(inter_distance)

    return [inner_distances, outer_distances, min_clusters, max_clusters]



