# Bioinformatics Textmining

## Introduction

In our current time, more information will be collected than ever before. This trend extends to almost every area in our life. In the private area and also in the professional environment. Especially in science, an above average increase can be observed.

In order to find more knowledge in this massive amount of data, the data will be placed and connected with other data, that was created in a comparable topic. Here is a simple example to show the general idea:

For example, when looking at Wikipedia articles that contains meaning-related topics. Without going in the content, new knowledge can be found by looking at which words occur in all articles. This makes it possible to find topics that play a role in each article.

With small amounts of data, this sounds easy and trivial. But with larger amounts of data, a search of this type through the data is only possible with the help of computers.



## General idea

For a efficient search process, all words will be encoded in integers. With this encoded data, simple integer comparisons are possible in shortest time.

With the help of some elementary set operations (the main important one: intersection), the set of words will be created, that appear in every source set.



## Example calculation
The first input file is usual a JSON file with the abstracts of many thousands studies:
```JSON
"tokens": [
    "Randomized",
    "phase",
    "II",
    "study",
    "of",
    "two",
    "different",
    "schedules",
    "of",
    "gemcitabine",
    "and",
    "oral",
    "..."]
```
The second input file is a "database" with interesting token sets. These sets can be formatted as JSON file or as simple text files. A JSON example:
```JSON
"tokens": ["gemcitabine", "in", "treatment", "arm", "1"]
```

A possible output of the calculation could be:
```JSON
"name_2_3":	{
    "tokens":	["gemcitabine", "in", "treatment", "arm", "1"],
    "tokens w/o stop words":	["gemcitabine", "treatment", "arm"],
    "Inters. (partial)":	{
        "23357440":	{
            "tokens":	["treatment", "arm"],
            "char offs.":	[50, 887],
            "word offs.":	[6, 140]
        },
        "20189026":	{
            "tokens":	["arm", "treatment"],
            "char offs.":	[807, 1298],
            "word offs.":	[111, 184]
        },
    },
    "Inters. (full)":	{
        "20147856":	{
            "tokens":	["gemcitabine", "treatment", "arm"],
            "char offs.":	[56, 440, 583],
            "word offs.":	[9, 66, 99]
        }
    }
},
```
With intersection operations (and many pre-calculation) between the different sets of tokens it is possible to decide, whether a list of tokens appear partial or full in abstracts of studies. With the char and word offsets it is possible to approximate the relevance of a set of tokens for a list with thousands studies. 



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
- `-k`, `--keep_single_token_results`: Keep results with only one token (By default single tokens results will be discarded)
- `-n`, `--no_cpu_extensions`: Don't use CPU extensions, even if there available on the host

- `-h`, `--help`: Show a help message and exit

Debugging arguments:
- `-A`, `--abort=<float>`: Abort the calculation after X percent
- `-T`, `--run_all_test_functions`: Running all test functions. This argument overrides all other arguments, except -h. (Only useful for debugging)



## Technical information

To make the implementation simple, the in and output will be created in a simple way. The name of the input files will be passed trough CLI parameter and the results will be represented as JSON file.

If available the program uses SSE and AVX2 extensions, if the host support these extensions. As fallback, whether a support is not available or not detectable, portable C code is used. (With the option `-n` or `--no_cpu_extensions` the usage of CPU extensions can be disabled.)

### Used libraries

- *argparse*: Simple and powerful CLI parser. Here available on [GitHub](https://github.com/cofyc/argparse).
- *cJSON*: Lightweight (and beautiful) JSON parser. Also available on [GitHub](https://github.com/DaveGamble/cJSON).
- *TinyTest*: Simple and tiny test suite. You can find it here on [GitHub](https://github.com/joewalnes/tinytest).



## Performance test

Infos about the performance test:
- First file (1.8 MB): ~52.000 tokens
- Second file (115 MB): ~1.7 Mio. tokens
- Result file: ~420.000 tokens in ~200.000 sets
- Intersection operations: ~88.200 Mio.

| Used CPU | Frequency (GHz) | Used CPU extension | Avg. time (s) |
|----------|-----------------|--------------------|---------------|
| AMD Ryzen 5 5600G | 3.9 - 4.4 | AVX2 | 18.9 |
| AMD Ryzen 5 5600G | 3.9 - 4.4 | n/a | 42.2 |
| AMD Ryzen 3 4300U | 2.7 - 3.7 | AVX2 | 25.6 |
| AMD Ryzen 3 4300U | 2.7 - 3.7 | n/a | 72.8 |
| AMD Athlon 5350 | 2.1 | SSE4.1 | 126.8 |
| AMD Athlon 5350 | 2.1 | n/a | 312.2 |
| Intel i5-3220M | 2.6 - 3.3 | SSE4.1 | 42.2 |
| Intel i5-3220M | 2.6 - 3.3 | n/a | 110.8 |
| Intel Atom N450 | 1.6 | SSE2 | 340.1 |
| Intel Atom N450 | 1.6 | n/a | 563.9 |

It seems to be particularly helpful to use a SIMD CPU extension, because a intersection operation can be converted to a matrix operation. Even on a low cost CPU from 2010 (N450) with an outdated extension (SSE2 is from 2001) the usage of such a extension will reduce the runtime about ~40%. On other systems a reduction of more than 50% seems to be the normal case.

It would be interesting to compare this results with a CPU, that supports AVX512. Unfortunately I don't own such a CPU.


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
