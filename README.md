# cbasic
CBASIC compiler for Linux x86.

##### Dependencies

In order to build this CBASIC compiler, you will need:
* ```bison``` v3.0+
* ```flex```

This CBASIC compiler uses ```NASM``` for it's backend, so please install ```NASM```.

The compiled programs are linked with the 32bit ```libc``` library. If you have a 64bit system you will have to install the 32bit libraries manually or by using your package manager.

##### The language

The language specification can be found here: http://www.cpm.z80.de/manuals/cbasic-m.pdf
