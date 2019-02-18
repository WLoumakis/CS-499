/**
 * Pretty Printer module created for printing
 * a parse tree into its respective code.
 * 
 * Created by Walt Loumakis
 */

#ifndef __MY_PRETTY__
#define __MY_PRETTY__

#include "Lexeme.h"

/**
 * Initializes the pretty printer by intializing
 * the parser with the file to read from.
 * 
 * @param {char *filename} The file to read from.
 */
extern void initPretty(char *filename);

/**
 * Closes the file that was passed in
 * during initialization.
 */
extern void closePretty();

/**
 * Prints the entire parse tree passed in to sdout.
 * 
 * @param {Lexeme *tree} A Lexeme representing the root of a parse tree.
 */
extern void pretty(Lexeme *tree);

#endif
