/**
 * Parser module created adapted from
 * recognizer module. This parser, in
 * addition to finding syntax errors,
 * will build a parse tree.
 * 
 * Created by Walt Loumakis
 */

#ifndef __MY_PARSER__
#define __MY_PARSER__

/**
 * Initializes the Parser
 * by initializing the Lexer.
 * 
 * @param {char *filename} The path to the file to parse.
 */
extern void initParser(char *filename);

/**
 * Closes the file opened
 * during initialization.
 */
extern void closeParser();

/**
 * Parses the file opened during initialization.
 * 
 * @returns {Lexeme *} The parse tree created.
 */
extern Lexeme *parse();

/**
 * Frees the passed in parse tree.
 * 
 * @param {Lexeme *tree} The tree to free.
 */
extern void freeParseTree(Lexeme *tree);

#endif
