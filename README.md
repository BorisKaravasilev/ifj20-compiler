# IFJ 2020 Compiler

From a UNIX like terminal run these commands:

```bash
make
./lex < input.txt
```

Lexical analyzer for any finite automata.

Sample FA definition in [finite_automata_definition.c](finite_automata_definition.c) sample FA graph drawing in [finite_automata_graph.jpg](finite_automata_graph.jpg).

### TODO

* Refactor magic numbers to constants
* Support symbol ranges for transitions in rules
* Proper token generation
* Symbol table
* Keyword check
* EOL flag
* Finite automata graph
* Hex numbers

Check also TODO's in code.
