from PIL import Image, ImageDraw

# Определяем размеры изображения
width = 400
height = 400

# Создаем новое изображение с белым фоном
image = Image.new('RGB', (width, height), '#FFFFFF')
draw = ImageDraw.Draw(image)

# Определяем основные цвета
face_color = '#ADD8E6'
outline_color = '#333399'
mouth_color = '#000000'
teeth_color = '#FFFFFF'
hair_color = '#556B2F'

# Рисуем круг лица
x0 = 50
y0 = 50
x1 = width - 50
y1 = height - 150
draw.ellipse((x0, y0, x1, y1), fill=face_color, outline=outline_color, width=5)

# Рисуем очки
eye_offset_x = 50
eye_size = 30
eye_y = 100
draw.ellipse((width // 2 - eye_offset_x - eye_size, eye_y - eye_size // 2, width // 2 - eye_offset_x + eye_size, eye_y + eye_size // 2), fill=outline_color)
draw.ellipse((width // 2 + eye_offset_x - eye_size, eye_y - eye_size // 2, width // 2 + eye_offset_x + eye_size, eye_y + eye_size // 2), fill=outline_color)
draw.rectangle((width // 2 - eye_offset_x + eye_size, eye_y - 5, width // 2 + eye_offset_x - eye_size, eye_y + 5), fill=outline_color)

# Рисуем рот (упрощенно)
mouth_y = 200
draw.rectangle((width // 2 - 70, mouth_y - 30, width // 2 + 70, mouth_y), fill=mouth_color)

# Рисуем зубы (упрощенно)
teeth = [(width // 2 - 50, mouth_y - 20, width // 2 - 30, mouth_y),
         (width // 2 - 10, mouth_y - 20, width // 2 + 10, mouth_y),
         (width // 2 + 30, mouth_y - 20, width // 2 + 50, mouth_y)]

for tooth in teeth:
    draw.rectangle(tooth, fill=teeth_color)

# Рисуем "волосы" и тени (упрощенно)
draw.ellipse((50,50,80,80),fill=hair_color) # просто кружок, чтобы передать общий смысл

image.show()