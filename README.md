# Bioinformatics Textmining

## EN (See german translation below)

### Introduction

In our current time, more information will be collected than ever before. This trend extends to almost every area in our life. In the private area and also in the professional environment. Especially in science, an above average increase can be observed.

In order to find more knowledge in this massive amount of data, the data will be placed and connected with other data, that was created in a comparable topic. Here is a simple example to show the general idea:

For example, when looking at Wikipedia articles that contains meaning-related topics. Without going in the content, new knowledge can be found by looking at which words occur in all articles. This makes it possible to find topics that play a role in each article.

With small amounts of data, this sounds easy and trivial. But with larger amounts of data, a search of this type through the data is only possible with the help of computers.

---

### General idea

For a efficient search process, all words will be encoded in integers. With this encoded data, simple integer comparisons are possible in shortest time.

With the help of some elementary set operations (the main important one: intersection), the set of words will be created, that appear in every source set.

---

### Structure of the evaluation
<img src="https://github.com/goto-vulture/Bioinformatics_Textmining/blob/Misc/Written_elaboration/Structure_of_the_evaluation_process_EN.png" width="50%" height="50%">

---

### Technical information

To make the implementation simple, the in and output will be created in a simple way. The name of the input files will be passed trough CLI parameter and the results will be represented as JSON file.

#### Used libraries

- *argparse*: Simple and powerful CLI parser. Here available on [GitHub](https://github.com/cofyc/argparse).
- *cJSON*: Lightweight (and beautiful) JSON parser. Also available on [GitHub](https://github.com/DaveGamble/cJSON).
- *TinyTest*: Simple and tiny test suite. You can find it here on [GitHub](https://github.com/joewalnes/tinytest).

---

### Future features

- Sentence offsets (The current implementation allows only a char offset)
- Smaller result file with following ideas:
    - Remove tabs
    - Use abbreviations for array names in the JSON file (e.g.: "Intersections" -> "Inters.")
    - Case insensitive comparison of the tokens
- A more user-friendly interface. Maybe with a GUI. But the priority in this project is low.

---
<br>

## DE

### Einleitung

In unserer heutigen Zeit werden mehr Informationen gesammelt als jemals zuvor. Dieser Trend erstreckt sich in nahezu jedem Bereich. Sowohl im privaten als auch im beruflichen Umfeld. Besonders im Bereich der Forschung ist eine überdurchschnittliche stetige Zunahme an Daten zu erkennen.

Um diesen Mengen an Daten mehr Informationen zu entlocken, werden diese im Zusammenhang mit anderen Daten gebracht, die in einer vergleichbaren Situation entstanden sind bzw. die sich auf ein vergleichbares Thema beziehen. Man kann sich das mithilfe eines einfachen Beispiels vor Augen führen wie dies aussehen kann:

Z.B. bei der Betrachtung von Wikipedia-Artikeln, die sich mit sinnverwandten Themen beschäftigen. Ohne auf den Inhalt direkt eingehen zu müssen, kann bereits neues Wissen gewonnen werden, wenn betrachtet wird, welche Wörter in allen Artikeln vorkommen. Dadurch können Themen identifiziert werden, die in jedem einzelnen Artikel eine Rolle spielen.

Bei kleinen Datenmengen klingt dies trivial. Bei größeren Mengen an Daten ist eine Durchsuchung dieser Art nur mittels Computern möglich.

---

### Grundidee

Um eine möglichst effiziente Durchsuchung der Daten vornehmen zu können, werden die verwendeten Begriffe als einfache Ganzzahlen codiert dargestellt. Dadurch sind Vergleichsoperationen in kürzester Zeit möglich.

Nach der Codierung wird mittels einiger elementarer Mengenoperationen (vornehmlich durch die Bildung der Schnittmenge) die Wortmengen gebildet, die in allen Datensätzen auftauchen.

---

### Struktur des Auswertungsprozesses
<img src="https://github.com/goto-vulture/Bioinformatics_Textmining/blob/Misc/Written_elaboration/Structure_of_the_evaluation_process_DE.png" width="50%" height="50%">

---

### Technische Details

Um die Implementierung auf das wesentliche zu fokussieren, werden die Ein- und Ausgabemöglichkeiten des Programms einfach ausfallen. So werden die Dateien mittels CLI-Parameter eingelesen; und die Ergebnisse werden in Form einer JSON-Datei dargestellt.

#### Verwendete Bibliotheken

- *argparse*: Einfacher CLI Parser. Verfügbar auf [GitHub](https://github.com/cofyc/argparse).
- *cJSON*: Wunderschöner und effizienter JSON Parser. Ebenfalls verfügbar auf [GitHub](https://github.com/DaveGamble/cJSON).
- *TinyTest*: Minimalistische Testsuite. Hier verfügbar: [GitHub](https://github.com/joewalnes/tinytest).

---

### Weitere Funktionen

- Satz Offsets (Die aktuelle Implementierung unterstuetzt nur zeichen-basierte Offsets)
- Kleinere Ergebnisdatei durch folgende Ansaetze:
    - Tabs entfernen
    - Abkuerzungen der Array Namen in der JSON Datei verwenden (e.g.: "Intersections" -> "Inters.")
    - Gross- und Kleinschreibung beim Vergleich der Tokens ignorieren
- Ein benutzerfreundlichere Bedienung; möglicherweise über eine GUI. Die Priorität ist allerdings gering, da diese auf der Funktionalität liegt.

---
