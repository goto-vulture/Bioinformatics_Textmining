#!/bin/bash
# Simple measurement of the execution time with the time command

# Important for a comma delimiter in float values !
LC_NUMERIC="de_DE.UTF-8"

INPUT_FILE_1=0
INPUT_FILE_2=0
OUTPUT_FILE="./test.json"
MAKEFILE="Makefile"
PROG_NAME="./Bioinformatics_Textmining_Release_Linux"

SEC_TOTAL_ADDED=0
AVERAGE_SEC=0
TOTAL_RUNS=2

MAX_RUNTIME=0
MIN_RUNTIME=999999 # To make sure, that min runtime is not zero because of an initialization with zero

##### ##### ##### ##### ##### BEGIN Check CLI input ##### ##### ##### ##### #####
# Test the CLI parameter
if [[ "$#" == 0 ]];
then
    echo "Missing parameter ! (input file 1 and input file 2)"
    exit 1
elif [[ "$#" == 1 ]];
then
    echo "Missing parameter ! (input file 2)"
    exit 1
elif [[ "$#" == 2 ]];
then
    INPUT_FILE_1=${1}
    INPUT_FILE_2=${2}
elif [[ "$#" == 3 ]];
then
    INPUT_FILE_1=${1}
    INPUT_FILE_2=${2}
    TOTAL_RUNS=${3}
else
    printf "Too much CLI parameter ! 2 - 3 parameter (both input files and an optional number of program runs) are expected ! Got %d !\n" "$#"
    exit 1
fi

# Test the input files
if [[ ! -f ${INPUT_FILE_1} ]]; then
    echo "The file \"${INPUT_FILE_1}\" not exits !"
    exit 1
fi
if [[ ! -f ${INPUT_FILE_2} ]]; then
    echo "The file \"${INPUT_FILE_2}\" not exits !"
    exit 1
fi
if [[ ! -r ${INPUT_FILE_1} ]]; then
    echo "The file \"${INPUT_FILE_1}\" is not readable !"
    exit 1
fi
if [[ ! -r ${INPUT_FILE_2} ]]; then
    printf "The file \"${INPUT_FILE_2}\" is not readable !"
    exit 1
fi

# Check total runs, if it was given
if [[ "$#" == 3 ]]; then
    if [[ ! ${3} =~ ^-?[0-9]+$ ]]; then
        echo "\"${3}\" is not an integer !"
        exit 1
    fi
    if [[ ${3} -lt 1 ]]; then
        echo "${3} is less than 1 !"
        exit 1
    fi
fi
##### ##### ##### ##### ##### END Check CLI input ##### ##### ##### ##### #####

echo "Using \"${INPUT_FILE_1}\" as first input file"
echo "Using \"${INPUT_FILE_2}\" as second input file"
echo "Using ${TOTAL_RUNS} program runs"
echo ""

if [[ ! -f ${MAKEFILE} ]]; then
    echo "There is no Makefile ..."
    exit 1
fi
if [[ ! -r ${MAKEFILE} ]]; then
    echo "The Makefile is not readable !"
    exit 1
fi

# Recreate the full program
make clean 1>> /dev/null
if [[ ${?} -ne 0 ]]; then
    echo "Last make call failed (make clean) !"
    exit 1
fi
time make Release=1 NO_DOCU=1
if [[ ${?} -ne 0 ]]; then
    echo "Build failed !"
    exit 1
fi
if [[ ! -f ${PROG_NAME} ]]; then
    echo "There is no program file with the name \"${PROG_NAME}\" !"
    exit 1
fi
if [[ ! -e ${PROG_NAME} ]]; then
    echo "The program \"${PROG_NAME}\" is not executable !"
    exit 1
fi
echo ""

##### ##### ##### ##### ##### BEGIN Program calls ##### ##### ##### ##### #####
for (( i=0; i<${TOTAL_RUNS}; i++ ))
do
    printf "===== Program run %3d / %3d =====\n" $(( ${i} + 1 )) ${TOTAL_RUNS}

    PROGRAM_OUTPUT=$(time ( ${PROG_NAME} -i ${INPUT_FILE_1} -j ${INPUT_FILE_2} -o ${OUTPUT_FILE} ) 2>&1 1> /dev/null)
    last_call=${?}

    # Delete the output file, if necessary
    if [[ -f ${OUTPUT_FILE} && -w ${OUTPUT_FILE} ]]; then
        rm ${OUTPUT_FILE}
    fi

    if [[ ${last_call} -ne 0 ]]; then
        echo "Last program call failed ! (Ret. value: ${last_call})"
        echo "${PROGRAM_OUTPUT}"
        exit 1
    fi

    # Get "real" time information out of the output
    TIME_INFO=$(echo ${PROGRAM_OUTPUT} | grep --extended-regexp --only-matching "real[[:blank:]]{1}[[:digit:]]{1,}m[[:digit:]]{1,2}(\.|\,)[[:digit:]]{3}s")

    # Split time informationen on spaces
    SPLITTED_TIME_INFO=($TIME_INFO)

    # Extract min. and sec. information
    MIN=$(echo ${SPLITTED_TIME_INFO[1]} | cut -d 'm' -f 1)
    SEC=$(echo ${SPLITTED_TIME_INFO[1]} | cut -d 'm' -f 2 | cut -d 's' -f 1 | tr , .)

    # Are there information in the variables ?
    if [[ -z ${MIN} ]];
    then
        echo "No minute output found! Maybe the programm exited abnormal (e.g. Segfault)."
        exit 1
    fi
    if [[ -z ${SEC} ]];
    then
        echo "No second output found! Maybe the programm exited abnormal (e.g. Segfault)."
        exit 1
    fi

    SEC_TOTAL=$(awk "BEGIN {print ${SEC} + 60 * ${MIN}}")
    SEC_TOTAL_ADDED=$(awk "BEGIN {print ${SEC_TOTAL} + ${SEC_TOTAL_ADDED}}")

    # Refresh min and max values
    if (( $(echo "${SEC_TOTAL} < ${MIN_RUNTIME}" | bc -l) )); then
        MIN_RUNTIME=${SEC_TOTAL}
    fi
    if (( $(echo "${SEC_TOTAL} > ${MAX_RUNTIME}" | bc -l) )); then
        MAX_RUNTIME=${SEC_TOTAL}
    fi

    printf "Runtime: %8.3f s\n\n" $(echo ${SEC_TOTAL} | tr . ,)
done
##### ##### ##### ##### ##### END Program calls ##### ##### ##### ##### #####

AVERAGE_SEC=$(awk "BEGIN {print ${SEC_TOTAL_ADDED} / ${TOTAL_RUNS}}")

printf "=> Average runtime: %8.3f sek.\n" $(echo ${AVERAGE_SEC} | tr . ,)
printf "   Min. runtime:    %8.3f sek.\n" $(echo ${MIN_RUNTIME} | tr . ,)
printf "   Max. runtime:    %8.3f sek.\n" $(echo ${MAX_RUNTIME} | tr . ,)
