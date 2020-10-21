/**
 * Project: Implementation of a compiler of the IFJ20 language.
 * @file    finite_automata.c
 * @brief   This file contains the implementation of the scanner rules.
 * @author  Boris Karavasilev <xkarav01@stud.fit.vutbr.cz>
 * @author  Domink Vecera <xvecer23@stud.fit.vutbr.cz>
 * @author  Robin Stancl <xstanc09@stud.fit.vutbr.cz>
 * @date    4. 10. 2020
 */

#include "finite_automata.h"
#include "states_list.h"
#include "token_types.h"
#include <stdio.h>


void init_finite_automata(finite_automataT* fa) {
    // Init array with predefined values that cant occur in the array
    init_int_array(fa->final_states, FINAL_STATES_LEN, 0);
    init_int_array(fa->states, STATES_LEN, 0);
    init_rules_array(fa->rules, RULES_LEN);
    // end of initializing arrays

    // ---------------------------------------------------------------------
    // .-= FINITE AUTOMATA DEFINITION =-.
    //
    // Representation of 'Konecny_automat_IFJ20.png'
    //
    //

    // States
    fa->states[0] = STATE_START;
    fa->states[1] = TOKEN_EOF;
    fa->states[2] = TOKEN_MINUS;
    fa->states[3] = TOKEN_PLUS;
    fa->states[4] = TOKEN_MULTIPLICATION;
    fa->states[5] = TOKEN_DIVISION;
    fa->states[6] = TOKEN_LEFT_BRACKET;
    fa->states[7] = TOKEN_RIGHT_BRACKET;
    fa->states[8] = TOKEN_COMMA;
    fa->states[9] = STATE_COLON;
    fa->states[10] = TOKEN_COLON_EQUAL;
    fa->states[11] = STATE_EXPLANATION_MARK;
    fa->states[12] = TOKEN_NOT_EQUAL;
    fa->states[13] = TOKEN_LEFT_ARROW;
    fa->states[14] = TOKEN_RIGHT_ARROW;
    fa->states[15] = TOKEN_LESS_OR_EQUAL;
    fa->states[16] = TOKEN_GREATER_OR_EQUAL;
    fa->states[17] = TOKEN_LEFT_CURLY_BRACE;
    fa->states[18] = TOKEN_RIGHT_CURLY_BRACE;
    fa->states[19] = TOKEN_SEMICOLON;
    fa->states[20] = TOKEN_EQUAL;
    fa->states[21] = TOKEN_DOUBLE_EQUAL;
    fa->states[22] = STATE_LINE_COMMENT_BODY;
    fa->states[23] = TOKEN_LINE_COMMENT;
    fa->states[24] = STATE_BLOCK_COMMENT_BODY;
    fa->states[25] = STATE_BLOCK_COMMENT_END;
    fa->states[26] = TOKEN_BLOCK_COMMENT;
    fa->states[27] = STATE_STRING;
    fa->states[28] = STATE_BACKSLASH;
    fa->states[29] = STATE_HEX_NUMBER;
    fa->states[30] = STATE_F;
    fa->states[31] = STATE_FF;
    fa->states[32] = TOKEN_STRING_LITERAL;
    fa->states[33] = TOKEN_UNDERSCORE;
    fa->states[34] = TOKEN_IDENTIFIER;
    fa->states[35] = TOKEN_INTEGER_LITERAL;
    fa->states[36] = STATE_EXPONENT;
    fa->states[37] = STATE_PLUS_MINUS;
    fa->states[38] = STATE_DECIMAL_NUMBER_NOT_END;
    fa->states[39] = TOKEN_DECIMAL_LITERAL;
    fa->states[40] = TOKEN_EXPONENT_LITERAL;
    fa->states[41] = TOKEN_SLASH;

    // Start state
    fa->start_state = STATE_START;

    // Final states
    fa->final_states[0] = TOKEN_EOF;
    fa->final_states[1] = TOKEN_MINUS;
    fa->final_states[2] = TOKEN_PLUS;
    fa->final_states[3] = TOKEN_MULTIPLICATION;
    fa->final_states[4] = TOKEN_DIVISION;
    fa->final_states[5] = TOKEN_LEFT_BRACKET;
    fa->final_states[6] = TOKEN_RIGHT_BRACKET;
    fa->final_states[7] = TOKEN_COMMA;
    fa->final_states[8] = TOKEN_COLON_EQUAL;
    fa->final_states[9] = TOKEN_NOT_EQUAL;
    fa->final_states[10] = TOKEN_LEFT_ARROW;
    fa->final_states[11] = TOKEN_RIGHT_ARROW;
    fa->final_states[12] = TOKEN_LESS_OR_EQUAL;
    fa->final_states[13] = TOKEN_GREATER_OR_EQUAL;
    fa->final_states[14] = TOKEN_LEFT_CURLY_BRACE;
    fa->final_states[15] = TOKEN_RIGHT_CURLY_BRACE;
    fa->final_states[16] = TOKEN_SEMICOLON;
    fa->final_states[17] = TOKEN_EQUAL;
    fa->final_states[18] = TOKEN_DOUBLE_EQUAL;
    fa->final_states[19] = TOKEN_LINE_COMMENT;
    fa->final_states[20] = TOKEN_BLOCK_COMMENT;
    fa->final_states[21] = TOKEN_STRING_LITERAL;
    fa->final_states[22] = TOKEN_UNDERSCORE;
    fa->final_states[23] = TOKEN_IDENTIFIER;
    fa->final_states[24] = TOKEN_INTEGER_LITERAL;
    fa->final_states[25] = TOKEN_DECIMAL_LITERAL;
    fa->final_states[26] = TOKEN_EXPONENT_LITERAL;
    fa->final_states[27] = TOKEN_SLASH;

    // --- Rule #0
    fa->rules[0].from_state = STATE_START;
    fa->rules[0].to_state = TOKEN_EOF;
    fa->rules[0].transition_ranges[0].single_char = EOF;

    // --- Rule #1
    fa->rules[1].from_state = STATE_START;
    fa->rules[1].to_state = TOKEN_MINUS;
    fa->rules[1].transition_ranges[0].single_char = '-';

    // --- Rule #2
    fa->rules[2].from_state = STATE_START;
    fa->rules[2].to_state = TOKEN_PLUS;
    fa->rules[2].transition_ranges[0].single_char = '+';

    // --- Rule #3
    fa->rules[3].from_state = STATE_START;
    fa->rules[3].to_state = TOKEN_MULTIPLICATION;
    fa->rules[3].transition_ranges[0].single_char = '*';

    // --- Rule #4
    fa->rules[4].from_state = STATE_START;
    fa->rules[4].to_state = TOKEN_LEFT_BRACKET;
    fa->rules[4].transition_ranges[0].single_char = '(';

    // --- Rule #5
    fa->rules[5].from_state = STATE_START;
    fa->rules[5].to_state = TOKEN_RIGHT_BRACKET;
    fa->rules[5].transition_ranges[0].single_char = ')';

    // --- Rule #6
    fa->rules[6].from_state = STATE_START;
    fa->rules[6].to_state = TOKEN_COMMA;
    fa->rules[6].transition_ranges[0].single_char = ',';

    // --- Rule #7
    fa->rules[7].from_state = STATE_START;
    fa->rules[7].to_state = STATE_COLON;
    fa->rules[7].transition_ranges[0].single_char = ':';

    // --- Rule #8
    fa->rules[8].from_state = STATE_COLON;
    fa->rules[8].to_state = TOKEN_COLON_EQUAL;
    fa->rules[8].transition_ranges[0].single_char = '=';

    // --- Rule #9
    fa->rules[9].from_state = STATE_START;
    fa->rules[9].to_state = STATE_EXPLANATION_MARK;
    fa->rules[9].transition_ranges[0].single_char = '!';

    // --- Rule #10
    fa->rules[10].from_state = STATE_EXPLANATION_MARK;
    fa->rules[10].to_state = TOKEN_NOT_EQUAL;
    fa->rules[10].transition_ranges[0].single_char = '=';

    // --- Rule #11
    fa->rules[11].from_state = STATE_START;
    fa->rules[11].to_state = TOKEN_LEFT_ARROW;
    fa->rules[11].transition_ranges[0].single_char = '<';

    // --- Rule #12
    fa->rules[12].from_state = TOKEN_LEFT_ARROW;
    fa->rules[12].to_state = TOKEN_LESS_OR_EQUAL;
    fa->rules[12].transition_ranges[0].single_char = '=';

    // --- Rule #13
    fa->rules[13].from_state = STATE_START;
    fa->rules[13].to_state = TOKEN_RIGHT_ARROW;
    fa->rules[13].transition_ranges[0].single_char = '>';

    // --- Rule #14
    fa->rules[14].from_state = TOKEN_RIGHT_ARROW;
    fa->rules[14].to_state = TOKEN_GREATER_OR_EQUAL;
    fa->rules[14].transition_ranges[0].single_char = '=';

    // --- Rule #15
    fa->rules[15].from_state = STATE_START;
    fa->rules[15].to_state = TOKEN_LEFT_CURLY_BRACE;
    fa->rules[15].transition_ranges[0].single_char = '{';

    // --- Rule #16
    fa->rules[16].from_state = STATE_START;
    fa->rules[16].to_state = TOKEN_RIGHT_CURLY_BRACE;
    fa->rules[16].transition_ranges[0].single_char = '}';

    // --- Rule #17
    fa->rules[17].from_state = STATE_START;
    fa->rules[17].to_state = TOKEN_SEMICOLON;
    fa->rules[17].transition_ranges[0].single_char = ';';

    // --- Rule #18
    fa->rules[18].from_state = STATE_START;
    fa->rules[18].to_state = TOKEN_EQUAL;
    fa->rules[18].transition_ranges[0].single_char = '=';

    // --- Rule #19
    fa->rules[19].from_state = TOKEN_EQUAL;
    fa->rules[19].to_state = TOKEN_DOUBLE_EQUAL;
    fa->rules[19].transition_ranges[0].single_char = '=';

    // --- Rule #20
    fa->rules[20].from_state = STATE_START;
    fa->rules[20].to_state = TOKEN_SLASH;
    fa->rules[20].transition_ranges[0].single_char = '/';

    // --- Rule #21
    fa->rules[21].from_state = TOKEN_SLASH;
    fa->rules[21].to_state = STATE_LINE_COMMENT_BODY;
    fa->rules[21].transition_ranges[0].single_char = '/';

    // --- Rule #22
    fa->rules[22].from_state = STATE_LINE_COMMENT_BODY;
    fa->rules[22].to_state = STATE_LINE_COMMENT_BODY;
    fa->rules[22].transition_ranges[0].from = 0;
    fa->rules[22].transition_ranges[0].to = 9;
    fa->rules[22].transition_ranges[1].from = 11;
    fa->rules[22].transition_ranges[1].to = 127;
    //TODO others - ASCII 10 is LF (Line Feed - new line), EOF is not ASCII character

    // --- Rule #23
    fa->rules[23].from_state = STATE_LINE_COMMENT_BODY;
    fa->rules[23].to_state = TOKEN_LINE_COMMENT;
    fa->rules[23].transition_ranges[0].single_char = '\n';
    fa->rules[23].transition_ranges[1].single_char = EOF;

    // --- Rule #24
    fa->rules[24].from_state = TOKEN_SLASH;
    fa->rules[24].to_state = STATE_BLOCK_COMMENT_BODY;
    fa->rules[24].transition_ranges[0].single_char = '*';

    // --- Rule #25
    fa->rules[25].from_state = STATE_BLOCK_COMMENT_BODY;
    fa->rules[25].to_state = STATE_BLOCK_COMMENT_BODY;
    fa->rules[25].transition_ranges[0].from = 0;
    fa->rules[25].transition_ranges[0].to = 41;
    fa->rules[25].transition_ranges[1].from = 43;
    fa->rules[25].transition_ranges[1].to = 127;
    //TODO others

    // --- Rule #26
    fa->rules[26].from_state = STATE_BLOCK_COMMENT_BODY;
    fa->rules[26].to_state = STATE_BLOCK_COMMENT_END;
    fa->rules[26].transition_ranges[0].single_char = '*';

    // --- Rule #27
    fa->rules[27].from_state = STATE_BLOCK_COMMENT_END;
    fa->rules[27].to_state = STATE_BLOCK_COMMENT_BODY;
    fa->rules[27].transition_ranges[0].from = 0;
    fa->rules[27].transition_ranges[0].to = 46;
    fa->rules[27].transition_ranges[1].from = 48;
    fa->rules[27].transition_ranges[1].to = 127;
    //TODO others

    // --- Rule #28
    fa->rules[28].from_state = STATE_BLOCK_COMMENT_END;
    fa->rules[28].to_state = TOKEN_BLOCK_COMMENT;
    fa->rules[28].transition_ranges[0].single_char = '/';

    // --- Rule #29
    fa->rules[29].from_state = STATE_START;
    fa->rules[29].to_state = STATE_STRING;
    fa->rules[29].transition_ranges[0].single_char = '"';

    // --- Rule #30
    fa->rules[30].from_state = STATE_STRING;
    fa->rules[30].to_state = STATE_STRING;
    fa->rules[30].transition_ranges[0].from = 32;
    fa->rules[30].transition_ranges[0].to = 127;
    //TODO ASCII > 31

    // --- Rule #31
    fa->rules[31].from_state = STATE_STRING;
    fa->rules[31].to_state = TOKEN_STRING_LITERAL;
    fa->rules[31].transition_ranges[0].single_char = '"';

    // --- Rule #32
    fa->rules[32].from_state = STATE_STRING;
    fa->rules[32].to_state = STATE_BACKSLASH;
    fa->rules[32].transition_ranges[0].single_char = 92; //ASCII code of backslash symbol is 92

    // --- Rule #33
    fa->rules[33].from_state = STATE_BACKSLASH;
    fa->rules[33].to_state = STATE_HEX_NUMBER;
    fa->rules[33].transition_ranges[0].single_char = 'x';

    // --- Rule #34
    fa->rules[34].from_state = STATE_HEX_NUMBER;
    fa->rules[34].to_state = STATE_F;
    fa->rules[34].transition_ranges[0].from = '0';
    fa->rules[34].transition_ranges[0].to = '9';
    fa->rules[34].transition_ranges[1].from = 'A';
    fa->rules[34].transition_ranges[1].to = 'F';
    fa->rules[34].transition_ranges[2].from = 'a';
    fa->rules[34].transition_ranges[2].to = 'f';

    // --- Rule #35
    fa->rules[35].from_state = STATE_F;
    fa->rules[35].to_state = STATE_FF;
    fa->rules[35].transition_ranges[0].from = '0';
    fa->rules[35].transition_ranges[0].to = '9';
    fa->rules[35].transition_ranges[1].from = 'A';
    fa->rules[35].transition_ranges[1].to = 'F';
    fa->rules[35].transition_ranges[2].from = 'a';
    fa->rules[35].transition_ranges[2].to = 'f';

    // --- Rule #36
    fa->rules[36].from_state = STATE_FF;
    fa->rules[36].to_state = STATE_STRING;
    fa->rules[36].transition_ranges[0].from = 32;
    fa->rules[36].transition_ranges[0].to = 127;
    //TODO ASCII > 31

    // --- Rule #37
    fa->rules[37].from_state = STATE_FF;
    fa->rules[37].to_state = TOKEN_STRING_LITERAL;
    fa->rules[37].transition_ranges[0].single_char = '"';

    // --- Rule #38
    fa->rules[38].from_state = STATE_BACKSLASH;
    fa->rules[38].to_state = STATE_STRING;
    fa->rules[38].transition_ranges[0].single_char = '"';
    fa->rules[38].transition_ranges[1].single_char = 'n';
    fa->rules[38].transition_ranges[2].single_char = 't';
    fa->rules[38].transition_ranges[3].single_char = 92; //92 is ASCII code od backslash symbol

    // --- Rule #39
    fa->rules[39].from_state = STATE_START;
    fa->rules[39].to_state = TOKEN_UNDERSCORE;
    fa->rules[39].transition_ranges[0].single_char = '_';

    // --- Rule #40
    fa->rules[40].from_state = STATE_START;
    fa->rules[40].to_state = TOKEN_IDENTIFIER;
    fa->rules[40].transition_ranges[0].from = 'a';
    fa->rules[40].transition_ranges[0].to = 'z';
    fa->rules[40].transition_ranges[1].from = 'A';
    fa->rules[40].transition_ranges[1].to = 'Z';

    // --- Rule #41
    fa->rules[41].from_state = TOKEN_IDENTIFIER;
    fa->rules[41].to_state = TOKEN_IDENTIFIER;
    fa->rules[41].transition_ranges[0].from = 'a';
    fa->rules[41].transition_ranges[0].to = 'z';
    fa->rules[41].transition_ranges[1].from = 'A';
    fa->rules[41].transition_ranges[1].to = 'Z';
    fa->rules[41].transition_ranges[2].from = '0';
    fa->rules[41].transition_ranges[2].to = '9';
    fa->rules[41].transition_ranges[3].single_char = '_';

    // --- Rule #42
    fa->rules[42].from_state = TOKEN_UNDERSCORE;
    fa->rules[42].to_state = TOKEN_IDENTIFIER;
    fa->rules[42].transition_ranges[0].from = 'a';
    fa->rules[42].transition_ranges[0].to = 'z';
    fa->rules[42].transition_ranges[1].from = 'A';
    fa->rules[42].transition_ranges[1].to = 'Z';
    fa->rules[42].transition_ranges[2].from = '0';
    fa->rules[42].transition_ranges[2].to = '9';
    fa->rules[42].transition_ranges[3].single_char = '_';

    // --- Rule #43
    fa->rules[43].from_state = STATE_START;
    fa->rules[43].to_state = TOKEN_INTEGER_LITERAL;
    fa->rules[43].transition_ranges[0].from = '0';
    fa->rules[43].transition_ranges[0].to = '9';

    // --- Rule #44
    fa->rules[44].from_state = TOKEN_INTEGER_LITERAL;
    fa->rules[44].to_state = TOKEN_INTEGER_LITERAL;
    fa->rules[44].transition_ranges[0].from = '0';
    fa->rules[44].transition_ranges[0].to = '9';

    // --- Rule #45
    fa->rules[45].from_state = TOKEN_INTEGER_LITERAL;
    fa->rules[45].to_state = STATE_EXPONENT;
    fa->rules[45].transition_ranges[0].single_char = 'e';
    fa->rules[45].transition_ranges[1].single_char = 'E';

    // --- Rule #46
    fa->rules[46].from_state = TOKEN_INTEGER_LITERAL;
    fa->rules[46].to_state = STATE_DECIMAL_NUMBER_NOT_END;
    fa->rules[46].transition_ranges[0].single_char = '.';

    // --- Rule #47
    fa->rules[47].from_state = STATE_DECIMAL_NUMBER_NOT_END;
    fa->rules[47].to_state = TOKEN_DECIMAL_LITERAL;
    fa->rules[47].transition_ranges[0].from = '0';
    fa->rules[47].transition_ranges[0].to = '9';

    // --- Rule #48
    fa->rules[48].from_state = TOKEN_DECIMAL_LITERAL;
    fa->rules[48].to_state = TOKEN_DECIMAL_LITERAL;
    fa->rules[48].transition_ranges[0].from = '0';
    fa->rules[48].transition_ranges[0].to = '9';

    // --- Rule #49
    fa->rules[49].from_state = TOKEN_DECIMAL_LITERAL;
    fa->rules[49].to_state = STATE_EXPONENT;
    fa->rules[49].transition_ranges[0].single_char = 'e';
    fa->rules[49].transition_ranges[1].single_char = 'E';

    // --- Rule #50
    fa->rules[50].from_state = STATE_EXPONENT;
    fa->rules[50].to_state = STATE_PLUS_MINUS;
    fa->rules[50].transition_ranges[0].single_char = '+';
    fa->rules[50].transition_ranges[1].single_char = '-';

    // --- Rule #51
    fa->rules[51].from_state = STATE_PLUS_MINUS;
    fa->rules[51].to_state = TOKEN_EXPONENT_LITERAL;
    fa->rules[51].transition_ranges[0].from = '0';
    fa->rules[51].transition_ranges[0].to = '9';

    // --- Rule #52
    fa->rules[52].from_state = STATE_EXPONENT;
    fa->rules[52].to_state = TOKEN_EXPONENT_LITERAL;
    fa->rules[52].transition_ranges[0].from = '0';
    fa->rules[52].transition_ranges[0].to = '9';

    // --- Rule #53
    fa->rules[53].from_state = TOKEN_EXPONENT_LITERAL;
    fa->rules[53].to_state = TOKEN_EXPONENT_LITERAL;
    fa->rules[53].transition_ranges[0].from = '0';
    fa->rules[53].transition_ranges[0].to = '9';

    // --- Rule #54
    fa->rules[54].from_state = STATE_START;
    fa->rules[54].to_state = STATE_START;
    fa->rules[54].transition_ranges[0].single_char = ' ';
    fa->rules[54].transition_ranges[1].single_char = '\n';
    //TODO Maybe other white space characters?
}

void init_int_array(int int_array[], int len, int init_value) {
    for (int i = 0; i < len; i++) {
        int_array[i] = init_value;
    }
}

void init_rules_array(ruleT rules_array[], int len) {
    for (int i = 0; i < len; i++) {
        init_rule(&rules_array[i]);
    }
}

void init_rule(ruleT *rule) {
    rule->to_state = 0;
    rule->from_state = 0;

    for (int i = 0; i < TRANS_RANGES_LEN; i++) {
        rule->transition_ranges[i].from = -1;
        rule->transition_ranges[i].to = -1;
        rule->transition_ranges[i].single_char = -1;
    }
}
