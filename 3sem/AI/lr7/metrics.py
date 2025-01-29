import numpy as np

from sklearn.metrics.pairwise import cosine_similarity


def jacquard_similarity(v1: dict, v2: dict):
    a = set(v1.keys())
    b = set(v2.keys())
    shared = a.intersection(b)
    total = a.union(b)
    return len(shared) / len(total)


def cosine_metric(v1: dict, v2: dict):
    keys = sorted(set(v1.keys()).union(v2.keys()))  # Объединяем все ключи
    vector1 = np.array([v1.get(key, 0) for key in keys])
    vector2 = np.array([v2.get(key, 0) for key in keys])
    return cosine_similarity([vector1], [vector2])[0][0]
