# thiCcASSembler

## Notes
* Machine word size is 24 bits.
* There are four types of lines: instruction, directive (`.data`, `.string`, etc...), comment, empty line.

## Assembler Steps
1. Pre-assembly
   * Expand macros
   * No need for error handling - assume all macros are defined properly
2. First pass
   * Create a table of all symbols
   * Create code and data images skeletons
   * Ensure each symbol exists only once
3. Second pass
   * Build machine code

## Requirements
* Get source code files as command line arguments. Names are passed without suffix, but assembler expects `.as` suffix and concatenates accordingly. For example, if command line arguments are `x y`, the assembler will process the files `x.as, y.as`.
* Process each file separately, and **for each** produce the following:
  * `.am` file, containing source code after pre-assembly stage (unpacked). If no macros are found in `.as` file, the `.am` file will be identical to it.
  * `.ob` file, containing machine code.
  * `.ext` file, containing addresses of external symbols (don't create if no `.extern` symbols found).
  * `.ent` file, containing info of entry symbols (don't create if no `.entry` symbols found).
* Maintain code image array - source code instructions list.
* Maintain Instruction-Counter (IC) - point to next available space in code image array. Initial value is 100.
* Maintain data image array - source code data (`.string` and `.data` items) list.
* Maintain Data-Counter (DC) - point to next available space in data image array. Initial value is 0.
* Maintain symbol table - encountered tags + metadata.
* Ignore comments and empty lines.

## Algorithms

### Main
```python
get arguments
for file_name in arguments:
    file_name += '.as'
    pre_assembly(file_name)
    first_pass(file_name)
    second_pass(file_name)
```

### Pre-assembly
```python
is_macro = False
for line in source_code:
    if first field is macro found in macro table:
        remove macro line
        add macro contents from macro table
        continue
    else if first field is "macro":  # macro definition start
        is_macro = True
        add macro name to macro table
        continue
    if is_macro:
        if line is "endm":
            delete line
            is_macro = False
        else:
            add line to macro table content
            delete line
save final result
```

### First pass

```python
IC = 100, DC = 0
for line in source_code:
    has_tag = False
    if first field is tag:
        has_tag = True
    if line is .data or .string directive:
        if has_tag:
            if tag illegal or already in symbol table, print error
            add tag to symbol table with attribute "data"
        determine data type
        encode in data array
        DC += len(data)
    else if line is .entry:
        continue
    else if line is .extern:
        if tag illegal or already in symbol table **without** "external" attribute, print error
        add operand symbol to symbol table with values 0 and attribute "external"
    else:  # instruction line
        if has_tag:
            if tag illegal or already in symbol table, print error
            add tag to symbol table with attribute "code"
        if instruction not in action_table, print error
        determine instruction operand structure
        L = instruction word count
        build binary code of every word
        save instruction metadata, save IC and L values of instruction
        IC += L
if errors were found, stop
ICF = IC, DCF = DC
for symbol in symbol table:
    if symbol attribute is not data:
        continue
    recalculate symbol base and offset
    update symbol table accordingly
```
### Second pass

```python
for line in source_code:
    if first field is tag, skip it
    if line is .data or .string or .extern directive:
        continue
    else if line is .entry directive:
        if operand symbol not in symbol table, print error
        add "entry" attribute to symbol in symbol table
    complete binary encoding for operands
    if symbol in operand:
        if symbol not in symbol_table, print error
        if symbol has "external" attribute:
            add address of word to list of external symbols words
            write in ext file address of data words  # offset address will always follow base address
if errors were found, stop
build output files
```