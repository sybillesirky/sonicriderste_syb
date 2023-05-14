import pathlib
import sys
from collections import Counter


def find_string(input_str, list_paths):
	accum_line = 0
	for path in list_paths:
		list_current_file = []
		with open(path, 'r') as file_content:
			for line in file_content:
				list_current_file.append(line)
		for line in list_current_file:
			accum_line += 1
			if input_str in line:
				print("Match in file:", path, "\nMatch in line:", accum_line)
				print(line, end="\n\n")


def create_filepaths(path_input):
    list_append = []
    list_return = []
    for item in path_input.iterdir():
        path_subdir = ""
        if item.is_file() == False:
            path_subdir = pathlib.Path(item)
            list_add = create_filepaths(path_subdir)
            if list_add != []:
                list_append += list_add
        else:
            list_append.append(item)
    for item in list_append:
        list_return.append(str(item))
    return list_return


def main():
    path_base = pathlib.Path(sys.argv[1])
    list_paths = create_filepaths(path_base)
    str_tomatch = input("String to match:")
    find_string(str_tomatch, list_paths)


main()