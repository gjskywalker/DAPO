def read_numbers_from_file(filename):
    numbers = []
    try:
        with open(filename, 'r') as file:
            for line in file:
                parts = line.split(':')
                if len(parts) > 1:
                    number_str = parts[1].strip()
                    try:
                        number = float(number_str)
                        numbers.append(number)
                    except ValueError:
                        print(f"Error parsing number in line: {line}")
    except FileNotFoundError:
        print(f"File '{filename}' not found.")
    return numbers

# 使用你的txt文件名替换下面的'your_file.txt'
filename = 'GED_Result/GED_Result.txt'
result = read_numbers_from_file(filename)

import torch
import numpy as np
import pickle

result = torch.tensor(result) / 100
print(torch.sum(result) / 4872)
# with open("GED_Result.pkl", "wb") as fp:
#     pickle.dump(result, fp)

# a = np.ones((1,4))
# b = np.zeros((1,4))
# a = torch.tensor(a)
# b = torch.tensor(b)
# result = (a - b).pow(2).sum(1).pow(1/2)
# print(result)