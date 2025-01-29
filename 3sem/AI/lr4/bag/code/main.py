"""
С использованием библиотеки opencv необходимо реализовать поиск на фотографии лошади и ромба на изображении.
"""
import cv2
import numpy as np

from pathlib import Path

WORK_DIR = (Path.cwd() / "..").resolve()

TEXT_DIR = WORK_DIR / "тексты"
RES_DIR = WORK_DIR / "report" / "images"
IMG_DIR = WORK_DIR / "images"

YOLO = WORK_DIR / "yolov3.weights"

names_path = WORK_DIR / "coco.names"

# Загрузка списка имен классов
with open(names_path, "r") as f:
    class_names = f.read().strip().split("\n")


def detect_diamonds(image): # Изменили название функции на множественное число
    # Преобразование изображения в оттенки серого
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    thresh = cv2.adaptiveThreshold(gray, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY_INV, 11, 2)
    contours, _ = cv2.findContours(thresh, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)

    diamonds_found = False # Флаг для отслеживания, были ли найдены ромбы

    for contour in contours:
        epsilon = 0.04 * cv2.arcLength(contour, True)
        approx = cv2.approxPolyDP(contour, epsilon, True)

        if len(approx) == 4 and cv2.isContourConvex(approx):
            sides = [np.linalg.norm(approx[i][0] - approx[(i+1) % 4][0]) for i in range(4)]
            side_ratios = [sides[i] / sides[(i+1) % 4] for i in range(4)]
            if all(0.6 <= ratio <= 1.4 for ratio in side_ratios):
                angles = []
                for i in range(4):
                    vec1 = approx[i][0] - approx[(i-1) % 4][0]
                    vec2 = approx[i][0] - approx[(i+1) % 4][0]
                    cos_angle = np.dot(vec1, vec2) / (np.linalg.norm(vec1) * np.linalg.norm(vec2))
                    angle = np.arccos(np.clip(cos_angle, -1.0, 1.0)) * (180.0 / np.pi)
                    angles.append(angle)

                if all(angle > 25 and angle < 165 for angle in angles):
                    acute_angles = sum(1 for angle in angles if 45 <= angle <= 85)
                    obtuse_angles = sum(1 for angle in angles if 95 <= angle <= 135)
                    if acute_angles == 2 and obtuse_angles == 2:
                        diag1_sq = np.sum((approx[0][0] - approx[2][0])**2)
                        diag2_sq = np.sum((approx[1][0] - approx[3][0])**2)
                        if not 0.9 <= diag1_sq / diag2_sq <= 1.2:

                            # Нарисовать найденный ромб (для каждого найденного ромба)
                            x, y, w, h = cv2.boundingRect(approx)
                            cv2.rectangle(image, (x, y), (x + w, y + h), (255, 0, 0), 2)
                            # cv2.putText(image, f'rhomb',
                            #             (x + 10, y - 10), cv2.FONT_HERSHEY_DUPLEX, 0.7, (0, 255, 0), 2)
                            diamonds_found = True # Устанавливаем флаг, что ромб найден

    if diamonds_found: # Возвращаем изображение только если были найдены ромбы
        return image
    else:
        return None # Возвращаем None, если ромбы не найдены


def detect_horse(image, model_path, horse_class_id):
    # Загрузка предварительно обученной модели YOLO
    net = cv2.dnn.readNet(model_path, model_path.replace(".weights", ".cfg"))

    # Подготовка входного изображения
    blob = cv2.dnn.blobFromImage(image, scalefactor=1/255.0, size=(416, 416), swapRB=True, crop=False)
    net.setInput(blob)

    # Получение выходных данных
    layer_names = net.getUnconnectedOutLayersNames()
    outputs = net.forward(layer_names)

    h, w = image.shape[:2]

    # Списки для хранения рамок и уверенности
    boxes = []
    confidences = []

    for output in outputs:
        for detection in output:
            scores = detection[5:]
            class_id = np.argmax(scores)
            confidence = scores[class_id]

            # Проверка, если это лошадь (horse_class_id) и уверенность больше порога
            if class_id == horse_class_id and confidence > 0.2:
                center_x, center_y, width, height = detection[:4] * np.array([w, h, w, h])
                x = int(center_x - width / 2)
                y = int(center_y - height / 2)

                # Добавляем координаты и уверенность для NMS
                boxes.append([x, y, int(width), int(height)])
                confidences.append(confidence)

    if len(boxes) == 0:
        return None

    # Применяем NMS для удаления дублирующихся рамок
    indices = cv2.dnn.NMSBoxes(boxes, confidences, score_threshold=0.3, nms_threshold=0.7)

    # Отображаем результаты на изображении
    if len(indices) > 0:
        for i in indices.flatten():
            box = boxes[i]
            x, y, w, h = box
            cv2.rectangle(image, (x, y), (x + w, y + h), (24, 143, 0), 2)  # Рисуем рамку
            cv2.putText(image, f'{class_names[horse_class_id]}: {confidences[i] * 100:.2f}%',
                        (x + 10, y - 10), cv2.FONT_HERSHEY_DUPLEX, 1, (0, 255, 0), 1)

    # Возвращаем изображение с аннотированными рамками
    return image


def get_images(image_dir):
    paths = []
    for filepath in image_dir.rglob("*.jpg"):
        paths.append(filepath)
    return paths


def proceed_image(image_path, model_path):
    horse_class_id = class_names.index("horse")

    image = cv2.imread(image_path)
    image_copy = image.copy()

    # Обнаружение ромба
    diamond = detect_diamonds(image)

    # Обнаружение лошади
    image_with_boxes = detect_horse(image_copy, model_path, horse_class_id)

    if image_with_boxes is not None:
        output = RES_DIR / "horses" / image_path.name
        output.parent.mkdir(parents=True, exist_ok=True)
        cv2.imwrite(str(output), image_with_boxes)

    if diamond is not None:
        output = RES_DIR / "diamonds" / image_path.name
        output.parent.mkdir(parents=True, exist_ok=True)
        cv2.imwrite(str(output), diamond)



def main():
    model_path = str(YOLO)
    images_path = get_images(IMG_DIR)

    for image_path in images_path:
        proceed_image(image_path, model_path)


if __name__ == "__main__":
    main()