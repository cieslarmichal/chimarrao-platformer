import os
from PIL import Image
directory_path = input("Resource directory (to undo): ")


def to_undo(path):
    return path.split(".")[-2] == "bak"


def undo_image(path):
    return Image.open(path)


for dirpath, dirnames, filenames in os.walk(directory_path):
    for file_name in filenames:
        if os.name == "nt":
            file_path = dirpath + "\\" + file_name
        else:
            file_path = dirpath + "/" + file_name

        if to_undo(file_path):
            image = undo_image(file_path)
            image.save(file_path[:-8], "PNG")
