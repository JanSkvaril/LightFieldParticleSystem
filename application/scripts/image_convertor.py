from PIL import Image
import os
images = []

path = "./dice/"
for img in os.listdir(path):
    images.append(Image.open(path + img))
size = 7
w = images[0].width
h = images[0].height
print(w, h, len(images))

result = Image.new("RGB", (w*size, h*size))
x = 0
y = 0
for img in images:
    result.paste(img, (x*w, y*h))
    x += 1
    if x == size:
        x = 0
        y += 1
result.save("result.jpg")
