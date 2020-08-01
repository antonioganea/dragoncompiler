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
BLOCK :
	CURLY_BRACKET_OPEN
	STATEMENT^
	CURLY_BRACKET_CLOSED

STATEMENT :
	VAR_DECLARATION
	ROM_DECLARATION
	VEC_DECLARATION
	VAR_DECLARATION_AND_INIT
	VAR_ASIGNMENT
	RETURN_STATEMENT

VAR_DECLARATION :
	KEYWORD_VAR GENERAL_IDENTIFIER SEMICOLON

ROM_DECLARATION :
	KEYWORD_ROM GENERAL_IDENTIFIER COLON (NUMERAL COMMA)* SEMICOLON

VEC_DECLARATION :
	KEYWORD_VEC GENERAL_IDENTIFIER COLON NUMERAL SEMICOLON

FUNC_DECLARATION :
	KEYWORD_FUNC GENERAL_IDENTIFIER PARATHESIS_OPEN PARANTHESIS_CLOSED  BLOCK

VAR_DECLARATION_AND_INIT :
	KEYWORD_VAR GENERAL_IDENTIFIER EQUALS NUMERAL SEMICOLON
	KEYWORD_VAR GENERAL_IDENTIFIER EQUALS GENERAL_IDENTIFIER SEMICOLON

VAR_ASIGNMENT :
	GENERAL_IDENTIFIER EQUALS GENERAL_IDENTIFIER SEMICOLON
	GENERAL_IDENTIFIER EQUALS NUMERAL SEMICOLON

RETURN_STATEMENT :
	KEYWORD_RETURN GENERAL_IDENTIFIER SEMICOLON
	KEYWORD_RETURN EQUALS SEMICOLON
```
