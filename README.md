# dragoncompiler

```

# DRAGON C CODE SAMPLE

# allocates 5 bytes in ram under label "a".
vec a : 5; # "a" is now a global symbol

var b = 9; # allocates 1 byte for variable 'b' with value 9

rom mynumbers : 10, 20, 32, 10, 6, 4, 2; # stores this in rom

func simple() { # func_simple:
  var b;     # SP -= 1;
  var c;     # SP -= 1;
  b = 68;    # LBI RX, 68; SB RX, +2(SP)
  c = 4;     # LBI RX, 4;  SB RX, +1(SP)
  b = b + c;

  return 7;
  # LBI RF, 7

  # release stack # SP += 2
  # JRT
}

func main(){
  var a, d;
  a = 10;
  d = simple();

  CALL simple;
  LB RF, +1(SP)

  a = a + d;
  PRINT(a);

  forever{
    a++;
    if ( a == 150 ){
        break
    }
    DRAW(0, 0, 14, 14, 3);
    HALT();
  }
}
```

```
Tokenized output sample:

"vec" "a" ":" "5" ";"

"func" "simple" "(" ")" "{"
"var" "b" ";"
"var" "c" ";"
"b" "=" "68" ";"
"c" "=" "4" ";"
"return" "7" ";"
 
```

```
possible token formats

words from letters :
aaaa func CALL black Blsadl main break slos

numbers:
101 23 59 5 2 102033 539

these characters in these combinations:
++
--
==
>=
<=
!=
these characters :
(
)
{    
}
[
]
+
-
=
,
;
:
<
>
```
## Grammar Rules ( WIP )
```
BREAK_STATEMENT :
  KEYWORD_BREAK SEMICOLON

VAR_DECLARATION :
	KEYWORD_VAR GENERAL_IDENTIFIER SEMICOLON

VEC_DECLARATION :
	KEYWORD_VEC GENERAL_IDENTIFIER COLON NUMERAL SEMICOLON

NUMERAL_LIST :
  NUMERAL COMMA NUMERAL
  NUMERAL_LIST COMMA NUMERAL

ROM_DECLARATION :
  KEYWORD_ROM GENERAL_IDENTIFIER COLON NUMERAL SEMICOLON
	KEYWORD_ROM GENERAL_IDENTIFIER COLON NUMERAL_LIST SEMICOLON

      FUNC_CALL :
        GENERAL_IDENTIFIER PARATHESIS_OPEN NUMERAL_LIST PARANTHESIS_CLOSED SEMICOLON






              PARAM_LIST :
                GENERAL_IDENTIFIER COMMA GENERAL_IDENTIFIER
                PARAM_LIST COMMA GENERAL_IDENTIFIER

              FUNC_PROTOTYPE :
                KEYWORD_FUNC GENERAL_IDENTIFIER PARATHESIS_OPEN PARANTHESIS_CLOSED
                KEYWORD_FUNC GENERAL_IDENTIFIER PARATHESIS_OPEN GENERAL_IDENTIFIER PARANTHESIS_CLOSED
                KEYWORD_FUNC GENERAL_IDENTIFIER PARATHESIS_OPEN PARAM_LIST PARANTHESIS_CLOSED

              FUNC_CALL :
                FUNC_PROTOTYPE SEMICOLON

BIN_OP :
  PLUS
  MINUS
  XOR
  MUL

EXPR :
  GENERAL_IDENTIFIER
  NUMERAL
  EXPR BIN_OP EXPR
  PARATHESIS_OPEN EXPR PARANTHESIS_CLOSED

VAR_ASSIGNMENT :
	GENERAL_IDENTIFIER EQUALS EXPR SEMICOLON

RETURN_STATEMENT :
	KEYWORD_RETURN EXPR SEMICOLON

LOGICAL_COMP :
  LOP_EQUALS
  LOP_NOT_EQUALS
  LOP_GREATER_THAN
  LOP_GREATER_EQUAL
  LOP_LESS_THAN
  LOP_LESS_EQUAL

LOGICAL_OP :
  KEYWORD_AND
  KEYWORD_OR

CONDITION :
  PARATHESIS_OPEN EXPR LOGICAL_COMP EXPR PARANTHESIS_CLOSED
  PARATHESIS_OPEN CONDITION LOGICAL_OP CONDITION PARANTHESIS_CLOSED
  PARATHESIS_OPEN EXCLAMATION_MARK CONDITION PARANTHESIS_CLOSED

STATEMENT :
	VAR_DECLARATION
	VEC_DECLARATION
	VAR_ASSIGNMENT
	RETURN_STATEMENT
  BREAK_STATEMENT
  FUNC_CALL
  IF
  FOREVER

STATEMENTS :
  STATEMENT
  STATEMENTS STATEMENT

BLOCK :
	CURLY_BRACKET_OPEN STATEMENTS CURLY_BRACKET_CLOSED
  CURLY_BRACKET_OPEN CURLY_BRACKET_CLOSED

IF :
  KEYWORD_IF CONDITION BLOCK

FOREVER :
  KEYWORD_FOREVER BLOCK

FUNC_DECLARATION :
	FUNC_PROTOTYPE BLOCK
  FUNC_PROTOTYPE BLOCK
  FUNC_PROTOTYPE BLOCK

// CONDITION simplifcation idea ( remove useless paranthesis ):
// the problem that paranthesis fix is that you wouldn't normally
// parse :
// EXPR LOGICAL_COMP EXPR
// for example case:
// 1 == a + 2
// As it will parse 1 == a into a CONDITION
// so you are left with
// CONDITION PLUS NUMERAL

// Potential fix : instead of paranthesis, this is usable too:
// evaluate EXPR first, program wide


```
