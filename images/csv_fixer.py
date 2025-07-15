import csv
import re

input_file = "prado.csv"
output_file = "clean_" + input_file

with open(input_file, mode="r", encoding="utf-8", newline='') as infile, \
     open(output_file, mode="w", encoding="utf-8", newline='') as outfile:

    raw_text = infile.read()
    writer = csv.writer(outfile)

    #final_result = []
    split_text = raw_text.split("@@@")
    for text in split_text:
        clean_text = text.replace("\n", "").replace("\r", "").replace("\t", "")

        writer.writerow(clean_text.split(","))

print("CSV cleaned and written to", output_file)