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
#include "Evaluator.h"
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

/*************************************/
/*          Public Methods           */
/*************************************/

Lexeme *eval(Lexeme *tree, Lexeme *env) {
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

Lexeme *evalProgram(Lexeme *tree, Lexeme *env) {
	Lexeme *ret = null;
	while (tree != null) {
		ret = eval(car(tree), env);
		tree = cdr(tree);
	}
	return ret;
}

Lexeme *evalMinus(Lexeme *tree, Lexeme *env) {
	Lexeme *num = eval(cdr(tree), env);
	if (getType(num) == INTEGER)
		return newLexeme(INTEGER, -1 * getIval(num), UNDEFINED, UNDEFINED, getLine(tree));
	else
		return newLexeme(REAL, UNDEFINED, -1 * getRval(num), UNDEFINED, getLine(tree));
}

Lexeme *evalAssign(Lexeme *tree, Lexeme *env) {
	Lexeme *id = car(tree);
	Lexeme *result = eval(cdr(tree), env);
	return insert(env, id, result);
}

Lexeme *evalObject(Lexeme *tree, Lexeme *env) {
	Lexeme *xenv = extend(env, null, null);
	eval(car(tree), xenv);
	displayEnvironments(xenv, stdout);
	return xenv;
}

Lexeme *evalAttrList(Lexeme *list, Lexeme *env) {
	Lexeme *ret = null;
	while (list != null) {
		ret = eval(car(list), env);
		list = cdr(list);
	}
	return ret;
}

Lexeme *evalArray(Lexeme *list, Lexeme *env) {
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

Lexeme *evalError(Lexeme *tree, Lexeme *env) {
	fprintf(stderr, "Error: tried to evaluate something with type %s!\n", getType(tree));
	displayEnvironments(env, stderr);
	exit(1);
}

/*************************************/
/*          Private Methods          */
/*************************************/

static int same(char *left, char *right) {
	return !strcmp(left, right);
}
