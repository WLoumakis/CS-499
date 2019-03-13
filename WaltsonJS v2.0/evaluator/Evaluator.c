/**
 * Evaluator module created to
 * evalue a parse tree created
 * by parsing a given WaltsonJS file.
 * 
 * Created by Walt Loumakis
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Types.h"
#include "Lexeme.h"
#include "Parser.h"
#include "Environments.h"

/*************************************/
/*              Globals              */
/*************************************/

#define null NULL

/*************************************/
/*     Private Method Prototypes     */
/*************************************/

/**
 * Boolean evaluation that checks if the two strings are the same. Returns 0 if
 * the two strings are not equivalent, non-zero otherwise.
 * 
 * @param {char *left} The left string.
 * @param {char *right} The right string.
 * @returns {int} 0 if the two strings are not equal; non-zero otherwise.
 */
static int same(char *left, char *right);

/**
 * The main dispatching function for evaluation of a WaltsonJS program.
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
static Lexeme *eval(Lexeme *tree, Lexeme *env);

/**
 * Evaluates a PROGRAM parse tree. Returns a Lexeme corresponding to the
 * evaluation of the last definition.
 * 
 * @param {Lexeme *tree} The PROGRAM parse tree.
 * @param {Lexeme *env} The environment in which to evaluate the program.
 * @returns {Lexeme *} The evaluation of the last definition.
 */
static Lexeme *evalProgram(Lexeme *tree, Lexeme *env);

/**
 * Evaluates a MINUS parse tree. Returns a Lexeme containing the negative of
 * the cdr of the tree.
 * 
 * @param {Lexeme *tree} The MINUS parse tree.
 * @param {Lexeme *env} The environment in which to evaluate the number.
 * @returns {Lexeme *} A Lexeme representing the negative number.
 */
static Lexeme *evalMinus(Lexeme *tree, Lexeme *env);

/**
 * Evaluates a COLON parse tree. Returns the result of inserting an id-value
 * pair into the environment.
 * 
 * @param {Lexeme *tree} The COLON parse tree.
 * @param {Lexeme *env} The environment in which to evaluate the assignment.
 * @returns {Lexeme *} The insertion of the id-value pair into the environment.
 */
static Lexeme *evalAssign(Lexeme *tree, Lexeme *env);

/**
 * Evaluates an OBJECT parse tree. Returns the extended environment after
 * evaluating the ATTRLIST found at the car of the tree.
 * 
 * @param {Lexeme *tree} The OBJECT parse tree.
 * @param {Lexeme *env} The static environment.
 * @returns {Lexeme *} The extended environment representing the object.
 */
static Lexeme *evalObject(Lexeme *tree, Lexeme *env);

/**
 * Evaluates an ATTRLIST parse tree. Returns a Lexeme corresponding to the
 * evaluation of the last definition, or NULL if none exist.
 * 
 * @param {Lexeme *list} The ATTRLIST parse tree.
 * @param {Lexeme *env} The environment in which to evaluate the attributes.
 * @returns {Lexeme *} The evaluation of the last definition | NULL.
 */
static Lexeme *evalAttrList(Lexeme *list, Lexeme *env);

/**
 * Evaluates a MIXINLIST parse tree. Returns an ARRAY Lexeme containing a
 * non-null aval attribute.
 * 
 * @param {Lexeme *list} The MIXINLIST parse tree.
 * @param {Lexeme *env} The environment in which to evaluate the elements.
 * @returns {Lexeme *} The ARRAY Lexeme containing an array of Lexemes.
 */
static Lexeme *evalArray(Lexeme *list, Lexeme *env);

/**
 * Prints information regarding why this may have been triggered and exits.
 * 
 * @param {Lexeme *tree} The parse tree that caused the error.
 * @param {Lexeme *env} The environment corresponding to the error.
 * @exits
 */
static Lexeme *evalError(Lexeme *tree, Lexeme *env);

/*************************************/
/*            Main Method            */
/*************************************/

int main(int argc, char *argv[]) {
	assert(argc == 2);
	initParser(argv[1]);
	Lexeme *tree = parse();
	closeParser();

	Lexeme *global = create();

	eval(tree, global);
	freeParseTree(tree);
	return 0;
}

/*************************************/
/*          Private Methods          */
/*************************************/

static int same(char *left, char *right) {
	return !strcmp(left, right);
}

static Lexeme *eval(Lexeme *tree, Lexeme *env) {
	if (tree == null)
		return null;
	char *type = getType(tree);
	if (same(type, TRUEVAL)) return tree;
	else if (same(type, FALSEVAL)) return tree;
	else if (same(type, STRING)) return tree;
	else if (same(type, INTEGER)) return tree;
	else if (same(type, REAL)) return tree;
	else if (same(type, ID)) return lookup(env, tree);
	else if (same(type, PROGRAM)) return evalProgram(tree, env);
	else if (same(type, MINUS)) return evalMinus(tree, env);
	else if (same(type, COLON)) return evalAssign(tree, env);
	else if (same(type, OBJECT)) return evalObject(tree, env);
	else if (same(type, ATTRLIST)) return evalAttrList(tree, env);
	else if (same(type, MIXINLIST)) return evalArray(tree, env);
	else return evalError(tree, env);
}

static Lexeme *evalProgram(Lexeme *tree, Lexeme *env) {
	Lexeme *ret = null;
	while (tree != null) {
		ret = eval(car(tree), env);
		tree = cdr(tree);
	}
	return ret;
}

static Lexeme *evalMinus(Lexeme *tree, Lexeme *env) {
	Lexeme *num = eval(cdr(tree), env);
	if (getType(num) == INTEGER)
		return newLexeme(INTEGER, -1 * getIval(num), UNDEFINED, UNDEFINED, getLine(tree));
	else
		return newLexeme(REAL, UNDEFINED, -1 * getRval(num), UNDEFINED, getLine(tree));
}

static Lexeme *evalAssign(Lexeme *tree, Lexeme *env) {
	Lexeme *id = car(tree);
	Lexeme *result = eval(cdr(tree), env);
	return insert(env, id, result);
}

static Lexeme *evalObject(Lexeme *tree, Lexeme *env) {
	Lexeme *xenv = extend(env, null, null);
	eval(car(tree), xenv);
	displayEnvironments(xenv, stdout);
	return xenv;
}

static Lexeme *evalAttrList(Lexeme *list, Lexeme *env) {
	Lexeme *ret = null;
	while (list != null) {
		ret = eval(car(list), env);
		list = cdr(list);
	}
	return ret;
}

static Lexeme *evalArray(Lexeme *list, Lexeme *env) {
	Lexeme *temp = list;
	int count = 0;
	while (temp != null) {
		temp = cdr(temp);
		count++;
	}
	Lexeme *new = newLexeme(ARRAY, UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED);
	newArray(new, count);
	count = 0;
	while (list != null) {
		setAval(new, count++, eval(car(list), env));
		list = cdr(list);
	}
	return new;
}

static Lexeme *evalError(Lexeme *tree, Lexeme *env) {
	fprintf(stderr, "Error: tried to evaluate something with type %s!\n", getType(tree));
	exit(1);
}
