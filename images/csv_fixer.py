import csv
import re

def clean_csv(input_file, output_file):
    """
    Reads the raw text from the input_file (assuming it is a csv from the prado dataset) and
    cleaning the text and generating a new csv file with each row cleanly formatted on new lines

    :param input_file: string file path to the file to read from
    :param output_file: string file path to the file to write to
    """
    with open(input_file, mode="r", encoding="utf-8", newline='') as infile, \
            open(output_file, mode="w", encoding="utf-8", newline='') as outfile:
        raw_text = infile.read()  # Reading raw text for processing
        writer = csv.writer(outfile)  # To save cleaned csv to

        raw_rows = re.split(r'\n(?=https://)', raw_text)  # Split raw text into all the rows (every row starts with \n and https link)
        for row_text in raw_rows:
            row_text = row_text.replace("\n", "").replace("\r", "").replace("\t", "")  # Remove whitespace
            row_text = row_text.replace(",,", ",N/A,")  # Fill empty cells with N/A
            row_text = row_text.replace(",,", ",N/A,")  # Have to do this again to fill remaining empty cells with N/A (Only one pass does this: ',,,' -> ',N/A,,')
            row_text = row_text.replace('""', '~')  # Replace double quotes for list/array items within cells with special character ~
            columns = re.findall(r'\"[^\"]*\"|[^,]+', row_text)  # Get each cell item (column) using regex
            clean_columns = []  # Represents all the columns in the row
            for column in columns:
                clean_columns.append(column.replace(",", "|"))  # Replace commas within cell items (columns) with special character |
            writer.writerow(clean_columns)  # Write the cleaned columns
    print("CSV cleaned and written to", output_file)
        

if __name__ == '__main__':
    infile = "prado.csv"
    clean_csv(infile, "clean_" + infile)
