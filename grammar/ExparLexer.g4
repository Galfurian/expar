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
LESS_THAN           : '<';
GREATER_THAN        : '>';
EQUAL               : '=';
AND                 : 'and';
COLON               : ':';
SEMICOLON           : ';';
EXCLAMATION_MARK    : '!';
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
ARROW               : '->';
PERCENT             : '%';
LESS_THEN_EQUAL     : '<=';
GREATER_THEN_EQUAL : '>=';
CARET               : '^';
TILDE               : '~';

// ----------------------------------------------------------------------------
// NUMERICAL VALUES
// ----------------------------------------------------------------------------
fragment DIGIT      : [0-9];
fragment HEXDIGIT   : '0x' ('0'..'9' | 'a'..'f' | 'A'..'F')+;
fragment OCTALDIGIT : '0' '0'..'7'+;
fragment EXP        : ('E' | 'e') ('+' | '-')? INT ;
fragment INT        : DIGIT+ [Ll]? LETTER?;
HEX                 : '0' ('x'|'X') HEXDIGIT+ [Ll]? ;
fragment FLOAT      : DIGIT+ '.' DIGIT* EXP? [Ll]? LETTER?
                    | DIGIT+ EXP? [Ll]? LETTER?
                    | '.' DIGIT+ EXP? [Ll]? LETTER?;
PERCENTAGE          : FLOAT '%'  ;
COMPLEX             : INT 'i' | FLOAT 'i' ;
NUMBER              : INT | FLOAT;

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
          | DOT)
          ( LETTER
          | EXCLAMATION_MARK
          | AT_SIGN
          | POUND_SIGN
          | DIGIT
          | UNDERSCORE
          | COLON
          | DOT
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
