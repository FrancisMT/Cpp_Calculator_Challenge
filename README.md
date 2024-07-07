# Calculator challenge
A C++ calculator that can receive different instructions and react accordingly.

## Implementation Details
One of the backbones of this implementation involves converting arithmetic expressions into Abstract Syntax Trees using the Shunting Yard algorithm for efficient parsing and evaluation.

Managing the current state of the calculator is achieved by:
* Maintaining Operation Order: to keep track of the sequence in which operations are performed;
* Storing Evaluated Results: to hold the results of evaluated expressions for future reference;
* Tracking Dependencies: to manage dependencies between operands and to ensure accurate and efficient expression evaluation;

## Tools
* C++20
* CMake
* GTest

## Build
```
❯ mkdir build && cd build
❯ cmake ..
❯ cmake --build .
```

## Usage
```
❯ ./Calculator-Challenge
Input Arithmetic expression to evaluate: a=2+3
a = 5

Input Arithmetic expression to evaluate: b=e-2

Input Arithmetic expression to evaluate: c=1+2
c = 3

Input Arithmetic expression to evaluate: d=e/3

Input Arithmetic expression to evaluate: e=a+c
e = 8, b = 6, d = 2

Input Arithmetic expression to evaluate: f=3+4
f = 7

Input Arithmetic expression to evaluate: undo 2
delete f, delete e

Input Arithmetic expression to evaluate: e=2+2
e = 4, b = 2, d = 1

Input Arithmetic expression to evaluate: f=g*7

Input Arithmetic expression to evaluate: result
return e = 4

Input Arithmetic expression to evaluate: g=3*2
g = 6, f = 42
```

## Documentation
This project is configured to generate documentation using Doxygen.

To access it, open the _build/doxygen/html/index.html_ file

## Coverage
CMake already takes care of automatically integrating Google test into the project, so there is no need to manually install and configure it.

### Run tests
```
❯ ctest
      Start  1: EvaluatorUnitTest.evaluatorThrowsIfInputIsNull
 1/12 Test  #1: EvaluatorUnitTest.evaluatorThrowsIfInputIsNull .....................................   Passed    0.00 sec
      Start  2: EvaluatorUnitTest.evaluatorOutputsCorrectResult
 2/12 Test  #2: EvaluatorUnitTest.evaluatorOutputsCorrectResult ....................................   Passed    0.00 sec
      Start  3: ParserUnitTest.parserThrowsWhenInputIsEmpty
 3/12 Test  #3: ParserUnitTest.parserThrowsWhenInputIsEmpty ........................................   Passed    0.00 sec
      Start  4: ParserUnitTest.parserThrowsWhenNegativeNumbersAreProvided
 4/12 Test  #4: ParserUnitTest.parserThrowsWhenNegativeNumbersAreProvided ..........................   Passed    0.00 sec
      Start  5: ParserUnitTest.parserThrowsWhenLiteralsAreTooLarge
 5/12 Test  #5: ParserUnitTest.parserThrowsWhenLiteralsAreTooLarge .................................   Passed    0.00 sec
      Start  6: ParserUnitTest.parserThrowsWhenParenthesesAreNotPaired
 6/12 Test  #6: ParserUnitTest.parserThrowsWhenParenthesesAreNotPaired .............................   Passed    0.00 sec
      Start  7: ParserUnitTest.parserThrowsWhenParenthesesAreIncorrectlyUsed
 7/12 Test  #7: ParserUnitTest.parserThrowsWhenParenthesesAreIncorrectlyUsed .......................   Passed    0.00 sec
      Start  8: ParserUnitTest.parserThrowsWhenOperatorsAreMissing
 8/12 Test  #8: ParserUnitTest.parserThrowsWhenOperatorsAreMissing .................................   Passed    0.00 sec
      Start  9: ParserUnitTest.parserThrowsWhenOperatorsAreIncorrectlyUsed
 9/12 Test  #9: ParserUnitTest.parserThrowsWhenOperatorsAreIncorrectlyUsed .........................   Passed    0.00 sec
      Start 10: ParserUnitTest.parserDoesNotThrowWhenValidInputIsProvided
10/12 Test #10: ParserUnitTest.parserDoesNotThrowWhenValidInputIsProvided ..........................   Passed    0.00 sec
      Start 11: ParserUnitTest.parserGenerasteAST
11/12 Test #11: ParserUnitTest.parserGenerasteAST ..................................................   Passed    0.00 sec
      Start 12: ArithmeticExpressionsHandlingTest.outputsTheCorrectResultOfArithmeticExpressions
12/12 Test #12: ArithmeticExpressionsHandlingTest.outputsTheCorrectResultOfArithmeticExpressions ...   Passed    0.00 sec

100% tests passed, 0 tests failed out of 12

Total Test time (real) =   0.05 sec
```
