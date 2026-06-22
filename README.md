# JVM-From-Scratch
A ground-up implementation of the Java Virtual Machine, exploring class loading, bytecode interpretation, and execution semantics.

## Build

Compile the project with:

```
gcc src/main.c src/viewer/printer.c src/viewer/classviewer.c src/viewer/attribute_decoder.c src/class_loader/loading/classparser.c src/class_loader/loading/classloader.c src/class_loader/linking/verifier.c src/class_loader/linking/preparation.c src/class_loader/linking/resolution.c src/class_loader/linking/linking.c src/class_loader/initialization/initialization.c src/runtime_data/method_area/methodarea.c src/runtime_data/thread_data/thread_data.c src/utils/stack.c -I src -I src/class_loader/loading -I src/class_loader/linking -I src/class_loader/initialization -I src/runtime_data/method_area -I src/runtime_data/thread_data -I src/utils -I src/viewer -std=c99 -o test
```

## Run

Execute the program by passing a `.class` file to either be executed or viewed:

```
./test run Belote       // .class file available in Examples/

./test view Belote      // .class file available in Examples/
```

The path to the `.class` file is relative to the directory where the command is executed.