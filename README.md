# cbasic
CBASIC compiler for Linux x86.

##### Dependencies

In order to build this CBASIC compiler, you will need:
* ```bison``` v3.0+
* ```flex```

This CBASIC compiler uses ```NASM``` for it's backend, so please install ```NASM```.

The compiled programs are linked with the 32bit ```libc``` library. If you have a 64bit system you will have to install the 32bit libraries manually or by using your package manager.

##### The language

The full language specification can be found here: http://www.cpm.z80.de/manuals/cbasic-m.pdf

Currently supported syntax and features:
* 32bit integer (```identifier%```) and floating point (```identifier```) types and arithmetic
* 255 byte strings (```identifier$```) with concatenation and comparisons
* variable declarations (```LET identifier = expression```)
* if-then block (```IF condition THEN [...] ENDIF```)
* if-then-else block (```IF condition THEN [...] ELSE [...] ENDIF```)
* loop statement (```WHILE condtion [...] WEND```)
* print statement (```PRINT expr [, expr ...]```)

##### Debug tools and sample usages

To compile the sample program ```fibo.bas```:
```
cbasic -o fibo samples/fibo.bas
```

To compile and print the AST and the program after semantic analysis:
```
cbasic -V 10 -o fibo samples/fibo.bas
```

To compile and print the program after semantic analysis and the intermediate language program:
```
cbasic -V 24 -o fibo samples/fibo.bas
```

You can inspect the output assembly code (which is the input of NASM) in the ```fibo.asm``` file.
