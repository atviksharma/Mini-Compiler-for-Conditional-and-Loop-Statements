# Review 1 Documentation

## Mini Compiler for Conditional and Loop Statements

**Course:** BCSE307L - Compiler Design  
**Team:** 10

---

## 1. Team Members and Responsibilities

| Member | Name | Responsibility |
|---|---|---|
| Member 1 | Palak Agarwal | Lexical Analysis |
| Member 2 | Shiksha Ajay Singh | Parser / Grammar |
| Member 3 | Patil Soham | Semantic Attributes |
| Member 4 | Harshvardhan Vashishtha | Backpatching + Three Address Code |
| Member 5 | Atvik | Testing + Documentation |

---

## 2. Project Objective

The objective of this project is to develop a mini compiler that accepts a small C-like source program and performs compilation of declarations, assignments, conditional statements, and loop statements.

The compiler reports syntax or semantic errors and generates Three Address Code (TAC) for successfully compiled programs.

---

## 3. Compiler Components

The project is organized into the following components:

### 3.1 Lexical Analysis

The lexical analyzer identifies tokens from the input source program, including:

- Keywords
- Identifiers
- Integer constants
- Operators
- Relational operators
- Delimiters

The lexical analysis functionality is implemented in:

- `src/lexer.c`
- `src/lexer.h`

### 3.2 Parser / Grammar

The parser processes the tokens according to the grammar supported by the mini compiler.

It handles constructs such as:

- Variable declarations
- Assignments
- `if-else` statements
- `while` loops
- Nested control-flow statements

The parser functionality is implemented in:

- `src/parser.c`
- `src/parser.h`

### 3.3 Semantic Analysis

Semantic checking is performed during parsing.

The compiler checks whether variables used in statements have been declared. For example:

```c
int a;
b = 10;