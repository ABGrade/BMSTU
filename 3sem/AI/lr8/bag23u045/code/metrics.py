from scipy.stats import pearsonr
from sklearn.decomposition import PCA
from sklearn.metrics.pairwise import cosine_similarity
from sklearn.metrics import jaccard_score


def cosine_metric(v1, v2):
    return cosine_similarity([v1], [v2])[0][0]


def pearson_metric(v1, v2):
    correlation, _ = pearsonr(v1, v2)
    return correlation


def jac_metric(v1, v2, to_binary=None):
    if to_binary:
        # Бинаризация по знаку: положительные значения -> 1, отрицательные -> 0
        v1_binary = [int(value >= 0) for value in v1]
        v2_binary = [int(value >= 0) for value in v2]
        return jaccard_score(v1_binary, v2_binary)
    else:
        return jaccard_score(v1, v2, average='macro')



def call_pca(v1s, v2s):
    sk_pca = PCA()

    all_keys_v1 = set().union(*[vec.keys() for vec in v1s])
    all_keys_v2 = set().union(*[vec.keys() for vec in v2s])

    # Преобразуем списки словарей в списки значений, добавляя 0 для отсутствующих ключей
    v1s_values = [[v1.get(key, 0) for key in all_keys_v1] for v1 in v1s]
    v2s_values = [[v2.get(key, 0) for key in all_keys_v2] for v2 in v2s]

    v1_transformed = sk_pca.fit_transform(v1s_values)
    v2_transformed = sk_pca.fit_transform(v2s_values)

    return v1_transformed, v2_transformed, v1s_values, v2s_values
