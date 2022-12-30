import json
import sys

if __name__ == "__main__":
    if len(sys.argv) != 2:
        print("Except exact one parameter (The JSON file) ! Got " + str(len(sys.argv) - 1) + " parameter !")
        exit(1)
    file_name = sys.argv[1]

    print("Load file \"" + file_name + "\"")
    with open(file_name) as f:
        file_content = json.load(f)
    print("Input file seems to be JSON compatible :D")
