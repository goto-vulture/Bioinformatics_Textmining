import matplotlib.pyplot as plot
import subprocess

program_name = "./Bioinformatics_Textmining_Debug_Linux"
program_runs = 150



#======================================================================================================================

# Datei komplett einlesen
for i in range(0, program_runs):
    print ("%3d / %3d\r" % (i + 1, program_runs))

    # Testprogramm aufrufen und die Ausgabe speichern
    p = subprocess.Popen(program_name, stdout=subprocess.PIPE, shell=True)

    (out, err) = p.communicate()
    p_status = p.wait()
    p.terminate()

    splitted_out = out.split (bytes('\n', "UTF-8"))

    for i2 in range(0, len(splitted_out)):
        print(splitted_out [i2])

#======================================================================================================================
