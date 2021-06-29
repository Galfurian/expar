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
    : EQUAL
    
    | PLUS | MINUS | STAR | SLASH

    | LOGIC_AND | LOGIC_BITWISE_AND
    | LOGIC_OR  | LOGIC_BITWISE_OR

    | LOGIC_EQUAL | LOGIC_NOT_EQUAL

    | LOGIC_XOR

    | LESS_THAN    | LESS_THAN_EQUAL
    | GREATER_THAN | GREATER_THAN_EQUAL

    | EXCLAMATION_MARK

    | BITWISE_SHIFT_LEFT | BITWISE_SHIFT_RIGHT

    | POWER_OPERATOR | CARET

    | PERCENT
    ;
value_atom
    : NUMBER
    | ID
    | PERCENTAGE
    ;