# REVIEW 2 - TESTING AND VALIDATION

## Team 10 - Mini Compiler
### BCSE307L - Compiler Design

---

## 1. Objective

The testing phase verifies the correctness of the mini compiler for:

- Variable declaration and assignment
- Arithmetic expressions
- If statements
- If-else statements
- While loops
- Nested control-flow statements
- Boolean AND (&&)
- Boolean OR (||)
- Short-circuit evaluation
- Three Address Code generation
- Backpatching
- Syntax error detection
- Semantic error detection
- Invalid token detection
- Line and column based error reporting

---

## 2. Test Environment

Language: C

Compiler: GCC

Execution Environment: GitHub Codespaces / Linux

Compilation Command:

gcc src/*.c -o mini_compiler

Execution Command:

./mini_compiler <test_file>

---

## 3. Test Case Summary

| Test | Feature | Expected Result | Actual Result | Status |
|------|---------|-----------------|---------------|--------|
| 1 | Assignment and Declaration | Successful compilation and TAC | Compilation successful | PASS |
| 2 | If-Else | Conditional TAC generation | Compilation successful | PASS |
| 3 | While Loop | Loop TAC and back edge | Compilation successful | PASS |
| 4 | Nested If-Else inside While | Nested control-flow TAC | Compilation successful | PASS |
| 5 | Undeclared Variable | Semantic error | Error reported with location | PASS |
| 6 | Boolean AND (&&) | Short-circuit and backpatching | Correct TAC generated | PASS |
| 7 | Boolean OR (||) | Short-circuit and backpatching | Correct TAC generated | PASS |
| 8 | Missing Semicolon | Syntax error | Error reported with location | PASS |
| 9 | Invalid Token | Lexer/parser error | Error reported with location | PASS |

---

## 4. Functional Testing

### Test 1: Assignment

Input:

int a,b;
a = 5;
b = 10;

Result:

0: a = 5
1: b = 10

Status: PASS

---

### Test 2: If-Else

The compiler successfully generates conditional jumps and separate TAC for both branches.

Status: PASS

---

### Test 3: While Loop

The compiler generates:

- Conditional jump
- Loop body
- Back edge to the loop condition
- Exit jump

Status: PASS

---

### Test 4: Nested Control Flow

A while loop containing an if-else statement was successfully compiled.

This verifies that nested control-flow constructs are handled correctly.

Status: PASS

---

## 5. Boolean Expression Testing

### Test 5: Boolean AND

Expression:

a < b && b > 5

The generated TAC contains separate conditional jumps for both conditions.

This verifies short-circuit evaluation and backpatching.

Status: PASS

---

### Test 6: Boolean OR

Expression:

a > b || b > 5

The generated TAC allows control to reach the true branch when either condition succeeds.

This verifies OR short-circuit evaluation and backpatching.

Status: PASS

---

## 6. Error Handling Testing

### Undeclared Variable

Input:

int a;
b = 10;

Output:

Syntax/Semantic Error at line 2, column 1: Undeclared variable

Status: PASS

---

### Missing Semicolon

Input contains an assignment without a terminating semicolon.

Output:

Syntax/Semantic Error at line 4, column 1: Unexpected token

Status: PASS

---

### Invalid Token

Input:

a = 10 @ 5;

Output:

Syntax/Semantic Error at line 3, column 8: Unexpected token

Status: PASS

---

## 7. Testing Result

Total test cases: 9

Passed: 9

Failed: 0

Pass percentage: 100%

The test suite verifies the major supported language constructs and validates successful TAC generation as well as syntax and semantic error handling.

---

## 8. Conclusion

Testing confirms that the current mini compiler successfully handles supported assignments, arithmetic expressions, conditional statements, loops, nested control flow, Boolean expressions, short-circuit evaluation, backpatching and Three Address Code generation.

Error cases are also detected with line and column information, providing useful diagnostic output to the user.
