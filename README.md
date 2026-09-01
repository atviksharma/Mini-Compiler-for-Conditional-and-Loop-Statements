# Mini Compiler for Conditional and Loop Statements

**Team 10**  
**BCSE307L - Compiler Design**

## Team Members

| Member | Name | Responsibility |
|---|---|---|
| Member 1 | Palak Agarwal | Lexical Analysis |
| Member 2 | Shiksha Ajay Singh | Parser / Grammar |
| Member 3 | Patil Soham | Semantic Attributes |
| Member 4 | Harshvardhan Vashishtha | Backpatching + TAC |
| Member 5 | Atvik | Testing + Documentation |

## Project Overview

This project implements a mini compiler for a C-like language supporting variable declarations, assignments, conditional statements, and loop statements.

The compiler performs lexical analysis, parsing, semantic checking, control-flow handling, and Three Address Code generation.

## Supported Constructs

- Variable declarations
- Assignment statements
- Arithmetic expressions
- Relational expressions
- `if` statements
- `if-else` statements
- `while` loops
- Nested conditional and loop statements
- Undeclared variable detection
- Three Address Code generation

## Project Structure

```text
Mini-Compiler-for-Conditional-and-Loop-Statements/
│
├── src/
│   ├── lexer.c
│   ├── lexer.h
│   ├── parser.c
│   ├── parser.h
│   └── main.c
│
├── tests/
│   ├── test_assignment.c
│   ├── test_if_else.c
│   ├── test_while.c
│   ├── test_nested.c
│   ├── test_error.c
│   └── test_cases.txt
│
├── docs/
│   └── review1.md
│
├── README.md
└── mini_compiler