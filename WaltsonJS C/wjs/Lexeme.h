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
 * @param {int ival} Used for integer-valued Lexemes.
 * @param {double rval} Used for real-valued Lexemes.
 * @param {char *sval} Used for string-valued Lexemes.
 * @param {int line} Stores the line number of the lexical item.
 * @returns {Lexeme *} The constructed Lexeme. 
 */
extern Lexeme *newLexeme(char *type, int ival, double rval, char *sval, int line);

/**************************************************************************/
/*                             Display Method                             */
/**************************************************************************/

/**
 * Displays the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to display.
 * @param {FILE *fp} The targeted stream to display to.
 * @returns {int} 0, if the Lexeme is legal; 1, otherwise.
 */
extern int displayLexeme(Lexeme *l, FILE *fp);

/**************************************************************************/
/*                             Freeing Method                             */
/**************************************************************************/

/**
 * Frees the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to free.
 */
extern void freeLexeme(Lexeme *l);

/**************************************************************************/
/*                            Accessor Methods                            */
/**************************************************************************/

/**
 * Gets the type of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the type of.
 * @returns {char *} The type of the passed in Lexeme.
 */
extern char *getType(Lexeme *l);

/**
 * Gets the ival of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the ival of.
 * @returns {int} The ival of the passed in Lexeme.
 */
extern int getIval(Lexeme *l);

/**
 * Gets the rval of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the rval of.
 * @returns {double} The rval of the passed in Lexeme.
 */
extern double getRval(Lexeme *l);

/**
 * Gets the sval of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the sval of.
 * @returns {char *} The sval of the passed in Lexeme.
 */
extern char *getSval(Lexeme *l);

/**
 * Gets the Lexeme of aval at the given index.
 * 
 * @param {Lexeme *l} The Lexeme pointer containing an array.
 * @param {int index} The index to access.
 * @returns {Lexeme *} The Lexeme at the given index of the aval of the passed in Lexeme.
 */
extern Lexeme *getAval(Lexeme *l, int index);

/**
 * Gets the avalSize of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the avalSize of.
 * @returns {int} The avalSize of the passed in Lexeme.
 */
extern int getAvalSize(Lexeme *l);

/**
 * Gets the avalCapacity of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the avalCapacity of.
 * @returns {int} The avalCapacity of the passed in Lexeme.
 */
extern int getAvalCapacity(Lexeme *l);

/**
 * Gets the line number of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to get the line number of.
 * @returns {int} The line number of the passed in Lexeme.
 */
extern int getLine(Lexeme *l);

/**************************************************************************/
/*                            Mutator Methods                             */
/**************************************************************************/

/**
 * Sets the type of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to set the type of.
 * @param {char *newType} The new type for the passed in Lexeme.
 * @returns {char *} The old type of the passed in Lexeme.
 */
extern char *setType(Lexeme *l, char *newType);

/**
 * Sets the ival of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to set the ival of.
 * @param {int newIval} The new ival for the passed in Lexeme.
 * @returns {int} The old ival of the passed in Lexeme.
 */
extern int setIval(Lexeme *l, int newIval);

/**
 * Sets the rval of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to set the rval of.
 * @param {double newRval} The new rval for the passed in Lexeme.
 * @returns {double} The old rval of the passed in Lexeme.
 */
extern double setRval(Lexeme *l, double newRval);

/**
 * Sets the sval of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to set the sval of.
 * @param {char *newSval} The new sval for the passed in Lexeme.
 * @returns {char *} The old sval of the passed in Lexeme.
 */
extern char *setSval(Lexeme *l, char *newSval);

/**
 * Allocates space for an array within the passed in Lexeme.
 * 
 * @param {Lexeme *l} The ARRAY-type Lexeme to store an array of Lexemes in.
 * @param {int size} The size of the array to be allocated.
 * @returns {Lexeme **} The new array of Lexemes within the passed in Lexeme.
 */
extern Lexeme **newArray(Lexeme *l, int size);

/**
 * Sets the Lexeme of aval of the passed in Lexeme at the passed in index.
 * 
 * @param {Lexeme *l} The Lexeme pointer containing an array.
 * @param {int index} The index to access.
 * @param {Lexeme *newLexeme} The new Lexeme to be stored.
 * @returns {Lexeme *} The old Lexeme at the given index.
 */
extern Lexeme *setAval(Lexeme *l, int index, Lexeme *new);

/**
 * Adds an element to the array stored in the Lexeme passed in.
 * 
 * @param {Lexeme *l} The Lexeme pointer containing an array.
 * @param {Lexeme *new} The new Lexeme to add to the array.
 * @returns {Lexeme *} The inserted Lexeme.
 */
extern Lexeme *addAval(Lexeme *l, Lexeme *new);

/**
 * Sets the avalSize of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to set the avalSize of.
 * @param {int newAvalSize} The new avalSize for the passed in Lexeme.
 * @returns {int} The old avalSize of the passed in Lexeme.
 */
extern int setAvalSize(Lexeme *l, int newAvalSize);

/**
 * Sets the avalCapacity of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to set the avalCapacity of.
 * @param {int newAvalCapacity} The new avalCapacity for the passed in Lexeme.
 * @returns {int} The old avalCapacity of the passed in Lexeme.
 */
extern int setAvalCapacity(Lexeme *l, int newAvalCapacity);

/**
 * Sets the line of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme to set the line of.
 * @param {int newLine} The new line for the passed in Lexeme.
 * @returns {int} The old line of the passed in Lexeme.
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
 * @returns {Lexeme *} The root of the new Lexeme (or parse, in this case) tree.
 */
extern Lexeme *cons(char *type, Lexeme *left, Lexeme *right);

/**
 * Gets the left child of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme we want to get the left child of.
 * @returns {Lexeme *} The left child of the passed in Lexeme.
 */
extern Lexeme *car(Lexeme *l);

/**
 * Sets the left child of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme whose left child will be set.
 * @param {Lexeme *left} The Lexeme to become the left child.
 * @returns {Lexeme *} The old left child of the passed in Lexeme.
 */
extern Lexeme *setCar(Lexeme *l, Lexeme *left);

/**
 * Gets the right child of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme we want to get the right child of.
 * @returns {Lexeme *} The right child of the passed in Lexeme.
 */
extern Lexeme *cdr(Lexeme *l);

/**
 * Sets the right child of the passed in Lexeme.
 * 
 * @param {Lexeme *l} The Lexeme whose right child will be set.
 * @param {Lexeme *right} The Lexeme to become the right child.
 * @returns {Lexeme *} The old right child of the passed in Lexeme.
 */
extern Lexeme *setCdr(Lexeme *l, Lexeme *right);

#endif
