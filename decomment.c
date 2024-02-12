/*The program performs a subset of the de-comment job of the C preprocessor */

/*The libraries used */
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

/*These are all the states used */
enum Statetype {INITIAL, STRING_LITERAL, CHARACTER_LITERAL, 
ESCAPE_STRING, ESCAPE_CHARACTER, MAYBE_A_COMMENT, IT_IS_A_COMMENT, 
MAYBE_CLOSING};

/*This handles the Intitial State */
enum Statetype handleInitialState(int inputChar, int lineCount, int startOfUnterminated)
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

/*This handles the String Literal State */
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

/*This handles the Character Literal State */
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

/*This handles the Escape String State */
enum Statetype handleEscapeStringState(int inputChar)
{
    enum Statetype state;
    putchar(inputChar);
    state = STRING_LITERAL;
    return state;
}

/*This handles the Escape Character State */
enum Statetype handleEscapeCharacterState(int inputChar)
{
    enum Statetype state;
    putchar(inputChar);
    state = CHARACTER_LITERAL;
    return state;
}

/*This handles the 'Maybe A Comment' State */
enum Statetype handleMaybeACommentState(int inputChar, int lineCount, int startOfUnterminated)
{
    enum Statetype state;
    if (inputChar == '*') {
        putchar(' ');
        startOfUnterminated += lineCount;
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

/*This handles the 'It Is A Comment' State */
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

/*This handles the 'Maybe Closing' State */
enum Statetype handleMaybeClosingState(int inputChar, int lineCount)
{
    enum Statetype state;
    if (inputChar == '/') {
        lineCount--;
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

/*This is the main where all the trasitions take place. */
int main(void)
{
    int inputChar; /*The current input character */
    int lineCount = 0; /*Keeps track of the line we are on */
    int startOfUnterminated = 1; /*Keeps track of the start of unterminated comment */
    enum Statetype state = INITIAL;
    while ((inputChar = getchar()) != EOF) {
        if (inputChar == '\n') { /*Keeps track of the line we are on */
            lineCount++;
        }
        switch (state) {
            case INITIAL:
                state = handleInitialState(inputChar, lineCount, startOfUnterminated);
                break;
            case STRING_LITERAL:
                state = handleStringLiteralState(inputChar);
                break;
            case CHARACTER_LITERAL:
                state = handleCharacterLiteralState(inputChar);
                break;
            case ESCAPE_STRING:
                state = handleEscapeStringState(inputChar);
                break;
            case ESCAPE_CHARACTER:
                state = handleEscapeCharacterState(inputChar);
                break;
            case MAYBE_A_COMMENT:
                state = handleMaybeACommentState(inputChar, lineCount, startOfUnterminated);
                break;
            case IT_IS_A_COMMENT:
                state = handleItIsACommentState(inputChar);
                break;
            case MAYBE_CLOSING:
                state = handleMaybeClosingState(inputChar, lineCount);
                break;
        }

    }

    /*Special case for a text ending with a forward slash when 
    in 'Maybe It Is A Comment' state */ 
    if (state == MAYBE_A_COMMENT) {
        putchar('/');
    }

    /* When the input ends with an unterminated comment and 
    print out exit failure message */
    if (state == IT_IS_A_COMMENT || state == MAYBE_CLOSING) {
        fprintf(stderr, "Error: line %d: unterminated comment\n", startOfUnterminated);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}