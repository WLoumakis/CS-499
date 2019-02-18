/**
 * Parser module for checking syntax errors.
 * This is an adapted Recognizer that returns
 * a parse tree comprised of Lexemes.
 * 
 * Created by Walt Loumakis
 */

#ifndef __MY_PARSER__
#define __MY_PARSER__

#include "Lexeme.h"

/**
 * Initializes the parser by intializing the Lexer.
 * 
 * @param {char *filename} Path to the filename to read from.
 */
extern void initParser(char *filename);

/**
 * Closes the file that was opened during initialization.
 */
extern void closeParser();


/**
 * Frees all the Lexemes in the given parse tree.
 * 
 * @param {Lexeme *tree} The tree to be freed.
 */
extern void freeParseTree(Lexeme *tree);

/**
 * Checks to make sure that Lexemes appear in the
 * correct order according to the grammar rules.
 * 
 * @returns {Lexeme *} The parse tree representing the file.
 */
extern Lexeme *parse();

#endif
