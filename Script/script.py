import subprocess
import re
import csv


p = 12
s = 5
n = 10
m = 10000

i = 100000
while i <= pow(10, n):
    exe_path = "taskbag.exe"
    with open("out/output_" + str(s) + ".csv", "w", newline='') as output_file:
        writer = csv.writer(output_file)
        print(i)
        for _ in range(m):
            result = subprocess.run([exe_path, str(i), str(p)], capture_output=True, text=True)
            numbers = re.findall(r'\d+\.\d+|\d+', result.stdout)
            writer.writerow(numbers)
        s += 1
        i = pow(10, s)