# JVM-From-Scratch
A ground-up implementation of the Java Virtual Machine, exploring class loading, bytecode interpretation, and execution semantics.

## Build

Compile the project with:

```
gcc src/main.c src/class_loader/loading/classparse.c -I src/class_loader/loading -std=c99 -o test
```

Alternatively, use the makefile or build.bat on Windows:
```
mingw32-make -f Makefile.win all
mingw32-make -f Makefile.win clean
mingw32-make -f Makefile.win debug
```
```
build.bat all
build.bat clean
build.bat debug
```

## Run

Execute the program by passing a `.class` file:

```
./test Examples/Belote.class
```

The path to the `.class` file is relative to the directory where the command is executed.