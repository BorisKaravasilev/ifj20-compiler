# IFJ 2020 Compiler

From a UNIX like terminal run these commands:

```bash
make
./ifj20-compiler < input.txt
```

Lexical analyzer for any finite automata.

Sample FA definition in [finite_automata.c](scanner/finite_automata.c) sample FA graph drawing in [finite_automata_graph.jpg](scanner/finite_automata_graph.jpg).

### TODO

Check TODO's in code and create issues on Github from them.

### CLion Setup

- Install *Makefile* plugin from settings.
- Install [cygwin](https://www.cygwin.com/) C compiler.
- Go to "Settings" -> "Build, Execution, Development" -> "Build Tools" -> "Make" and set the path to "C:\cygwin64\bin\make.exe" or where your *make* is located. I checked the option "Use Cygwin".
- Add path to your "make.exe" to system variable PATH in Windows (For me it is "C:\cygwin64\bin\make.exe").
- In CLion in the upper right corner next to the green triangle, open the dropdown menu and go into "Edit configurations...", add a new configuration of type "Makefile" and type "run" in "Targets" field, select path to "Makefile" -> "Apply" -> "Ok".
- Then just using Cmd+f5 shortcut build and run the code.

#### Input file

For running in CLion in file "ifj20-compiler.c" change "IS_INPUT_FROM_FILE" to true.
Then the input will come from file and not *stdin*.
Edit or add input file in folder *input_files*.
Keep it clean, cheers!

#### Adding folders

When you add new folder with ".c" files, add the new folder to Makefile.
