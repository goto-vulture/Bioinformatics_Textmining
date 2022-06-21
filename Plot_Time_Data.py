import matplotlib.pyplot as plt
import subprocess
import re

program_name                            = "./Bioinformatics_Textmining_Debug_Linux"
regex_2_nested_loops_time               = "Mode: 2 nested loops:\s{1,}\d{1,}\.\d{4} sec\."
regex_qsort_and_binary_search_time      = "Mode: QSort and binary search:\s{1,}\d{1,}\.\d{4} sec\."
regex_heapsort_and_binary_search_time   = "Mode: Heap\-Sort and binary search:\s{1,}\d{1,}\.\d{4} sec\."
regex_clean_time                        = "\d{1,}\.\d{4}"



#======================================================================================================================

if __name__ == "__main__":
    # Testprogramm aufrufen und die Ausgabe speichern
    p = subprocess.Popen(program_name, stdout=subprocess.PIPE, shell=True)

    (out, err) = p.communicate()
    p_status = p.wait()
    p.terminate()

    splitted_out = out.split (bytes('\n', "UTF-8"))

    matcher_2_nested_loops = re.compile(regex_2_nested_loops_time)
    matcher_qsort_and_binary_search = re.compile(regex_qsort_and_binary_search_time)
    matcher_heapsort_and_binary_search = re.compile(regex_heapsort_and_binary_search_time)
    matcher_clean_time = re.compile(regex_clean_time)

    list_2_nested_loops_times = []
    list_qsort_and_binary_search_list = []
    list_heapsort_and_binary_search_list = []

    for i2 in range(0, len(splitted_out)):
        match_2_nested_loops = matcher_2_nested_loops.match(splitted_out [i2].decode())
        match_qsort_and_binary_search = matcher_qsort_and_binary_search.match(splitted_out [i2].decode())
        match_heapsort_and_binary_search = matcher_heapsort_and_binary_search.match(splitted_out [i2].decode())

        if match_2_nested_loops is not None:
            clean_result = matcher_clean_time.search(match_2_nested_loops.group())
            print (clean_result.group())
            list_2_nested_loops_times.append(float(clean_result.group()))
        if match_qsort_and_binary_search is not None:
            clean_result = matcher_clean_time.search(match_qsort_and_binary_search.group())
            print (clean_result.group())
            list_qsort_and_binary_search_list.append(float(clean_result.group()))
        if match_heapsort_and_binary_search is not None:
            clean_result = matcher_clean_time.search(match_heapsort_and_binary_search.group())
            print (clean_result.group())
            list_heapsort_and_binary_search_list.append(float(clean_result.group()))

    MAX_ARRAY_LENGTH    = 15000
    NUMBER_OF_ARRAYS    = 10
    NUMBER_OF_RUNS      = 50
    DIV_VALUE           = 1000

    x_axis = []
    min_x_axis = ((MAX_ARRAY_LENGTH / NUMBER_OF_RUNS) * (1)) / DIV_VALUE
    max_x_axis = ((MAX_ARRAY_LENGTH / NUMBER_OF_RUNS) * (NUMBER_OF_RUNS)) / DIV_VALUE
    for i in range(0, NUMBER_OF_RUNS):
        x_axis.append(((MAX_ARRAY_LENGTH / NUMBER_OF_RUNS) * (i + 1)) / DIV_VALUE)

    min_measure_list = []
    max_measure_list = []

    min_measure_list.append(min(list_2_nested_loops_times))
    max_measure_list.append(max(list_2_nested_loops_times))

    min_measure_list.append(min(list_qsort_and_binary_search_list))
    max_measure_list.append(max(list_qsort_and_binary_search_list))

    min_measure_list.append(min(list_heapsort_and_binary_search_list))
    max_measure_list.append(max(list_heapsort_and_binary_search_list))

    # Y-Achsen Skalierung
    min_y_axis = min(min_measure_list)
    max_y_axis = max(max_measure_list)

    # Daten visualisieren
    plt.plot(x_axis, list_2_nested_loops_times, "ro",
            label="Two nested loops\n" + r"$\mathcal{O}(n²)$")
    plt.plot(x_axis, list_qsort_and_binary_search_list, "bo",
            label="QSort + binary search\n" + r"$\mathcal{O}(n \cdot log(n)) + \mathcal{O}(log(n))$")
    plt.plot(x_axis, list_heapsort_and_binary_search_list, "go",
            label="Heap-Sort + binary search\n" + r"$\mathcal{O}(n \cdot log(n)) + \mathcal{O}(log(n))$")
    plt.axis([min_x_axis, max_x_axis, min_y_axis, max_y_axis])

    plt.xlabel("Number of elements " + r"$\cdot$" + " 1000 per subset (" +
            r"$\vert M_{1,i} \vert$" + " and " + r"$\vert M_{2,i}\vert$" + ")\nI := {1, ..., " + str(NUMBER_OF_ARRAYS) + "}")
    #plt.xlabel("Number of elements " + r"$\cdot$" + " 1000 per subset (" +
    #        r"$\vert M_{1,i} \vert$" + "\n" + r"$\vert M_{2,i} \vert$ = 5" + "    I := {1, ..., " + str(NUMBER_OF_ARRAYS) + "}")
    plt.ylabel("Runtime in seconds")
    plt.title("Comparison intersection runtime")

    plt.legend()
    plt.show()

#======================================================================================================================
