/**
 * Evaluator module created to
 * evalue a parse tree created
 * by parsing a given WaltsonJS file.
 * 
 * Created by Walt Loumakis
 */

#ifndef __MY_EVALUATOR__
#define __MY_EVALUATOR__

#include "Lexeme.h"

/**
 * The primary dispatching function for evaluation of a WaltsonJS program.
 * Literals (STRINGs, REALs, etc.) return the tree.
 * ID Lexemes are dispatched to the environment lookup function.
 * PROGRAM Lexemes are dispatched to evalProgram.
 * MINUS Lexemes are dispatched to evalMinus.
 * COLON Lexemes are dispatched to evalAssign.
 * OBJECT Lexemes are dispatched to evalObject.
 * ATTRLIST Lexemes are dispatched to evalAttrList.
 * Any other Lexemes will be dispatched to evalError.
 * 
 * @param {Lexeme *tree} The parse tree to evaluate.
 * @param {Lexeme *env} The environment in which to evaluate the parse tree.
 * @returns {Lexeme *} The evaluation of the parse tree | NULL.
 */
extern Lexeme *eval(Lexeme *tree, Lexeme *env);

/**
 * Evaluates a PROGRAM parse tree. Returns a Lexeme corresponding to the
 * evaluation of the last definition.
 * 
 * @param {Lexeme *tree} The PROGRAM parse tree.
 * @param {Lexeme *env} The environment in which to evaluate the program.
 * @returns {Lexeme *} The evaluation of the last definition.
 */
extern Lexeme *evalProgram(Lexeme *tree, Lexeme *env);

/**
 * Evaluates a MINUS parse tree. Returns a Lexeme containing the negative of
 * the cdr of the tree.
 * 
 * @param {Lexeme *tree} The MINUS parse tree.
 * @param {Lexeme *env} The environment in which to evaluate the number.
 * @returns {Lexeme *} A Lexeme representing the negative number.
 */
extern Lexeme *evalMinus(Lexeme *tree, Lexeme *env);

/**
 * Evaluates a COLON parse tree. Returns the result of inserting an id-value
 * pair into the environment.
 * 
 * @param {Lexeme *tree} The COLON parse tree.
 * @param {Lexeme *env} The environment in which to evaluate the assignment.
 * @returns {Lexeme *} The insertion of the id-value pair into the environment.
 */
extern Lexeme *evalAssign(Lexeme *tree, Lexeme *env);

/**
 * Evaluates an OBJECT parse tree. Returns the extended environment after
 * evaluating the ATTRLIST found at the car of the tree.
 * 
 * @param {Lexeme *tree} The OBJECT parse tree.
 * @param {Lexeme *env} The static environment.
 * @returns {Lexeme *} The extended environment representing the object.
 */
extern Lexeme *evalObject(Lexeme *tree, Lexeme *env);

/**
 * Evaluates an ATTRLIST parse tree. Returns a Lexeme corresponding to the
 * evaluation of the last definition, or NULL if none exist.
 * 
 * @param {Lexeme *list} The ATTRLIST parse tree.
 * @param {Lexeme *env} The environment in which to evaluate the attributes.
 * @returns {Lexeme *} The evaluation of the last definition | NULL.
 */
extern Lexeme *evalAttrList(Lexeme *list, Lexeme *env);

/**
 * Evaluates a MIXINLIST parse tree. Returns an ARRAY Lexeme containing a
 * non-null aval attribute.
 * 
 * @param {Lexeme *list} The MIXINLIST parse tree.
 * @param {Lexeme *env} The environment in which to evaluate the elements.
 * @returns {Lexeme *} The ARRAY Lexeme containing an array of Lexemes.
 */
extern Lexeme *evalArray(Lexeme *list, Lexeme *env);

/**
 * Prints information regarding why this may have been triggered and exits.
 * 
 * @param {Lexeme *tree} The parse tree that caused the error.
 * @param {Lexeme *env} The environment corresponding to the error.
 * @exits
 */
extern Lexeme *evalError(Lexeme *tree, Lexeme *env);

#endif