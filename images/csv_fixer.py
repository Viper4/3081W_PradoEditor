'''import csv
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

print("CSV cleaned and written to", output_file)'''

import csv
import re
from io import StringIO

input_file = "smallerpaintings.csv"
output_file = "clean_" + input_file

with open(input_file, mode="r", encoding="utf-8", newline='') as infile, \
        open(output_file, mode="w", encoding="utf-8", newline='') as outfile:
    raw_text = infile.read()
    writer = csv.writer(outfile)

    # Split on @@@ (each block is assumed to be one column item)
    raw_rows = re.split(r'\n(?=https://)', raw_text)
    for row in raw_rows:
        row = row.replace("\n", "").replace("\r", "").replace("\t", "")
        outfile.write(row + "\n")

    '''
    row = []
    for text in split_text:
        # Remove newlines and tabs within the text block
        print(text)
        if text.startswith("https://content3"):
            print("Here:", row)
            writer.writerow(row)
            row.clear()
        clean_text = text.replace("\r", "").replace("\t", "")
        row.append(clean_text)'''

print("✅ CSV cleaned and written to", output_file)