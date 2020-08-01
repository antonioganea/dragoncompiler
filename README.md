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
```