import subprocess
import re
import csv

exe_path = "taskbag-uname -m.exe"

with open("output.csv", "w", newline='') as output_file:
    writer = csv.writer(output_file)
    for _ in range(2):
        result = subprocess.run([exe_path], capture_output=True, text=True)
        numbers = re.findall(r'\d+\.\d+|\d+', result.stdout)
        writer.writerow(numbers)