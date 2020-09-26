import os
directory_path = input("Resource directory (to undo): ")


def to_remove(path):
    return path.split(".")[-2] == "bak"





for dirpath, dirnames, filenames in os.walk(directory_path):
    for file_name in filenames:
        if os.name == "nt":
            file_path = dirpath + "\\" + file_name
        else:
            file_path = dirpath + "/" + file_name

        if to_remove(file_path):
            os.remove(file_path)
