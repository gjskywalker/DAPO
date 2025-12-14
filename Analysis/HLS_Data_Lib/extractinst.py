import os
from openpyxl import Workbook

def get_directory_contents(folder_path):
    file_list = []
    for root, dirs, files in os.walk(folder_path):
        for file in files:
            file_list.append(file)
    return file_list

folder_path = "/home/x2/jinming-ge/HKUST_Intern/LLVM-16-Tutorials/HLS_Data_Lib"
file_paths = get_directory_contents(folder_path)

workbook = Workbook()

worksheet = workbook.active

for index, file_path in enumerate(file_paths, start=1):
    worksheet.cell(row=index, column=1, value=file_path)

output_file = 'output.xlsx'
workbook.save(output_file)