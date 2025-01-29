"""
Классифицировать данные из mnist dataset, использую нейросетевой подход с ReLU функцией активации и KL Divergence функцией потерь.
1) Определить состояние переобучения и недообучения для различного соотношения обучвающей и тестовой выборок: [[10, 90], [20, 80], ... [90, 10]].
2) при различном количество скрытых слоёв: 0,1,5
3) По неравенству Чебышёва рассчитать аналитически необходимый размер обучающей выборки
"""

import tensorflow as tf
from tensorflow.keras import layers, models
from tensorflow.keras.losses import KLDivergence
from tensorflow.keras.utils import image_dataset_from_directory, to_categorical
from sklearn.model_selection import train_test_split

import numpy as np
import matplotlib.pyplot as plt
from pathlib import Path
import math

# Параметры модели
EPOCHS = 10
BATCH_SIZE = 32
HIDDEN_LAYER_CONFIGS = [0, 1, 5]
DATA_SPLITS = [(10, 90), (20, 80), (30, 70), (40, 60), (50, 50), (60, 40), (70, 30), (80, 20), (90, 10)]

# Загрузка данных
WORKDIR = (Path.cwd() / "..").resolve()
MNIST_DIR = WORKDIR / "mnist"
TRAIN_DIR = MNIST_DIR / "train"
TEST_DIR = MNIST_DIR / "test"
IMG_DIR = WORKDIR / "report" / "images"

# Функция построения модели
def build_model(hidden_layers):
    model = models.Sequential()
    model.add(layers.Input(shape=(28, 28, 1)))  # Входные данные как изображения (28x28x1)
    model.add(layers.Flatten())  # Преобразование в плоский вектор

    for _ in range(hidden_layers):
        model.add(layers.Dense(128, activation='relu'))

    model.add(layers.Dense(10, activation='softmax'))  # 10 классов
    return model


# Функция нормализации данных
def normalize_data(image, label):
    image = tf.cast(image, tf.float32) / 255.0
    label = to_categorical(label, num_classes=10)  # One-hot encoding
    return image, label

# Функция разделения данных на обучение и валидацию
def split_dataset(dataset, train_size):
    dataset_list = list(dataset.unbatch().as_numpy_iterator())  # Убираем лишнее измерение
    images, labels = zip(*dataset_list)
    images = np.array(images)
    labels = np.array(labels)

    x_train, x_val, y_train, y_val = train_test_split(images, labels, train_size=train_size)
    train_ds = tf.data.Dataset.from_tensor_slices((x_train, y_train)).batch(BATCH_SIZE)
    val_ds = tf.data.Dataset.from_tensor_slices((x_val, y_val)).batch(BATCH_SIZE)
    return train_ds, val_ds


# Функция обучения и оценки модели
def train_and_evaluate(train_ds, val_ds, test_ds, hidden_layers):
    model = build_model(hidden_layers)
    model.compile(optimizer='adam', loss=KLDivergence(), metrics=['accuracy'])
    history = model.fit(train_ds, validation_data=val_ds, epochs=EPOCHS, verbose=0)
    test_loss, test_acc = model.evaluate(test_ds, verbose=0)
    return history, test_loss, test_acc

# Анализ результатов
def plot_results(results):
    for hidden_layers in HIDDEN_LAYER_CONFIGS:
        plt.figure(figsize=(10, 6))
        for split in DATA_SPLITS:
            res = [r for r in results if r['hidden_layers'] == hidden_layers and r['split'] == split]
            if res:
                acc = res[0]['history'].history['accuracy']
                val_acc = res[0]['history'].history['val_accuracy']
                plt.plot(acc, label=f'Train Acc (split {split})')
                plt.plot(val_acc, linestyle='--', label=f'Val Acc (split {split})')

        title = f'Results for {hidden_layers} hidden layers'
        output_file = IMG_DIR / f'{title}.png'

        plt.title(title)
        plt.xlabel('Epoch')
        plt.ylabel('Accuracy')
        plt.legend()
        plt.savefig(output_file)

# Функция расчета размера выборки по неравенству Чебышева
def chebyshev_sample_size(epsilon, delta, sample_variance):
    if epsilon <= 0 or delta <= 0 or delta >= 1:
        raise ValueError("Epsilon and delta must be greater than zero and delta must be less than one.")

    n = math.ceil(sample_variance / (epsilon ** 2 * delta))
    return n

if __name__ == "__main__":
    print(tf.config.list_physical_devices('GPU'), tf.config.list_physical_devices('CPU'))
    # Загрузка данных
    train_ds = image_dataset_from_directory(TRAIN_DIR, label_mode="int", image_size=(28, 28),
                                            batch_size=BATCH_SIZE, color_mode="grayscale")
    test_ds = image_dataset_from_directory(TEST_DIR, label_mode="int", image_size=(28, 28),
                                           batch_size=BATCH_SIZE, color_mode="grayscale")

    train_ds = train_ds.map(normalize_data)
    test_ds = test_ds.map(normalize_data)

    # Подготовка данных для обучения
    results = []
    for split in DATA_SPLITS:
        train_size = split[0] / 100

        # Разделение train_ds
        train_ds_split, val_ds_split = split_dataset(train_ds, train_size)

        for hidden_layers in HIDDEN_LAYER_CONFIGS:
            history, test_loss, test_acc = train_and_evaluate(train_ds_split, val_ds_split, test_ds, hidden_layers)
            results.append({
                'split': split,
                'hidden_layers': hidden_layers,
                'test_loss': test_loss,
                'test_acc': test_acc,
                'history': history
            })

        # Рассчет размера обучающей выборки по неравенству Чебышева
        train_acc = [res['history'].history['accuracy'][-1] for res in results if res['split'] == split]
        sample_variance = np.var(train_acc)
        epsilon = 0.02
        delta = 0.05
        n = chebyshev_sample_size(epsilon, delta, sample_variance)
        print(f"For split {split}, Chebyshev sample size: {n} \n")

    # Построение графиков
    plot_results(results)

    # Вывод результатов
    for result in results:
        print(f"Split: {result['split']}, Hidden Layers: {result['hidden_layers']}, "
              f"Test Loss: {result['test_loss']:.4f}, Test Accuracy: {result['test_acc']:.4f}")