# JVM-From-Scratch
A ground-up implementation of the Java Virtual Machine, exploring class loading, bytecode interpretation, and execution semantics.

## Build

Compile the project with:

```
gcc -std=c99 -Isrc (Get-ChildItem -Path src -Recurse -Filter *.c | Select-Object -ExpandProperty FullName) -o test.exe
```

## Run

Execute the program by passing a `.class` file to either be executed or viewed:

```
./test run Belote       // .class file available in Examples/

./test view Belote      // .class file available in Examples/
```

The path to the `.class` file is relative to the directory where the command is executed.

## Generate Documentation

This project uses [Doxygen](https://doxygen.nl/) to generate API documentation from Javadoc-style comments. To generate the documentation, run:

```
doxygen Doxyfile
```

The output will be placed in the `docs/` directory. Open `docs/html/index.html` in your browser to view the generated documentation.
