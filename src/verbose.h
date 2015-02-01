#ifndef VERBOSE_H_
#define VERBOSE_H_

//
// Verbose flags
//
extern unsigned int verbose_flags;

//
// Verbose flags definiton
//
#define VERBOSE_FLAG_PRINT_PARSED				0x1
#define VERBOSE_FLAG_PRINT_AST					0x2
#define VERBOSE_FLAG_PRINT_SYMBOLS				0x4
#define VERBOSE_FLAG_PRINT_FINAL				0x8

#define VERBOSE_FLAG_MAX						0xF

//
// Verbose macros
//
#define VERBOSE_PRINT_PARSED					(verbose_flags & VERBOSE_FLAG_PRINT_PARSED)
#define VERBOSE_PRINT_AST						(verbose_flags & VERBOSE_FLAG_PRINT_AST)
#define VERBOSE_PRINT_SYMBOLS					(verbose_flags & VERBOSE_FLAG_PRINT_SYMBOLS)
#define VERBOSE_PRINT_FINAL						(verbose_flags & VERBOSE_FLAG_PRINT_FINAL)

#endif
