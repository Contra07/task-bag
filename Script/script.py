import subprocess
import re
import csv


p = 12
n = 8
m = 10000

i = 3
while pow(10, i) <= pow(10, n):
    exe_path1 = "taskbag.exe"
    exe_path2 = "QuickSort.exe"
    with open("out/output_task_" + str(i) + ".csv", "w", newline='') as output_file:
        writer = csv.writer(output_file)
        if(pow(10, i) < pow(10, 8)):
            m = pow(10, 2)
        else:
            m = abs(pow(10, (n-i+1)))
        print([m, exe_path1, pow(10, i), str(p)])
        for _ in range(m):
            result = subprocess.run([exe_path1, str(pow(10, i)), str(p)], capture_output=True, text=True)
            numbers = re.findall(r'\d+\.\d+|\d+', result.stdout)
            writer.writerow(numbers)
    with open("out/output_openmp_" + str(i) + ".csv", "w", newline='') as output_file:
        writer = csv.writer(output_file)
        if(pow(10, i) < pow(10, 8)):
            m = pow(10, 2)
        else:
            m = abs(pow(10, (n-i+1)))
        print([m, exe_path2, pow(10, i), str(p)])
        for _ in range(m):
            result = subprocess.run([exe_path2, str(pow(10, i)), str(p)], capture_output=True, text=True)
            numbers = re.findall(r'\d+\.\d+|\d+', result.stdout)
            writer.writerow(numbers)
    i += 1
