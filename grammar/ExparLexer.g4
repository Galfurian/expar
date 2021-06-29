// ----------------------------------------------------------------------------
// Author : Enrico Fraccaroli
// Date   : 04/02/2018
// Reference: references/eldo_ur.pdf
// ----------------------------------------------------------------------------
lexer grammar ExparLexer;

channels { COMMENTS }

// ----------------------------------------------------------------------------
// COMMENT
// ----------------------------------------------------------------------------
COMMENT : (SLASH SLASH) .*? NL -> channel(COMMENTS);

// ----------------------------------------------------------------------------
// EXPRESSIONS
// ----------------------------------------------------------------------------
EQUAL               : '=';
EXCLAMATION_MARK    : '!';
LESS_THAN           : '<';
GREATER_THAN        : '>';
LESS_THAN_EQUAL     : LESS_THAN EQUAL;
GREATER_THAN_EQUAL  : GREATER_THAN EQUAL;
LOGIC_EQUAL         : EQUAL EQUAL;
LOGIC_NOT_EQUAL     : EXCLAMATION_MARK EQUAL;
LOGIC_AND           : AMPERSAND AMPERSAND;
LOGIC_OR            : PIPE PIPE;
LOGIC_BITWISE_AND   : AMPERSAND;
LOGIC_BITWISE_OR    : PIPE;
LOGIC_XOR           : CARET CARET;
BITWISE_SHIFT_LEFT  : LESS_THAN LESS_THAN;
BITWISE_SHIFT_RIGHT : GREATER_THAN GREATER_THAN;
POWER_OPERATOR      : STAR STAR;
AND                 : 'and';
OR                  : 'or';
COLON               : ':';
SEMICOLON           : ';';
PLUS                : '+';
MINUS               : '-';
STAR                : '*';
OPEN_ROUND          : '(';
CLOSE_ROUND         : ')';
OPEN_SQUARE         : '[';
CLOSE_SQUARE        : ']';
OPEN_CURLY          : '{';
CLOSE_CURLY         : '}';
QUESTION_MARK       : '?';
COMMA               : ',';
DOLLAR              : '$';
AMPERSAND           : '&';
DOT                 : '.';
UNDERSCORE          : '_';
AT_SIGN             : '@';
POUND_SIGN          : '#';
BACKSLASH           : '\\';
SLASH               : '/';
APEX                : '\'';
QUOTES              : '"';
PIPE                : '|';
PERCENT             : '%';
CARET               : '^';
TILDE               : '~';
ARROW               : MINUS GREATER_THAN;

// ----------------------------------------------------------------------------
// NUMERICAL VALUES
// ----------------------------------------------------------------------------
fragment DIGIT      : [0-9];
fragment HEXDIGIT   : '0x' ('0'..'9' | 'a'..'f' | 'A'..'F')+;
fragment OCTALDIGIT : '0' '0'..'7'+;
fragment EXP        : ('E' | 'e') ('+' | '-')? INT ;
fragment INT        : DIGIT+ [Ll]? LETTER?;
fragment FLOAT      : DIGIT+ '.' DIGIT* EXP? [Ll]? LETTER?
                    | DIGIT+ EXP? [Ll]? LETTER?
                    | '.' DIGIT+ EXP? [Ll]? LETTER?;
fragment HEX        : '0' ('x'|'X') HEXDIGIT+ [Ll]? ;
PERCENTAGE          : FLOAT '%'  ;
COMPLEX             : INT 'i' | FLOAT 'i' ;
NUMBER              : INT | FLOAT | HEX;

// ----------------------------------------------------------------------------
// STRINGS
// ----------------------------------------------------------------------------
fragment LETTER : [a-zA-Z];
ID      : ( LETTER
          | EXCLAMATION_MARK
          | AT_SIGN
          | POUND_SIGN
          | DIGIT
          | UNDERSCORE
          | DOLLAR
          //| DOT
          )
          ( LETTER
          | EXCLAMATION_MARK
          | AT_SIGN
          | POUND_SIGN
          | DIGIT
          | UNDERSCORE
          //| COLON
          //| DOT
          | LESS_THAN
          | GREATER_THAN
          | BACKSLASH LESS_THAN
          | BACKSLASH GREATER_THAN
          | DOLLAR
          | PERCENT
          | ARROW )*;

// ----------------------------------------------------------------------------
// WHITESPACES and NEWLINES
// ----------------------------------------------------------------------------
NL  : '\r'?'\n';
WS  : [ \t]+ -> skip;
