// ----------------------------------------------------------------------------
// Author : Enrico Fraccaroli
// Date   : 04/02/2018
// ----------------------------------------------------------------------------
parser grammar ExparParser;

options {
    tokenVocab = ExparLexer;
}

// ============================================================================
value
    : value_unary
    | value_function_call
    | value_scope
    | value_atom
    | value value_operator value;
value_unary
    : (PLUS | MINUS) value;
value_function_call
    : ID OPEN_ROUND (value COMMA?)+ CLOSE_ROUND;
value_scope
    : (OPEN_ROUND | OPEN_CURLY | APEX | OPEN_SQUARE) (value COMMA?)+ (CLOSE_ROUND | CLOSE_CURLY | APEX | CLOSE_SQUARE);
value_operator
    : PLUS
    | MINUS
    | STAR
    | SLASH
    | PIPE PIPE
    | AMPERSAND AMPERSAND
    | CARET CARET
    | EXCLAMATION_MARK
    | PIPE
    | AMPERSAND
    | LESS_THAN LESS_THAN
    | GREATER_THAN GREATER_THAN
    | EQUAL EQUAL
    | EXCLAMATION_MARK EQUAL
    | LESS_THAN
    | GREATER_THAN
    | LESS_THAN EQUAL
    | GREATER_THAN EQUAL
    | STAR STAR
    | CARET
    | PERCENT
    ;
value_atom
    : NUMBER
    | ID
    | PERCENTAGE
    | HEX;