# Math Parser

A mathematical expression parser and evaluator powered by Pratt's top-down operator precedence.

## Features

* **C99 compliant** with **no dependencies**.
* **Symbol table** for pre-loaded constants (`pi` and `e`) and **user-defined variables**.
* Supports standard **math functions** (`sin`, `sqrt`, `log`, etc.).
* Usable as a one-shot **CLI tool** or an interactive **REPL**.
* Compiles to a `.a` file for easy integration into other C projects.

## Building

The project uses a `Makefile` and requires `gcc` to compile. To generate the static library and the executable binary, there are two options.

* **Release mode**: Use `make` to build with maximum optimization.
* **Debug mode**: Use `make DEBUG=1` to build with debug symbols and no optimizations.

This will output `lib/mode/libmathparser.a` and `bin/mode/parser`, where `mode` is either `release` or `debug`. Object files are placed in `build/mode/`.

## Usage

### CLI

Pass an expression as a quoted string to evaluate it immediately.

```bash
./parser "ln(e^(-1/3))"
```

### REPL

Run without arguments to enter the REPL. This allows for AST visualization and variable assignment. Type `.help` to see all available commands.

```bash
> x = y = 12
12.000000
> sqrt(x + y + 1)
5.000000
```

### Library

Include the necessary headers (`parser.h` and `environment.h` from `include/`) and initialize both the parser and the symbol table.

```c
Parser parser = parser_init();
SymbolTable symbol_table = symbol_table_init();
```

Add variables to the symbol table, update their value and retrieve them.

```c
symbol_table_set(&symbol_table, "my_var", 6, 9.5);  // Create new
symbol_table_set(&symbol_table, "my_var", 6, 30.8); // Update
symbol_table_get(&symbol_table, "my_var", 6);       // Fetch
```

Parse an expression and evaluate the resulting AST.

```c
Node *root = parser_parse(&parser, "sin(pi*3-0.2)");
double result = env_evaluate(root, &symbol_table);
```

## License

This project is available under the MIT License.