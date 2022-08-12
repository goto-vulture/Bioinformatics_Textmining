##### ##### ##### ##### ##### Makefile Skript fuer das Praktikumsprojekt "Bioinformatics_Textmining" ##### ##### ##### ##### #####

CC = gcc
RM = rm
MKDIR = mkdir

# Pfad zu doxygen, falls es auf dem System vorhanden ist
DOXYGEN = doxygen
DOXYGEN_PATH = $(shell command -v $(DOXYGEN) 2> /dev/null)


# Flags, die sowohl im Debug- als auch im Release-Build, verwendet werden
CCFLAGS = -std=c11 -pedantic -Wall -Wextra -Wconversion -fmessage-length=0

# Verwendete Libs
LIBS = -lm

# Weitere hilfreiche Compilerflags
# Programmabbruch bei Ueberlauf von vorzeichenbehafteten Integers
# CCFLAGS += -ftrapv => Funktioniert leider nicht wie erhofft :(
# Warnung, wenn Gleitkommazahlen auf Gleichheit mittels == getestet werden
CCFLAGS += -Wfloat-equal
# Warnung, wenn Variablen ueberdeckt werden
CCFLAGS += -Wshadow
# Warnung, wenn irgendwas von der Groesse einer Funktion oder des void-Typs abhaengt
CCFLAGS += -Wpointer-arith
# Fest codierte Strings bekommen den Typ const char*
CCFLAGS += -Wwrite-strings
# Warnung, wenn ein switch Ausdruck keinen default Pfad besitzt
CCFLAGS += -Wswitch-default
# Warnung, wenn bei einem switch Ausdruck ein enum Typ verwendet wird und nicht alle moeglichen Konstanten im switch Ausdruck verwendet werden
CCFLAGS += -Wswitch-enum
# Warnung, wenn Code existiert, der nie erreicht werden kann
CCFLAGS += -Wunreachable-code
# Warnung, wenn eine Funktion ohne Parameterangabe deklariert oder definiert wird
CCFLAGS += -Wstrict-prototypes
# Warnung, wenn Formatstrings dynamisch erzeugt werden und und nicht zur Kompilierungszeit bekannt sind
CCFLAGS += -Wformat-nonliteral
# Warnung, wenn ein Wert sich selbst zugewiesen wird
CCFLAGS += -Winit-self
# Warnung, wenn Funktionen, die einen Formatstring erwarten, diesen nicht bekommen
CCFLAGS += -Wformat-security
# Einige weitere Moeglichkeiten den Code etwas sicherer zu machen => Diese Flags werden als systemspezifische Flags verwendet,
# da Windows mit der Standardauswahl nicht arbeiten kann ...
# CCFLAGS += -fstack-protector -Wl,-z,relro -Wl,-z,now


# Debug Build: Keine Optimierung und das hoechste Debug Level
DEBUG_FLAGS = -O0 -g3 -D_FORTIFY_SOURCE=2
# Eine Praeprozessorkonstante setzen, wenn im Debug-Modus das Programm uebersetzt wird
DEBUG_FLAGS += -DDEBUG_BUILD

# Release Build: Hoechste Optimierung und keine Debug Informationen
RELEASE_FLAGS = -O3
# Optimierungen fuer den Linker (Ich wusste vorher gar nicht, dass es so ein Flag auch fuer den Linker gibt ... :o)
RELEASE_FLAGS += -Wl,-O1
# Positionsunabhaengigen Code erzeugen
RELEASE_FLAGS += -fPIE
# Eine Praeprozessorkonstante setzen, wenn im Release-Modus das Programm uebersetzt wird
RELEASE_FLAGS += -DRELEASE_BUILD



DEBUG = 0
RELEASE = 0

PROJECT_NAME = Bioinformatics_Textmining
DOCUMENTATION_PATH = ./Documentation
NO_DOCUMENTATION = 0

# addsuffix, welches einen String am Ende einer Variable anbringt, kann das Ergebnis NICHT einer Variablen zuweisen, wenn diese
# Variable im Aufruf von addsuffix vorhanden ist !
# D.h.: test += $(addsuffix _X_, $(test)) ist NICHT moeglich !
# Daher der Umweg ueber mehrere Variablen
TEMP_1 =
TARGET =

# Zusaetzliche Flags fuer Linux
ADDITIONAL_LINUX_FLAGS = -fstack-protector -Wl,-z,relro -Wl,-z,now

# Zusaetzliche Flags fuer Windows
# Unter Windows gibt es bei Format-Strings einige Probleme !
# -Wno-pedantic-ms-format:    Bestimmte Warnungen, die die Format-Strings betreffen, abschalten
# -D__USE_MINGW_ANSI_STDIO=1: Standardmaessig ist der Formatstring "%z" fuer size_t Variablen abgeschaltet (warum auch immer) ...
#                             Dieses Flag schaltet das Element ein, sodass size_t Variablen richtig ausgegeben werden koennen
#                             Siehe: https://lists.gnu.org/archive/html/bug-gnulib/2014-09/msg00056.html
# Unter Windows fuehrt die Erzeugung des Stack-Protektors zu Segmentation Faults beim Aufruf von Funktionen (also wirklich
# WAEHREND des Aufrufes ! -> wird unter Windows nicht mehr verwendet.)
ADDITIONAL_WINDOWS_FLAGS = -Wno-pedantic-ms-format -D__USE_MINGW_ANSI_STDIO=1 # -fstack-protector

# Der Debug-Build ist die Standardvariante, wenn nichts anderes angegeben wurde
# Fuer den Release-Build muss die Variable "Release", "RELEASE" oder "release" auf 1 gesetzt werden
ifeq ($(Release), 1)
	CCFLAGS += $(RELEASE_FLAGS)
	TEMP_1 = $(addsuffix _Release_, $(PROJECT_NAME))
	RELEASE = 1
else
	ifeq ($(RELEASE), 1)
		CCFLAGS += $(RELEASE_FLAGS)
		TEMP_1 = $(addsuffix _Release_, $(PROJECT_NAME))
		RELEASE = 1
	else
		ifeq ($(release), 1)
			CCFLAGS += $(RELEASE_FLAGS)
			TEMP_1 = $(addsuffix _Release_, $(PROJECT_NAME))
			RELEASE = 1
		else
			CCFLAGS += $(DEBUG_FLAGS)
			TEMP_1 = $(addsuffix _Debug_, $(PROJECT_NAME))
			DEBUG = 1
		endif
	endif
endif

# Soll die Dokumentation mittels Doxygen erzeugt werden ? Die Erzeugung der Dokumentation benoetigt mit Abstand die meiste
# Zeit bei der Erstellung des Programms
# "NO_DOCUMENTATION", "NO_DOCU", "NO_DOCS": Alle CLI-Parameter schalten die Erzeugung der Doxygen-Dokumentation ab
ifeq ($(NO_DOCUMENTATION), 1)
	NO_DOCUMENTATION = 1
endif
ifeq ($(NO_DOCU), 1)
	NO_DOCUMENTATION = 1
endif
ifeq ($(NO_DOCS), 1)
	NO_DOCUMENTATION = 1
endif

# Unter Windows wird u.a. das Flag "-Wno-pedantic-ms-format" benoetigt, da die MinGW Implementierung nicht standardkonforme
# Formatstrings verwendet. Mit diesem Compilerflag wird die Warnung, dass die MinGW Formatstrings nicht dem Standard
# entsprechen, entfernt
#
# Unter Linux werden einige Flags verwendet, die unter Windows nicht verwendet werden koennen
ifeq ($(OS), Windows_NT)
	CCFLAGS += $(ADDITIONAL_WINDOWS_FLAGS)
	TARGET = $(addsuffix Win, $(TEMP_1))
else
	CCFLAGS += $(ADDITIONAL_LINUX_FLAGS)
	TARGET = $(addsuffix Linux, $(TEMP_1))
endif

##### ##### ##### BEGINN Uebersetzungseinheiten ##### ##### #####
MAIN_C = ./src/main.c

STR2INT_H = ./src/str2int.h
STR2INT_C = ./src/str2int.c

INT2STR_H = ./src/int2str.h
INT2STR_C = ./src/int2str.c

DYNAMIC_MEMORY_H = ./src/Error_Handling/Dynamic_Memory.h
DYNAMIC_MEMORY_C = ./src/Error_Handling/Dynamic_Memory.c

TINYTEST_H = ./src/Tests/tinytest.h
TINYTEST_C = ./src/Tests/tinytest.c

# Hiervon gibt es nur eine Headerdatei !
ASSERT_MSG_H = ./src/Error_Handling/Assert_Msg.h

ARGPARSE_H = ./src/argparse.h
ARGPARSE_C = ./src/argparse.c

CLI_PARAMETER_H = ./src/CLI_Parameter.h
CLI_PARAMETER_C = ./src/CLI_Parameter.c

PRINT_TOOLS_H = ./src/Print_Tools.h
PRINT_TOOLS_C = ./src/Print_Tools.c

STRING_TOOLS_H = ./src/String_Tools.h
STRING_TOOLS_C = ./src/String_Tools.c

DOCUMENT_WORD_LIST_H = ./src/Document_Word_List.h
DOCUMENT_WORD_LIST_C = ./src/Document_Word_List.c

TEST_DOCUMENT_WORD_LIST_H = ./src/Tests/TEST_Document_Word_List.h
TEST_DOCUMENT_WORD_LIST_C = ./src/Tests/TEST_Document_Word_List.c

CREATE_TEST_DATA_H = ./src/Tests/Create_Test_Data.h
CREATE_TEST_DATA_C = ./src/Tests/Create_Test_Data.c

INTERSECTION_APPROACHES_H = ./src/Intersection_Approaches.h
INTERSECTION_APPROACHES_C = ./src/Intersection_Approaches.c

FILE_READER_H = ./src/File_Reader.h
FILE_READER_C = ./src/File_Reader.c

TOKEN_INT_MAPPING_H = ./src/Token_Int_Mapping.h
TOKEN_INT_MAPPING_C = ./src/Token_Int_Mapping.c

CJSON_H = ./src/JSON_Parser/cJSON.h
CJSON_C = ./src/JSON_Parser/cJSON.c

TEST_CJSON_PARSER_H = ./src/Tests/TEST_cJSON_Parser.h
TEST_CJSON_PARSER_C = ./src/Tests/TEST_cJSON_Parser.c

MISC_H = ./src/Misc.h
MISC_C = ./src/Misc.c

EXEC_INTERSECTION_H = ./src/Exec_Intersection.h
EXEC_INTERSECTION_C = ./src/Exec_Intersection.c

STOP_WORDS_H = ./src/Stop_Words/Stop_Words.h
STOP_WORDS_C = ./src/Stop_Words/Stop_Words.c
##### ##### ##### ENDE Uebersetzungseinheiten ##### ##### #####



# Komplettes Projekt erstellen
all: $(TARGET)
	@echo

# Wenn doxygen auf den System nicht vorhanden ist, dann wird die Kompilierung fortgesetzt; aber ohne Erzeugung der Dokumentation
ifndef DOXYGEN_PATH
	@echo No $(DOXYGEN) in $(PATH). Skip the creation of the documentation.
	@echo If you want the program documentation, try to install $(DOXYGEN) with apt-get install $(DOXYGEN).
else
ifeq ($(NO_DOCUMENTATION), 0)
	@echo Generating the documentation ...
	$(DOXYGEN_PATH) ./Doxyfile
endif
endif

	@echo
	@echo Bioinformatics_Textmining build completed !

$(TARGET): main.o str2int.o int2str.o Dynamic_Memory.o tinytest.o argparse.o CLI_Parameter.o Print_Tools.o String_Tools.o Document_Word_List.o TEST_Document_Word_List.o Create_Test_Data.o Intersection_Approaches.o File_Reader.o Token_Int_Mapping.o cJSON.o TEST_cJSON_Parser.o Misc.o Exec_Intersection.o Stop_Words.o
	@echo
	@echo Linking object files ...
	@echo
	$(CC) $(CCFLAGS) -o $(TARGET) main.o str2int.o int2str.o Dynamic_Memory.o tinytest.o argparse.o CLI_Parameter.o Print_Tools.o String_Tools.o Document_Word_List.o TEST_Document_Word_List.o Create_Test_Data.o Intersection_Approaches.o File_Reader.o Token_Int_Mapping.o cJSON.o TEST_cJSON_Parser.o Misc.o Exec_Intersection.o Stop_Words.o $(LIBS)

##### BEGINN Die einzelnen Uebersetzungseinheiten #####
main.o: $(MAIN_C)
	@echo Build target: $(TARGET).
	@echo
ifeq ($(RELEASE), 1)
	@echo Using RELEASE build.
else
	@echo Using DEBUG build.
endif
	@echo
ifeq ($(NO_DOCUMENTATION), 1)
	@echo No documentation will be generated.
else
	@echo Documentation will be generated.
endif
	@echo
	$(CC) $(CCFLAGS) -c $(MAIN_C) $(DYNAMIC_MEMORY_H) $(ALKANE_H)

str2int.o: $(STR2INT_C)
	$(CC) $(CCFLAGS) -c $(STR2INT_C)

int2str.o: $(INT2STR_C)
	$(CC) $(CCFLAGS) -c $(INT2STR_C)

Dynamic_Memory.o: $(DYNAMIC_MEMORY_C)
	$(CC) $(CCFLAGS) -c $(DYNAMIC_MEMORY_C)

tinytest.o: $(TINYTEST_C)
	$(CC) $(CCFLAGS) -c $(TINYTEST_C)

argparse.o: $(ARGPARSE_C)
	$(CC) $(CCFLAGS) -c $(ARGPARSE_C)

CLI_Parameter.o: $(CLI_PARAMETER_C)
	$(CC) $(CCFLAGS) -c $(CLI_PARAMETER_C)

Print_Tools.o: $(PRINT_TOOLS_C)
	$(CC) $(CCFLAGS) -c $(PRINT_TOOLS_C)

String_Tools.o: $(STRING_TOOLS_C)
	$(CC) $(CCFLAGS) -c $(STRING_TOOLS_C)

Document_Word_List.o: $(DOCUMENT_WORD_LIST_C)
	$(CC) $(CCFLAGS) -c $(DOCUMENT_WORD_LIST_C)

TEST_Document_Word_List.o: $(TEST_DOCUMENT_WORD_LIST_C)
	$(CC) $(CCFLAGS) -c $(TEST_DOCUMENT_WORD_LIST_C)

Create_Test_Data.o: $(CREATE_TEST_DATA_C)
	$(CC) $(CCFLAGS) -c $(CREATE_TEST_DATA_C)

Intersection_Approaches.o: $(INTERSECTION_APPROACHES_C)
	$(CC) $(CCFLAGS) -c $(INTERSECTION_APPROACHES_C)

File_Reader.o: $(FILE_READER_C)
	$(CC) $(CCFLAGS) -c $(FILE_READER_C)

Token_Int_Mapping.o: $(TOKEN_INT_MAPPING_C)
	$(CC) $(CCFLAGS) -c $(TOKEN_INT_MAPPING_C)

cJSON.o: $(CJSON_C)
	$(CC) $(CCFLAGS) -c $(CJSON_C)

TEST_cJSON_Parser.o: $(TEST_CJSON_PARSER_C)
	$(CC) $(CCFLAGS) -c $(TEST_CJSON_PARSER_C)

Misc.o: $(MISC_C)
	$(CC) $(CCFLAGS) -c $(MISC_C)

Exec_Intersection.o: $(EXEC_INTERSECTION_C)
	$(CC) $(CCFLAGS) -c $(EXEC_INTERSECTION_C)
	
Stop_Words.o: $(STOP_WORDS_C)
	$(CC) $(CCFLAGS) -c $(STOP_WORDS_C)
##### ENDE Die einzelnen Uebersetzungseinheiten #####

# Alles wieder aufraeumen
clean:
	@echo Clean Bioinformatics_Textmining build.
	@echo
	@echo \> Deleting compilation files:
	$(RM) -f $(PROJECT_NAME)* *.o ./src/Error_Handling/*.gch gmon.out
	@echo
	@echo \> Deleting doxygen documentation:
	$(RM) -rf $(DOCUMENTATION_PATH)
	$(MKDIR) $(DOCUMENTATION_PATH)
	@echo
	@echo Bioinformatics_Textmining build cleaned.
