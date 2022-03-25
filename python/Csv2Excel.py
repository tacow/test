import sys
import pandas

def csv_to_xlsx(csvFile, xlsxFile, encoding):
    csv = pandas.read_csv(csvFile, encoding=encoding)
    csv.to_excel(xlsxFile, sheet_name='Sheet 1')

if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: %s [CSV File] [XLSX File] <CSV File Encoding>" % sys.argv[0])
        sys.exit()
    csvFile = sys.argv[1]
    xlsxFile = sys.argv[2]
    
    encoding = 'UTF-8'
    if len(sys.argv) >= 4:
        encoding = sys.argv[3]
    csv_to_xlsx(csvFile, xlsxFile, encoding)