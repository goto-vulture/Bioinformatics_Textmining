# Bioinformatics Textmining

## Introduction

In our current time, more information will be collected than ever before. This trend extends to almost every area in our life. In the private area and also in the professional environment. Especially in science, an above average increase can be observed.

In order to find more knowledge in this massive amount of data, the data will be placed and connected with other data, that was created in a comparable topic. Here is a simple example to show the general idea:

For example, when looking at Wikipedia articles that contains meaning-related topics. Without going in the content, new knowledge can be found by looking at which words occur in all articles. This makes it possible to find topics that play a role in each article.

With small amounts of data, this sounds easy and trivial. But with larger amounts of data, a search of this type through the data is only possible with the help of computers.



## General idea

For a efficient search process, all words will be encoded in integers. With this encoded data, simple integer comparisons are possible in shortest time.

With the help of some elementary set operations (the main important one: intersection), the set of words will be created, that appear in every source set.



## Structure of the evaluation
<img src="https://github.com/goto-vulture/Bioinformatics_Textmining/blob/Misc/Written_elaboration/Structure_of_the_evaluation_process_EN.png" width="50%" height="50%">



## Usage

### Building

make is the provided tool to build the program. To build the program with the default settings a simple `make` call is enough, because the program (and the used libraries) uses only the C standard lib. The compiler needs at least a C99 support; with a C11 support some C11 specific enhancements like `_Generic` and `_Static_assert` can be used.

There are two switches for the building process:
- `DEBUG`: Building the program with debug information in the binary file (default)
- `RELEASE`: Building with compiler optimization.

Another argument for the makefile is the C standard:
- `STD` or `std`: STD=99 for C99 respectively STD=11 for C11 (C11 is the default setting)

Some build examples:
- `make Debug=1 STD=99`: Build the project with debug settings and the C99 standard.
- `make Release=1`: Build the project with release settings and the C11 standard.

`make clean` removes all compilation files - including the object files.

### A simplified building tutorial

There is no universal building instruction possible without any expectations about the used platform. For the following tutorial I assume, that Linux as OS will be used. With Linux the compilation should be work out of the box. A compilation with Windows is also possible, but the installation of a C compiler and the Make build tool is more complicated. There are many tutorials available in the internet for example [here](https://www.freecodecamp.org/news/how-to-install-c-and-cpp-compiler-on-windows/). In details the installation way differs depending on the Windows version.

- Download the repository as ZIP file via the github web page
- Extract the ZIP file
- Go into the new created folder
- Use the `make` command without any parameter; this will build the program in debug mode
- With Linux as OS the compiled program name will be "Bioinformatics_Textmining_Debug_Linux"
- Call the program with `./Bioinformatics_Textmining_Debug_Linux` followed by the CLI parameter 

E.g.: If you want to run the program with the first source file "input.json" and the second source file "input2.txt" the program will create a output file "output.json" with the following command: `./Bioinformatics_Textmining_Debug_Linux -i "./input.json" -j "./input2.txt" -o "./output.json"`

This example command expects, that the source files are in the same directory as the program file.

### Interface

The CLI accepts the following main inputs, that are required:
- `-i`, `--input=<str>`: First input file
- `-j`, `--input2=<str>`: Second input file
- `-o`, `--output=<str>`: Output file

Optional arguments:
- `-f`, `--format`: Format the output for better readability in a normal editor?
- `-s`, `--sentence_offset`: Calculate sentence offsets? (In normal cases this is not necessary)
- `-w`, `--word_offset`: Calculate word offsets?
- `--show_too_long_tokens`: Show too long tokens in the export file
- `--no_part_matches`: No part matches will appear in the export file
- `--no_full_matches`: No full matches will appear in the export file
- `-h`, `--help`: Show a help message and exit

Debugging arguments:
- `-A`, `--abort=<float>`: Abort the calculation after X percent
- `-T`, `--run_all_test_functions`: Runing all test functions. This argument overrides all other arguments, except -h. (Only useful for debugging)



## Technical information

To make the implementation simple, the in and output will be created in a simple way. The name of the input files will be passed trough CLI parameter and the results will be represented as JSON file.

### Used libraries

- *argparse*: Simple and powerful CLI parser. Here available on [GitHub](https://github.com/cofyc/argparse).
- *cJSON*: Lightweight (and beautiful) JSON parser. Also available on [GitHub](https://github.com/DaveGamble/cJSON).
- *TinyTest*: Simple and tiny test suite. You can find it here on [GitHub](https://github.com/joewalnes/tinytest).



## Future features

- Determine the end of sentences (Using POS tags from the source files; list of common abbreviations; 3rd party sentence tokenizer)
- Calculate the error rate of the sentence end determining
- "Smart" Case insensitive comparison of the tokens:
    - Abbreviations should be compared case-sensitive
    - They need to be appear in their original representation in the result file
    - In all other cases a case insensitive comparison
- A more user-friendly interface. Maybe with a GUI. But the priority in this project is low.



## Help

If you have any questions or suggestions: Get in contact with me: goto-vulture@gmx.de
