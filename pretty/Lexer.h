/**
 * Lexer module created to read through a file
 * and return lexical items as it comes across them.
 * 
 * Created by Walt Loumakis
 */

#ifndef __MY_LEXER__
#define __MY_LEXER__

#include "Lexeme.h"

/**
 * Initializes the Lexer to point to a given file.
 * 
 * @param {char *filename} The path of the filename to run lex() on.
 */
extern void initLexer(char *filename);

/**
 * Closes the file that was opened during initLexer().
 */
extern void closeLexer();

/**
 * Reads the file and returns a lexical item.
 * 
 * @returns {Lexeme *} The Lexeme corresponding to a type.
 */
extern Lexeme *lex();

#endif
