---
name: java-compiler-testing-and-auto-code-generation
description: generating testing code for the Java compiler and generating some boilerplate code for Java compiler
---

# TokenType Representation

## Instruction

1. Modify `../src/Token.cpp`, so that the `const char *toString(TokenType tokenType)` function can
   return the string representation for all the token types. All the token type representation has
   the format of `TokenType::<tokentype enum name>`, using the definition in `enum TokenType` in `../src/Token.h`
to replace the `<tokentype enum name>` placeholder.

# General Test Code Generation Requirements

## Role & Goals

You are an expert C++ QA engineer specializing in compiler design and lexical analysis. Your goal is to generate thorough, production-grade unit tests for a C++ Java Compiler using `GoogleTest`.

## Requirements

- The files under the tests/ directory are the only files that you can modify when generating test codes.
- DO NOT generate test code just for pass the testing, you should obey the testing rules specified by 
this skill and validate all branch cases rigorously.
- Your test code should NOT only cover the test cases that shown in the examples. If the lexing grammar
or parsing grammar is given, you should consider different cases as much as possible

# Java Number Lexer Test Generator Skill

## Rules & Lexing Grammar Coverage
The tests must rigorously validate all branch cases matching the official Java Number Literal specification:

1. **Decimal Integer Literals**: `0`, `123`, `123L`, `0L`
2. **Hex Integer Literals**: `0x0`, `0x1A3F`, `0XabcL`
3. **Octal Integer Literals**: `0777`, `00`, `0123L`
4. **Decimal Floating Point Literals**:
    - `123.456`, `.456`, `123.`, `123.e10f`, `.456e-10D`, `0123.456`
    - Edge Case: Numbers starting with `0` like `0123.456` **must** be classified as Decimal Floating Point, NOT Octal.
5. **Hexadecimal Floating Point Literals**:
    - `0x1.0p-5D`, `0XFFp10`, `0x.A1p2f`, `0x10.P+3`
6. **Invalid / Boundary Literals**:
    - `0999` (Invalid octal digit)
    - `0xG` (Invalid hex digit)
    - `123abc` (Trailing garbage)
    - `.` (Standalone dot)

## Test Structure Requirements

1. **Framework**: Output GoogleTest (`TEST` macros) or parameterized tests (`TEST_P`).
2. **Assertion Rules**:
    - Validate both the expected `TokenType` enum and the extracted token text length/string.
    - Test edge-case lexing where number literals are followed immediately by non-numeric tokens (e.g., `123+456`).
3. **Isolation**: Test cases should be grouped into separate test suites per literal category (`DecimalIntTests`, `HexFloatTests`, etc.).

# Java Char and String Literal Test Generator Skill

## Lexing Grammar

The following describes the lexing grammar for character literal and string literal in Java, according to https://docs.oracle.com/javase/specs/jls/se6/html/lexical.html#100850

### Character Literals

```
CharacterLiteral:
' SingleCharacter '
' EscapeSequence '

SingleCharacter:
InputCharacter but not ' or \
```

### String Literals

```
StringLiteral:
        " StringCharactersopt "

StringCharacters:
        StringCharacter
        StringCharacters StringCharacter

StringCharacter:
        InputCharacter but not " or \
        EscapeSequence
```

### Escape Characters

```
EscapeSequence:
        \ b                     /* \u0008: backspace BS                   */
        \ t                     /* \u0009: horizontal tab HT              */
        \ n                     /* \u000a: linefeed LF                    */
        \ f                     /* \u000c: form feed FF           */
        \ r                     /* \u000d: carriage return CR             */
        \ "                     /* \u0022: double quote "             */
        \ '                     /* \u0027: single quote '          */
        \ \                     /* \u005c: backslash \                     */
        OctalEscape               /* \u0000 to \u00ff: from octal value   */
        UnicodeEscape

OctalEscape:
        \ OctalDigit
        \ OctalDigit OctalDigit
        \ ZeroToThree OctalDigit OctalDigit

OctalDigit: one of
        0 1 2 3 4 5 6 7

ZeroToThree: one of
        0 1 2 3
        
UnicodeEscape:
        \ UnicodeMarker HexDigit HexDigit HexDigit HexDigit

UnicodeMarker:
        u
        UnicodeMarker u

HexDigit: one of
        0 1 2 3 4 5 6 7 8 9 a b c d e f A B C D E F
```