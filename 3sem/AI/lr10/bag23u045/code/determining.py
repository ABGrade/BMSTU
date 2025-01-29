import numpy as np
import pandas as pd


def to_del_by_dispersion(df: pd.DataFrame):
    threshold = 0.99
    df_var = df.var()
    print(f"Длина до: {df_var.shape[0]}")

    # Среднее отклонение от среднего
    mean_df_var = df_var.mean()

    lower_bound = mean_df_var * (1 - threshold)
    upper_bound = mean_df_var * (1 + threshold)

    list_to_remove = df_var[(df_var > lower_bound) & (df_var < upper_bound)].index.to_list()

    print(f"Длина после: {df_var.shape[0] - len(list_to_remove)}")
    print(f"На удаление: {list_to_remove}")
    return list_to_remove


def to_del_by_mean(df: pd.DataFrame):
    threshold = 0.04  # Признак встречается только в 2 документах (4% от общего числа)

    df_mean = df.mean()
    print(f"Длина до: {df_mean.shape[0]}")

    list_to_remove = df_mean[df_mean <= threshold].index.to_list()
    print(f"Длина после: {df_mean.shape[0] - len(list_to_remove)}")
    print(f"На удаление: {list_to_remove}")
    return list_to_remove


def to_del_by_covariance(df: pd.DataFrame):
    threshold = 0.6

    # Вычисляем матрицу корреляции Пирсона
    df_cov = df.cov().abs()
    print(f"Длина до: {df_cov.shape[0]}")

    # Устанавливаем значения ниже порога в NaN, чтобы отфильтровать только значимые корреляции
    df_cov = df_cov.where(np.tril(df_cov, k=-1) > threshold)

    # Находим признаки для удаления
    to_drop = set()
    for col in df_cov.columns:
        if col not in to_drop:
            # Находим все признаки, которые коррелируют с текущим
            correlated_features = df_cov[col].dropna().index
            # Добавляем их в список для удаления, кроме текущего признака
            to_drop.update(correlated_features.difference([col]))

    print(f"Длина после: {len(df_cov.columns) - len(to_drop)}")
    # print(f"На удаление: {sorted(to_drop)}")
    return to_drop


def to_del_by_pearson(df: pd.DataFrame):
    threshold = 0.99

    # Вычисляем матрицу корреляции Пирсона
    df_pearson = df.corr(method='pearson').abs()
    print(f"Длина до: {df_pearson.shape[0]}")

    # Устанавливаем значения выше порога в NaN
    df_pearson = df_pearson.where(np.tril(df_pearson, k=-1) > threshold)

    # Находим признаки для удаления
    to_drop = set()
    for col in df_pearson.columns:
        if col not in to_drop:
            # Находим все признаки, которые коррелируют с текущим
            correlated_features = df_pearson[col].dropna().index
            # Добавляем их в список для удаления, кроме текущего признака
            to_drop.update(correlated_features.difference([col]))

    print(f"Длина после: {len(df_pearson.columns) - len(to_drop)}")
    # print(f"На удаление: {to_drop}")
    return to_drop


def to_del_by_spearman(df: pd.DataFrame):
    threshold = 0.99
    # Вычисляем матрицу корреляции Спирмена
    df_spearman = df.corr(method='spearman').abs()
    print(f"Длина до: {df_spearman.shape[0]}")

    # Устанавливаем значения ниже порога в NaN, чтобы отфильтровать только значимые корреляции
    df_spearman = df_spearman.where(np.tril(df_spearman, k=-1) > threshold)

    # Находим признаки для удаления
    to_drop = set()
    for col in df_spearman.columns:
        if col not in to_drop:
            # Находим все признаки, которые коррелируют с текущим
            correlated_features = df_spearman[col].dropna().index
            # Добавляем их в список для удаления, кроме текущего признака
            to_drop.update(correlated_features.difference([col]))

    print(f"Длина после: {len(df_spearman.columns) - len(to_drop)}")
    # print(f"На удаление: {to_drop}")
    return to_drop
