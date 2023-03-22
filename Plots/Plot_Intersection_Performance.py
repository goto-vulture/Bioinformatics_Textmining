import matplotlib.pyplot as plt
import os
import subprocess
import re
import time

PROGRAM_NAME = "Bioinformatics_Textmining_Release_Linux"
FIRST_INPUT_FILE = "./src/Tests/Test_Data/test_ebm.json"
SECOND_INPUT_FILE = "~/Downloads/Testdaten/intervention.txt"
OUTPUT_FILE = "./out.txt"
PROGRAM_CALLS = 5
NO_CPU_EXTENSIONS = "--no_cpu_extensions"

num_tokens = 0
num_sets = 0
num_tokens_first_file = 0
num_tokens_second_file = 0

REGEX_TOKENS_FOUND = "\d{1,} tokens found"
REGEX_TOKENS_FOUND_IN_RESULT = "Intersection tokens found:\s{1,}\d{1,}"
REGEX_CLEAN_TOKENS_FOUND = "\d{1,}"
REGEX_SETS_FOUND = "Intersection sets found:\s{1,}\d{1,}"
REGEX_CLEAN_SETS_FOUND = "\d{1,}"

#----------------------------------------------------------------------------------------------------------------------

def Real_Time(program_call, no_cpu_extensions=False):
    global num_tokens_first_file
    global num_tokens_second_file
    global num_tokens
    global num_sets

    matcher_sets = re.compile(REGEX_SETS_FOUND)
    matcher_clean_sets = re.compile(REGEX_CLEAN_SETS_FOUND)
    matcher_tokens = re.compile(REGEX_TOKENS_FOUND)
    matcher_clean_tokens = re.compile(REGEX_CLEAN_TOKENS_FOUND)
    matcher_tokens_in_result = re.compile(REGEX_TOKENS_FOUND_IN_RESULT)
    matcher_clean_tokens_in_result = re.compile(REGEX_CLEAN_TOKENS_FOUND)

    if no_cpu_extensions == True:
        program_call += " " + NO_CPU_EXTENSIONS

    start = time.perf_counter()
    p = subprocess.Popen(program_call, stdout=subprocess.PIPE, shell=True)
    (out, err) = p.communicate()
    p_status = p.wait()
    p.terminate()
    end = time.perf_counter()

    real_time = end - start
    # print("real: " + str(real_time))

    splitted_out = out.split (bytes('\n', "UTF-8"))
    for i in range(0, len(splitted_out)):
        match_tokens = matcher_tokens.search(splitted_out [i].decode())
        match_tokens_in_result = matcher_tokens_in_result.search(splitted_out [i].decode())
        match_sets = matcher_sets.search(splitted_out [i].decode())

        if match_tokens is not None:
            match_clean_tokens = matcher_clean_tokens.search(match_tokens.group(0))
            if num_tokens_first_file == 0:
                num_tokens_first_file = int(match_clean_tokens.group(0))
            elif num_tokens_second_file == 0:
                num_tokens_second_file = int(match_clean_tokens.group(0))

        if match_tokens_in_result is not None:
            match_clean_tokens_in_result = matcher_clean_tokens_in_result.search(match_tokens_in_result.group(0))
            num_tokens = int(match_clean_tokens_in_result.group(0))
        if match_sets is not None:
            match_clean_sets_in_result = matcher_clean_sets.search(match_sets.group(0))
            num_sets = int(match_clean_sets_in_result.group(0))

    return real_time

#----------------------------------------------------------------------------------------------------------------------

def CPU_Time(program_call, no_cpu_extensions=False):
    if no_cpu_extensions == True:
        program_call += " " + NO_CPU_EXTENSIONS

    p = subprocess.Popen(program_call, shell=True)
    pid, exit_status, resource_usage = os.wait4(p.pid, 0)

    user_time = resource_usage.ru_utime
    system_time = resource_usage.ru_stime

    print("user: " + str(user_time))
    print("system: " + str(system_time))
    print("CPU: " + str(user_time + system_time))

#======================================================================================================================

if __name__ == "__main__":
    program_call = "time ./" + PROGRAM_NAME + " -i " + FIRST_INPUT_FILE + " -j " + SECOND_INPUT_FILE + " -o " + OUTPUT_FILE + " " + NO_CPU_EXTENSIONS

    # Determine the number of tokens in the two input files
    # The result about the used time will be ignored for the average runtime
    print("Determine number of tokens in input files ...")
    Real_Time(program_call)
    print("First file:    " + str(num_tokens_first_file) + " (\"" + FIRST_INPUT_FILE + "\")")
    print("Second file:   " + str(num_tokens_second_file) + " (\"" + SECOND_INPUT_FILE + "\")")
    print("Result tokens: " + str(num_tokens))
    print("Result sets:   " + str(num_sets) + "\n")

    runtimes = []

    for i in range(0, PROGRAM_CALLS):
        print("Run: " + str(i + 1) + " / " + str(PROGRAM_CALLS))

        # Call the program
        curr_runtime = Real_Time(program_call, False)
        runtimes.append(curr_runtime)
        print(str(curr_runtime) + "s\n")

    runtimes_sum = 0.0
    for i in range(0, len(runtimes)):
        runtimes_sum += runtimes[i]
        print(str(runtimes[i]) + "s")

    average_runtime = runtimes_sum / PROGRAM_CALLS
    intersection_operations = num_tokens_first_file * num_tokens_second_file
    print("\nAvg time:                                     " + str(average_runtime) + "s")
    print("Number of intersection operations:            " + str(int(intersection_operations / 1000000)) + " Mio.")
    print("Number of intersection operations per second: " + str(int(intersection_operations / average_runtime / 1000000)) + " Mio.")

# => Used files <=
# First file:    51784 ("./src/Tests/Test_Data/test_ebm.json")
# Second file:   1703064 ("~/Downloads/Testdaten/intervention.txt")
# Result tokens: 424454
# Result sets:   202008

### AMD Athlon 5350 (2.1GHz) 64 bit | SSE4.1 ###
# Avg time:                                     126.85447577279993s
# Number of intersection operations:            88191 Mio.
# Number of intersection operations per second: 695 Mio.

### AMD Athlon 5350 (2.1GHz) 64 bit | NO EXTENSIONS ###
# Avg time:                                     312.244104743599s
# Number of intersection operations:            88191 Mio.
# Number of intersection operations per second: 282 Mio.


### AMD Ryzen 5 5600G (3.9GHz - 4.4GHz) 64 bit | AVX2 ###
# Avg time:                                     18.92501155539994s
# Number of intersection operations:            88191 Mio.
# Number of intersection operations per second: 4660 Mio.

### AMD Ryzen 5 5600G (3.9GHz - 4.4GHz) 64 bit | NO EXTENSIONS ###
# Avg time:                                     71.96592135119991s
# Number of intersection operations:            88191 Mio.
# Number of intersection operations per second: 1225 Mio.


### Intel Core i5-3320M (2.6GHz - 3.3GHz) 64 bit | SSE4.1 ###
# Avg time:                                     42.19706262219997s
# Number of intersection operations:            88191 Mio.
# Number of intersection operations per second: 2089 Mio.

### Intel Core i5-3320M (2.6GHz - 3.3GHz) 64 bit | NO EXTENSIONS ###
# Avg time:                                     110.84809149160006s
# Number of intersection operations:            88191 Mio.
# Number of intersection operations per second: 795 Mio.


#======================================================================================================================
