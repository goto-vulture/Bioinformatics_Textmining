#!/bin/bash
# Test script for the program
# It tests the debug and release mode with C99 and C11 standard

# Important for a comma delimiter in float values !
LC_NUMERIC="de_DE.UTF-8"

MAKEFILE="Makefile"
PROG_NAME_DEBUG="./Bioinformatics_Textmining_Debug_Linux"
PROG_NAME_RELEASE="./Bioinformatics_Textmining_Release_Linux"
OUTPUT_FILE="./test.json"

C_STD=("STD=99" "STD=11")
MODES=("Debug=1" "Release=1")

# Test the CLI parameter
if [[ "$#" != 0 ]];
then
    echo "There are no CLI parameter allowed !"
    exit 1
fi

# Is a Makefile available ?
if [[ ! -f ${MAKEFILE} ]];
then
    echo "There is no Makefile ..."
    exit 1
fi
if [[ ! -r ${MAKEFILE} ]];
then
    echo "The Makefile is not readable !"
    exit 1
fi

##### ##### ##### BEGIN Bash functions ##### ##### #####
function Check_Last_Program_Call {
    if [[ ${last_call} -ne 0 ]];
    then
        echo "Last program call failed ! (Ret. value: ${last_call})"
        echo "${PROGRAM_OUTPUT}"
        exit 1
    fi
}
##### ##### ##### END Bash functions ##### ##### #####

##### ##### ##### BEGIN Test calls ##### ##### #####
for (( i=0; i<${#C_STD[@]}; i++ ))
do
    for (( j=0; j<${#MODES[@]}; j++ ))
    do
        printf "===== Test mode \"%s\" with \"%s\" =====\n" ${MODES[j]} ${C_STD[i]}

        program_output=$(make clean)
        last_call=${?}
        Check_Last_Program_Call

        program_output=$(make ${MODES[j]} ${C_STD[i]})
        last_call=${?}
        Check_Last_Program_Call

        if [[ ${j} -eq 0 ]];
        then
            program_output=$(time ${PROG_NAME_DEBUG} -T)
        else
            program_output=$(time ${PROG_NAME_RELEASE} -T)
        fi
        last_call=${?}

        # Delete the output file, if necessary (Do this before the program return value check to avoid
        # useless output files, if the last program call failed)
        if [[ -f ${OUTPUT_FILE} && -w ${OUTPUT_FILE} ]];
        then
            rm ${OUTPUT_FILE}
        fi

        Check_Last_Program_Call

        grep_output_1=$(echo ${program_output} | grep --extended-regexp "PASSED")
        grep_output_2=$(echo ${program_output} | grep --extended-regexp "FAILURE")

        # Are there no PASSED match in the program output ?
        if [[ -z ${grep_output_1} ]];
        then
            echo "Mode failed !"
            if [[ ! -z ${grep_output_2} ]];
            then
                printf "Error output: %s\n" ${grep_output_2}
            fi
        else
            echo "OK"
        fi

        echo ""
    done
done
##### ##### ##### END Test calls ##### ##### #####
