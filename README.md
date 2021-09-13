# IFJ 2020 Compiler
We created this compiler of the Go language as our semester project for the ["Formal Languages and Compilers"](https://www.fit.vut.cz/study/course/13981/.en) course which was the biggest and most difficult coures project during our bachelor's degree. We are proud to say that we received almost full points for it because it is a course that many people fail because of this project.

### How to run

From a UNIX like terminal run these commands:

```bash
make
./ifj20-compiler < input.txt
```

Lexical analyzer for any finite automata.

Sample FA definition in [finite_automata.c](scanner/finite_automata.c) sample FA graph drawing in [finite_automata_graph.jpg](scanner/finite_automata_graph.jpg).

### CLion Setup - current cmake project

Run / Debug configuration should be set to - "CMake Application":
Configuration should look like this:
- Target: <ins>ifj20_compiler</ins> (should be set automatically or should exist in dropdown menu)
- Executable: <ins>ifj20_compiler</ins> (should be set automatically or should exist in dropdown menu)
- Program arguments: <ins>input_files/input_1.txt</ins> or any other file in that folder
- Working directory: <ins>*C:\full path to your project directory*\ifj20-compiler</ins> or any other file in that folder
- Else remains blank / default
- File -> "Reload CMake Project"

#### Input file

For running in CLion change "Program arguments" setting in "Edit configurations" to <ins>input_files/input_name.txt</ins>
