/**
 * Translator module created for translating WaltsonJS
 * code into NodeJS code for building a IBM Watson Chatbot.
 * This is similar to the pretty printer, except it prints
 * NodeJS code as opposed to duplicating the input code.
 * 
 * Created by Walt Loumakis
 */

#ifndef __MY_TRANSLATOR__
#define __MY_TRANSLATOR__

#include <stdio.h>
#include "Lexeme.h"

/**
 * Initializes the Translator by opening the file for writing.
 * 
 * @param {char *filename} The path of the file to write to.
 * @returns {FILE *} The opened file.
 */
extern FILE *initTranslator(char *filename);

/**
 * Closes the file opened during initialization.
 */
extern void closeTranslator();

/**
 * The primary dispatching function for translating a WaltsonJS program into a
 * NodeJS program.
 * Literals (STRINGs, REALs, etc.) print the literal.
 * ID Lexemes print the name of the variable.
 * PROGRAM Lexemes are dispatched to translateProgram.
 * MINUS Lexemes are dispatched to translateMinus.
 * COLON Lexemes are dispatched to translateAssign.
 * OBJECT Lexemes are dispatched to translateObject.
 * ATTRLIST Lexemes are dispateched to translateAttrList.
 * MIXINLIST Lexemes are dispatched to translateArray.
 * 
 * @param {Lexeme *tree} The parse tree to translate.
 */
extern void translate(Lexeme *tree);

/**
 * Translates a PROGRAM parse tree.
 * 
 * @param {Lexeme *tree} The PROGRAM parse tree.
 */
extern void translateProgram(Lexeme *tree);

/**
 * Translates a MINUS parse tree.
 * 
 * @param {Lexeme *tree} The MINUS parse tree.
 */
extern void translateMinus(Lexeme *tree);

/**
 * Translates a COLON parse tree.
 * 
 * @param {Lexeme *tree} The COLON parse tree.
 */
extern void translateAssign(Lexeme *tree);

/**
 * Translates an OBJECT parse tree.
 * 
 * @param {Lexeme *tree} The OBJECT parse tree.
 */
extern void translateObject(Lexeme *tree);

/**
 * Translates an ATTRLIST parse tree.
 * 
 * @param {Lexeme *list} The ATTRLIST parse tree.
 */
extern void translateAttrList(Lexeme *list);

/**
 * Translates a MIXINLIST parse tree.
 * 
 * @param {Lexeme *list} The MIXINLIST parse tree.
 */
extern void translateMixinList(Lexeme *list);

/**
 * Translates an ARRAY Lexeme.
 * 
 * @param {Lexeme *l} The ARRAY Lexeme.
 */
extern void translateArray(Lexeme *l);

/**
 * Prints information regarding why this may have been triggered an exits.
 * 
 * @param {Lexeme *tree} The parse tree that caused an error.
 * @exits
 */
extern void translateError(Lexeme *tree);

#endif