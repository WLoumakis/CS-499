/**
 * Lexeme module created for Lexical items
 * corresponding to the grammar rules
 * laid out for generating NodeJS code.
 * 
 * Created by Walt Loumakis
 */

#ifndef __MY_LEXEME__
#define __MY_LEXEME__

#include <stdio.h>

#define UNDEFINED 0

/**
 * Structure definition for a Lexeme.
 */
typedef struct lexeme Lexeme;

/**************************************************************************/
/*                              Constructor                               */
/**************************************************************************/

/**
 * Constructs a new Lexeme.
 * 
 * @param {char *type} Determines the type of the Lexeme created.
 * @param {int ival} Used for integral-valued Lexemes.
 * @param {double rval} Used for real-valued Lexemes.
 * @param {char *sval} Used for string-valued Lexemes.
 * @param {int line} Stores the line number of the lexical item.
 * 
 * @constructor
 * 
 * @returns {Lexeme *} The constructed Lexeme. 
 */
extern Lexeme *newLexeme(char *type, int ival, double rval, char *sval, int line);

/**************************************************************************/
/*                             Display Method                             */
/**************************************************************************/

/**
 * Displays the Lexeme passed in.
 * NOTE: 	If trying to display a bad Lexeme,
 * 			this method will output an error and exit with code 1.
 * 
 * @param {FILE *fp} The targeted stream to display to.
 * @param {Lexeme *l} The Lexeme to display.
 */
extern void displayLexeme(FILE *fp, Lexeme *l);

/**************************************************************************/
/*                             Freeing Method                             */
/**************************************************************************/

/**
 * Frees the Lexeme passed in.
 * 
 * @param {Lexeme *l} The Lexeme to free.
 */
extern void freeLexeme(Lexeme *l);

/**************************************************************************/
/*                            Accessor Methods                            */
/**************************************************************************/

/**
 * Gets the type of the Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the type of.
 * 
 * @returns {char *} The type of Lexeme l.
 */
extern char *getType(Lexeme *l);

/**
 * Gets the ival of the Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the ival of.
 * 
 * @returns {int} The ival of Lexeme l.
 */
extern int getIval(Lexeme *l);

/**
 * Gets the rval of the Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the rval of.
 * 
 * @returns {double} The rval of Lexeme l.
 */
extern double getRval(Lexeme *l);

/**
 * Gets the sval of the Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the sval of.
 * 
 * @returns {char *} The sval of Lexeme l.
 */
extern char *getSval(Lexeme *l);

/**
 * Gets the line number of the Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the line number of.
 * 
 * @returns {int} The line number of Lexeme l.
 */
extern int getLine(Lexeme *l);

/**************************************************************************/
/*                            Mutator Methods                             */
/**************************************************************************/

/**
 * Sets the type of the Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the type of.
 * @param {char *newType} The new type for Lexeme l.
 * 
 * @returns {char *} The old type of Lexeme l.
 */
extern char *setType(Lexeme *l, char *newType);

/**
 * Sets the type of the Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the type of.
 * @param {char *newType} The new type for Lexeme l.
 * 
 * @returns {char *} The old type of Lexeme l.
 */
extern int setIval(Lexeme *l, int newIval);

/**
 * Sets the type of the Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the type of.
 * @param {char *newType} The new type for Lexeme l.
 * 
 * @returns {char *} The old type of Lexeme l.
 */
extern double setRval(Lexeme *l, double newRval);

/**
 * Sets the type of the Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the type of.
 * @param {char *newType} The new type for Lexeme l.
 * 
 * @returns {char *} The old type of Lexeme l.
 */
extern char *setSval(Lexeme *l, char *newSval);

/**
 * Sets the type of the Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the type of.
 * @param {char *newType} The new type for Lexeme l.
 * 
 * @returns {char *} The old type of Lexeme l.
 */
extern int setLine(Lexeme *l, int newLine);

/**************************************************************************/
/*                           Parse Tree Methods                           */
/**************************************************************************/

/**
 * Constructs a new Lexeme that glues two related Lexemes together.
 * 
 * @param {char *type} Determines the type of the Lexeme created.
 * @param {Lexeme *left} The pointer to the Lexeme who will become the left child of the new Lexeme.
 * @param {Lexeme *right} The pinter to the Lexeme who will become the right child of the new Lexeme.
 * 
 * @returns {Lexeme *} The root of the new Lexeme (or parse, in this case) tree.
 */
extern Lexeme *cons(char *type, Lexeme *left, Lexeme *right);

/**
 * Gets the left child of the passed in Lexeme, l.
 * 
 * @param {Lexeme *l} The Lexeme we want to get the left child of.
 * 
 * @returns {Lexeme *} The left child of l.
 */
extern Lexeme *car(Lexeme *l);

/**
 * Sets the left child of the passed in Lexeme, l, to left.
 * 
 * @param {Lexeme *l} The Lexeme whose left child will be set.
 * @param {Lexeme *left} The Lexeme to become the left child.
 * 
 * @returns {Lexeme *} The old left child of Lexeme l.
 */
extern Lexeme *setCar(Lexeme *l, Lexeme *left);

/**
 * Gets the right child of the passed in Lexeme l.
 * 
 * @param {Lexeme *l} The Lexeme we want to get the right child of.
 * 
 * @returns {Lexeme *} The right child of l.
 */
extern Lexeme *cdr(Lexeme *l);

/**
 * Sets the right child of the passed in Lexeme, l, to right.
 * 
 * @param {Lexeme *l} The Lexeme whose right child will be set.
 * @param {Lexeme *right} The Lexeme to become the right child.
 * 
 * @returns {Lexeme *} The old right child of Lexeme l.
 */
extern Lexeme *setCdr(Lexeme *l, Lexeme *right);

#endif
