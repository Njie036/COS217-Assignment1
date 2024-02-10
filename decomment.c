#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

enum Statetype {INITIAL, STRING_LITERAL, CHARACTER_LITERAL, 
ESCAPE_STRING, ESCAPE_CHARACTER, MAYBE_A_COMMENT, IT_IS_A_COMMENT, 
MAYBE_CLOSING};

enum Statetype handleInitialState(int inputChar)
{
    enum Statetype state;
    if (inputChar == '/') {
        state = MAYBE_A_COMMENT;
    }
    else if (inputChar == '"') {
        putchar(inputChar);
        state = STRING_LITERAL;
    }
    else if (inputChar == '\'') {
        putchar(inputChar);
        state = CHARACTER_LITERAL;
    }
    else { 
        putchar(inputChar);
        state = INITIAL;
    }
    return state;

}

enum Statetype handleStringLiteralState(int inputChar)
{ 
    enum Statetype state;
    if (inputChar == '"') {
        putchar(inputChar);
        state = INITIAL;
    }
    else if (inputChar == '\\'){
        putchar(inputChar);
        state = ESCAPE_STRING;
    }
    else{  
        putchar(inputChar);
        state = STRING_LITERAL;
    }
    return state;

}

enum Statetype handleCharacterLiteralState(int inputChar)
{ 
    enum Statetype state;
    if (inputChar == '\'') {
        putchar(inputChar);
        state = INITIAL;
    }
    else if (inputChar == '\\') {
        putchar(inputChar);
        state = ESCAPE_CHARACTER;
    }
    else  {
        putchar(inputChar);
        state = CHARACTER_LITERAL;
    }
    return state;
}

enum Statetype handleEscapeStringState(int inputChar)
{
    enum Statetype state;
    putchar(inputChar);
    state = STRING_LITERAL;
    return state;
}

enum Statetype handleEscapeCharacterState(int inputChar)
{
    enum Statetype state;
    putchar(inputChar);
    state = CHARACTER_LITERAL;
    return state;
}

enum Statetype handleMaybeACommentState(int inputChar)
{
    enum Statetype state;
    if (inputChar == '*') {
        putchar(' ');
        state = IT_IS_A_COMMENT;
    }
    else if (inputChar == '/') {
        /*putchar(inputChar); */  /*I added this to handle the case in 04..7.txt*/
        state = MAYBE_A_COMMENT;
    }
    else if (inputChar == '\'') {
        putchar('/');
        putchar(inputChar);
        state = CHARACTER_LITERAL;
    }
    else if (inputChar == '"') {
        putchar('/');
        putchar(inputChar);
        state = STRING_LITERAL;
    }
    else {
        putchar('/');
        putchar(inputChar);
        state = INITIAL;
    }
    return state;
}

enum Statetype handleItIsACommentState(int inputChar)
{
    enum Statetype state;
    if (inputChar == '*') {
        state = MAYBE_CLOSING;
    }
    else {
        if (inputChar == '\n') {
            putchar(inputChar);
        }
        state = IT_IS_A_COMMENT;
    }
    return state;
}

enum Statetype handleMaybeClosingState(int inputChar)
{
    enum Statetype state;
    if (inputChar == '/') {
        state = INITIAL;
    }
    else if (inputChar == '*'){
        state = MAYBE_CLOSING;
    }
    else {
        if (inputChar == '\n') {
            putchar(inputChar);
        }
        state = IT_IS_A_COMMENT;
    }
    return state;
}

int main(void)
{
    int inputChar;
    enum Statetype state = INITIAL;
    int lineCount = 1;
    while ((inputChar = getchar()) != EOF) {
        switch (state) {
            case INITIAL:
                state = handleInitialState(inputChar);
                break;
            case STRING_LITERAL:
                state = handleStringLiteralState(inputChar);
                break;
            case CHARACTER_LITERAL:
                state = handleCharacterLiteralState(inputChar);
                break;
            case ESCAPE_STRING:
                state = handleEscapeCharacterState(inputChar);
                break;
            case ESCAPE_CHARACTER:
                state = handleEscapeCharacterState(inputChar);
                break;
            case MAYBE_A_COMMENT:
                state = handleMaybeACommentState(inputChar);
                break;
            case IT_IS_A_COMMENT:
                state = handleItIsACommentState(inputChar);
                break;
            case MAYBE_CLOSING:
                state = handleMaybeClosingState(inputChar);
                break;
        }
        if (inputChar == '\n') {
            lineCount ++;
        }
    }
    /*special case for ending with a forward slash when in maybe its a comment*/ 
    if (state == MAYBE_A_COMMENT) {
        putchar('/');
    }
    /* Special case for ending in an unterminated comment and print out exit failure message */
    if (state == IT_IS_A_COMMENT || state == MAYBE_CLOSING) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", lineCount);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}