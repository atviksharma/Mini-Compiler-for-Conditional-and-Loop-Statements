# Mini Compiler for Conditional and Loop Statements

## Team 10
**Course:** BCSE307L - Compiler Design

## Project Description

This project implements a syntax-directed mini compiler in C for conditional and loop statements.

The compiler performs lexical analysis, syntax analysis, semantic processing, backpatching, and Three Address Code (TAC) generation.

## Objectives

- Perform lexical analysis of the input program.
- Parse declarations, assignments, expressions, conditional statements, and loops.
- Maintain a symbol table for declared variables.
- Use semantic attributes during syntax-directed translation.
- Generate Three Address Code (TAC).
- Implement backpatching for conditional and loop statements.
- Detect syntax and semantic errors.

## Features

- Lexical analyzer
- Recursive descent parser
- Symbol table
- Arithmetic expressions
- Relational expressions
- Logical operators `&&` and `||`
- `if-else` statements
- `while` loops
- Semantic attributes
- Backpatching
- Three Address Code generation
- Undeclared variable detection

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
│   └── test_cases.txt
│
├── docs/
│   └── review1.md
│
└── README.md
