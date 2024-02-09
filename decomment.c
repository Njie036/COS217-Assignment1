#include <stdio.h>
#include <ctype.h>

enum Statetype {INITIAL, STRING_LITERAL, CHARACTER_LITERAL, 
ESCAPE_STRING, ESCAPE_CHARACTER, MAYBE_A_COMMENT, IT_IS_A_COMMENT, 
MAYBE_CLOSING};

enum Statetype handleInitialState(char inputChar)
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

enum Statetype handleStringLiteralState(char inputChar)
{ 
    enum Statetype state;
    if (inputChar == '"') {
        putchar(inputChar);
        state = INITIAL;
    }
    else if (inputChar == '/'){
        putchar(inputChar);
        state = ESCAPE_STRING;
    }
    else{  
        putchar(inputChar);
        state = STRING_LITERAL;
    }
    return state;

}

enum Statetype
handleCharacterLiteralState(char inputChar)
{ 
    enum Statetype state;
    if (inputChar == '/') {
        putchar(inputChar);
        state = ESCAPE_CHARACTER;
    }
    else  {
        putchar(inputChar);
        state = CHARACTER_LITERAL;
    }
    return state;
}

enum Statetype 
handleEscapeStringState(char inputChar)
{
    enum Statetype state;
    if (inputChar) {
        putchar(inputChar);
        state = STRING_LITERAL;
    }
    return state;
}

enum Statetype 
handleEscapeCharacterState(char inputChar)
{
    enum Statetype state;
    if (inputChar) {
        putchar(inputChar);
        state = CHARACTER_LITERAL;
    }
    return state;
}

enum Statetype handleMaybeACommentState(char inputChar)
{
    enum Statetype state;
    if (inputChar == '*') {
        putChar(" ");
        state = IT_IS_A_COMMENT;
    }
    else if (inputChar == '/') {
        putchar(inputChar);
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

enum Statetype 
handleItIsACommentState(char inputChar)
{
    enum Statetype state;
    if (inputChar == '*') {
        putchar(' ');
        state = MAYBE_CLOSING;
    }
    else {
        putchar(inputChar);
        state = IT_IS_A_COMMENT;
    }
}

enum Statetype 
handleMaybeClosingState(char inputChar)
{
    enum Statetype state;
    if (inputChar == '/') {
        putchar(' ');
        state = INITIAL;
    }
    else {
        putchar(inputChar);
        state = IT_IS_A_COMMENT;
    }
}

int main(void)
{
    char inputChar;
    enum Statetype state = INITIAL;
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
                state = handleEscapeCharacter(inputChar);
                break;
            case ESCAPE_CHARACTER:
                state = handleEscapeCharacter(inputChar);
                break;
            case MAYBE_A_COMMENT:
                state = handleMaybeAComment(inputChar);
                break;
            case IT_IS_A_COMMENT:
                state = handleItIsAComment(inputChar);
                break;
            case MAYBE_CLOSING:
                state = handleMaybeClosing(inputChar);
                break;
        }
    }
    return 0;
}