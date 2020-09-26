import os
from PIL import Image

directory_path = input("Resource directory: ")  # C:\Users\wojtek\Documents\chimarrao-platformer\resources\Tiles\1.png
left = int(input("Pixels to cut from left: "))  # 27
top = int(input("Pixels to cut from top: "))  # 18
right = int(input("Pixels to cut from right: "))  # 67
bottom = int(input("Pixels to cut from bottom: "))  # 81


def is_image(path):
    split_path = path.split(".")
    return split_path[-1] == "png" and split_path[-2] != "bak"


def adjust_image(path):
    im = Image.open(path)
    im.save(path[:-4] + ".bak.png", "PNG")
    return im.crop((left, top, right, bottom))


for dirpath, dirnames, filenames in os.walk(directory_path):
    for file_name in filenames:
        if os.name == "nt":
            file_path = dirpath + "\\" + file_name
        else:
            file_path = dirpath + "/" + file_name

        if is_image(file_path):
            image = adjust_image(file_path)
            image.save(file_path, "PNG")
