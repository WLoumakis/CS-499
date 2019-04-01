/**
 * Recognizer module created for
 * recognizing syntax errors within
 * a WaltsonJS file.
 * 
 * Created by Walt Loumakis
 */

#include <stdlib.h>
#include <string.h>
#include "Lexer.h"
#include "Types.h"
#include "Lexeme.h"
#include "Recognizer.h"

/*************************************/
/*              Globals              */
/*************************************/

Lexeme *current;

/*************************************/
/*     Private Method Prototypes     */
/*************************************/

static int check(char *type);
static Lexeme *match(char *type);
static Lexeme *advance();

static void program();
static void definition();
static void unary();
static void number();
static void nonnegNum();
static void negNum();
static void object();
static void optProgram();
static void array();
static void mixinList();
static void mixin();

/* Pending functions */

static int programPending();
static int definitionPending();
static int numberPending();
static int nonnegNumPending();
static int negNumPending();
static int objectPending();

/*************************************/
/*          Public Methods           */
/*************************************/

void initRecognizer(char *filename) {
	initLexer(filename);
}

void closeRecognizer() {
	closeLexer();
}

void recognize() {
	current = lex();
	program();
	printf("legal\n");
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
	current = lex();
	return prev;
}


static void program() {
	definition();
	if (check(COMMA)) {
		freeLexeme(advance());
		program();
	}
}

static void definition() {
	freeLexeme(match(ID));
	freeLexeme(match(COLON));
	unary();
}

static void unary() {
	if (check(TRUEVAL) ||
		check(FALSEVAL) ||
		check(STRING))
			freeLexeme(advance());
	else if (numberPending())
		number();
	else if (objectPending())
		object();
	else
		array();
}

static void number() {
	if (nonnegNumPending())
		nonnegNum();
	else
		negNum();
}

static void nonnegNum() {
	if (check(INTEGER))
		match(INTEGER);
	else
		match(REAL);
}

static void negNum() {
	freeLexeme(match(MINUS));
	if (check(INTEGER))
		match(INTEGER);
	else
		match(REAL);
}

static void object() {
	freeLexeme(match(OBRACE));
	optProgram();
	freeLexeme(match(CBRACE));
}

static void optProgram() {
	if (programPending())
		program();
}

static void array() {
	freeLexeme(match(OBRACKET));
	mixinList();
	freeLexeme(match(CBRACKET));
}

static void mixinList() {
	mixin();
	if (check(COMMA)) {
		freeLexeme(advance());
		mixinList();
	}
}

static void mixin() {
	if (definitionPending())
		definition();
	else
		object();
}

/* Pending functions */

static int programPending() {
	return definitionPending();
}

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
