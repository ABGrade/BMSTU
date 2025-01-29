from scipy.stats import pearsonr
from sklearn.metrics.pairwise import cosine_similarity
from sklearn.metrics import jaccard_score


def cosine_metric(v1, v2):
    return cosine_similarity([v1], [v2])[0][0]


def pearson_metric(v1, v2):
    correlation, _ = pearsonr(v1, v2)
    return abs(correlation)


def jac_metric(v1, v2, to_binary=None):
    if to_binary:
        # Бинаризация по знаку: положительные значения -> 1, отрицательные -> 0
        v1_binary = [int(value >= 0) for value in v1]
        v2_binary = [int(value >= 0) for value in v2]
        return jaccard_score(v1_binary, v2_binary)
    else:
        return jaccard_score(v1, v2, average='macro')