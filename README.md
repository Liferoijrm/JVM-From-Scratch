# JVM-From-Scratch
A ground-up implementation of the Java Virtual Machine, exploring class loading, bytecode interpretation, and execution semantics.

## Build

Compile the project with:

```
gcc src/main.c src/class_loader/loading/classparser.c src/viewer/classviewer.c src/viewer/attribute_decoder.c src/viewer/printer.c src/class_loader/loading/classloader.c src/runtime_data/method_area/methodarea.c src/utils/stack.c -I src/class_loader/loading -I src/viewer -I src/utils -I src/runtime_data/method_area -std=c99 -o test
```

## Run

Execute the program by passing a `.class` file:

```
./test Examples/Belote.class
```

The path to the `.class` file is relative to the directory where the command is executed.