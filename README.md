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

make is the provided tool to build the program. To build the program with the default settings a simple `make` call is enough.

There are two switches for the building process:
- `DEBUG`: Building the program with debug information in the binary file (default)
- `RELEASE`: Building with compiler optimization.

`make clean` removes all compilation files.

### Interface

The CLI accepts the following main inputs, that are required:
- `-i`, `--input=<str>`: First input file
- `-j`, `--input2=<str>`: Second input file
- `-o`, `--output=<str>`: Output file

Optional arguments:
- `-h`, `--help`: Show a help message and exit
- `-A`, `--abort=<float>`: Abort the calculation after X percent
- `-T`, `--run_all_test_functions`: Runing all test functions (Only useful for debugging)



## Technical information

To make the implementation simple, the in and output will be created in a simple way. The name of the input files will be passed trough CLI parameter and the results will be represented as JSON file.

### Used libraries

- *argparse*: Simple and powerful CLI parser. Here available on [GitHub](https://github.com/cofyc/argparse).
- *cJSON*: Lightweight (and beautiful) JSON parser. Also available on [GitHub](https://github.com/DaveGamble/cJSON).
- *TinyTest*: Simple and tiny test suite. You can find it here on [GitHub](https://github.com/joewalnes/tinytest).



## Future features

- Sentence offsets (The current implementation allows only a char offset)
- Smaller result file with following ideas:
    - Remove tabs
    - Use abbreviations for array names in the JSON file (e.g.: "Intersections" -> "Inters.")
    - Case insensitive comparison of the tokens
- A more user-friendly interface. Maybe with a GUI. But the priority in this project is low.
