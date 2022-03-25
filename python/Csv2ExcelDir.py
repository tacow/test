import os
import sys
import pandas

def csv_to_xlsx(csvFile, xlsxFile, encoding):
    csv = pandas.read_csv(csvFile, encoding=encoding)
    csv.to_excel(xlsxFile, sheet_name='Sheet 1')

def csv_to_xlsx_dir(csvDir, encoding):
    entries = os.listdir(csvDir)
    for entry in entries:
        csvPath = os.path.join(csvDir, entry)
        if not os.path.isfile(csvPath):
            continue
        root, ext = os.path.splitext(csvPath)
        if ext.lower() != '.csv':
            continue
        xlsxPath = root + '.xlsx'
        csv_to_xlsx(csvPath, xlsxPath, encoding)

if __name__ == '__main__':
    if len(sys.argv) < 2:
        print("Usage: %s [CSV Dir] <CSV File Encoding>" % sys.argv[0])
        sys.exit()
    csvDir = sys.argv[1]
    
    encoding = 'UTF-8'
    if len(sys.argv) >= 3:
        encoding = sys.argv[2]
    csv_to_xlsx_dir(csvDir, encoding)