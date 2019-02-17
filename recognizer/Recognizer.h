/**
 * Recognizer module for checking syntax errors.
 * 
 * Created by Walt Loumakis.
 */

#ifndef __MY_RECOGNIZER__
#define __MY_RECOGNIZER__

/**
 * Initializes the recognizer by intializing the Lexer.
 * 
 * @param {char *filename} Path to the filename to read from.
 */
extern void initRecognizer(char *filename);

/**
 * Closes the file that was opened during initialization.
 */
extern void closeRecognizer();

/**
 * Checks to make sure that Lexemes appear in the
 * correct order according to the grammar rules.
 * 
 * Prints "legal" if the file is syntactically correct,
 * otherwise prints "illegal"
 */
extern void recognize();

#endif
