/**
 * Parser module created adapted from
 * recognizer module. This parser, in
 * addition to finding syntax errors,
 * will build a parse tree.
 * 
 * Created by Walt Loumakis
 */

#include <stdlib.h>
#include <string.h>
#include "Lexer.h"
#include "Types.h"
#include "Lexeme.h"
#include "Parser.h"

/*************************************/
/*              Globals              */
/*************************************/

#define null NULL

Lexeme *current;

/*************************************/
/*     Private Method Prototypes     */
/*************************************/

static int check(char *type);
static Lexeme *match(char *type);
static Lexeme *advance();

static Lexeme *program();
static Lexeme *definition();
static Lexeme *unary();
static Lexeme *number();
static Lexeme *nonnegNum();
static Lexeme *negNum();
static Lexeme *object();
static Lexeme *optAttrList();
static Lexeme *attrList();
static Lexeme *array();
static Lexeme *mixinList();
static Lexeme *mixin();

/* Pending functions */

static int definitionPending();
static int numberPending();
static int nonnegNumPending();
static int negNumPending();
static int objectPending();
static int attrListPending();

/*************************************/
/*          Public Methods           */
/*************************************/

void initParser(char *filename) {
	initLexer(filename);
}

void closeParser() {
	closeLexer();
}

Lexeme *parse() {
	current = lex();
	Lexeme *tree = program();
	freeLexeme(match(ENDofINPUT));
	return tree;
}

void freeParseTree(Lexeme *tree) {
	if (tree == null)
		return;
	freeParseTree(car(tree));
	freeParseTree(cdr(tree));
	freeLexeme(tree);
}

/*************************************/
/*          Private Methods          */
/*************************************/

static int check(char *type) {
	return !strcmp(type, getType(current));
}

static Lexeme *match(char *type) {
	if (check(type))
		return advance();
	fprintf(stderr, "Error on line %d: Expected %s, got %s\n", getLine(current), type, getType(current));
	printf("illegal\n");
	exit(1);
}

static Lexeme *advance() {
	Lexeme *prev = current;
	if (!check(ENDofINPUT))
		current = lex();
	return prev;
}

static Lexeme *program() {
	Lexeme *a, *b;
	a = definition();
	if (check(COMMA)) {
		freeLexeme(advance());
		b = program();
	}
	else
		b = null;
	return cons(PROGRAM, a, b);
}

static Lexeme *definition() {
	Lexeme *a;
	a = match(ID);
	freeLexeme(match(COLON));
	return cons(COLON, a, unary());
}

static Lexeme *unary() {
	if (check(TRUEVAL) ||
		check(FALSEVAL) ||
		check(STRING))
			return advance();
	else if (numberPending())
		return number();
	else if (objectPending())
		return object();
	else
		return array();
}

static Lexeme *number() {
	if (nonnegNumPending())
		return nonnegNum();
	else
		return negNum();
}

static Lexeme *nonnegNum() {
	if (check(INTEGER))
		return advance();
	else
		return advance();
}

static Lexeme *negNum() {
	freeLexeme(match(MINUS));
	return cons(MINUS, null, nonnegNum());
}

static Lexeme *object() {
	Lexeme *a;
	freeLexeme(match(OBRACE));
	a = optAttrList();
	freeLexeme(match(CBRACE));
	return cons(OBJECT, a, null);
}

static Lexeme *optAttrList() {
	if (attrListPending())
		return attrList();
	return null;
}

static Lexeme *attrList() {
	Lexeme *a, *b;
	a = definition();
	if (check(COMMA)) {
		freeLexeme(advance());
		b = attrList();
	}
	else
		b = null;
	return cons(ATTRLIST, a, b);
}

static Lexeme *array() {
	Lexeme *a;
	freeLexeme(match(OBRACKET));
	a = mixinList();
	freeLexeme(match(CBRACKET));
	return a;
}

static Lexeme *mixinList() {
	Lexeme *a, *b;
	a = mixin();
	if (check(COMMA)) {
		freeLexeme(advance());
		b = mixinList();
	}
	else
		b = null;
	return cons(MIXINLIST, a, b);
}

static Lexeme *mixin() {
	if (definitionPending())
		return definition();
	else
		return object();
}

/* Pending functions */

static int definitionPending() {
	return check(ID);
}

static int numberPending() {
	return (nonnegNumPending() ||
			negNumPending());
}

static int nonnegNumPending() {
	return (check(INTEGER) ||
			check(REAL));
}

static int negNumPending() {
	return check(MINUS);
}

static int objectPending() {
	return check(OBRACE);
}

static int attrListPending() {
	return definitionPending();
}
