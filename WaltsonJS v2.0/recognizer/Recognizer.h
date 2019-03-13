/**
 * Recognizer module created for
 * recognizing syntax errors within
 * a WaltsonJS file.
 * 
 * Created by Walt Loumakis
 */

#ifndef __MY_RECOGNIZER__
#define __MY_RECOGNIZER__

/**
 * Initializes the Recognizer
 * by initializing the Lexer.
 * 
 * @param {char *filename} The path to the file to recognize.
 */
extern void initRecognizer(char *filename);

/**
 * Closes the file opened
 * during initialization.
 */
extern void closeRecognizer();

/**
 * Recognizes the file opened
 * during initialization.
 */
extern void recognize();

#endif
