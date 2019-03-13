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

static int same(char *left, char *right);

static Lexeme *eval(Lexeme *tree, Lexeme *env);
static Lexeme *evalProgram(Lexeme *tree, Lexeme *env);
static Lexeme *evalMinus(Lexeme *tree, Lexeme *env);
static Lexeme *evalAssign(Lexeme *tree, Lexeme *env);
static Lexeme *evalObject(Lexeme *tree, Lexeme *env);
static Lexeme *evalAttrList(Lexeme *tree, Lexeme *env);
static Lexeme *evalArray(Lexeme *tree, Lexeme *env);

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
	else return evalArray(tree, env);
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

static Lexeme *evalAttrList(Lexeme *tree, Lexeme *env) {
	Lexeme *ret = null;
	while (tree != null) {
		ret = eval(car(tree), env);
		tree = cdr(tree);
	}
	return ret;
}

static Lexeme *evalArray(Lexeme *tree, Lexeme *env) {
	Lexeme *temp = tree;
	int count = 0;
	while (temp != null) {
		temp = cdr(temp);
		count++;
	}
	Lexeme *new = newLexeme(ARRAY, UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED);
	newArray(new, count);
	count = 0;
	while (tree != null) {
		setAval(new, count++, eval(car(tree), env));
		tree = cdr(tree);
	}
	return new;
}
