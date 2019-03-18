/**
 * Translator module created for translating WaltsonJS
 * code into NodeJS code for building a IBM Watson Chatbot.
 * This is similar to the pretty printer, except it prints
 * NodeJS code as opposed to duplicating the input code.
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

/*************************************/
/*              Globals              */
/*************************************/

#define null NULL

FILE *fp;
int numTabs;
int inObject;

/*************************************/
/*     Private Method Prototypes     */
/*************************************/

static void printTabs();

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
 * The main dispatching function for translating a WaltsonJS program into a
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
static void translate(Lexeme *tree);

/**
 * Translates a PROGRAM parse tree.
 * 
 * @param {Lexeme *tree} The PROGRAM parse tree.
 */
static void translateProgram(Lexeme *tree);

/**
 * Translates a MINUS parse tree.
 * 
 * @param {Lexeme *tree} The MINUS parse tree.
 */
static void translateMinus(Lexeme *tree);

/**
 * Translates a COLON parse tree.
 * 
 * @param {Lexeme *tree} The COLON parse tree.
 */
static void translateAssign(Lexeme *tree);

/**
 * Translates an OBJECT parse tree.
 * 
 * @param {Lexeme *tree} The OBJECT parse tree.
 */
static void translateObject(Lexeme *tree);

/**
 * Translates an ATTRLIST parse tree.
 * 
 * @param {Lexeme *list} The ATTRLIST parse tree.
 */
static void translateAttrList(Lexeme *list);

/**
 * Translates a MIXINLIST parse tree.
 * 
 * @param {Lexeme *list} The MIXINLIST parse tree.
 */
static void translateArray(Lexeme *list);

/**
 * Prints information regarding why this may have been triggered an exits.
 * 
 * @param {Lexeme *tree} The parse tree that caused an error.
 * @exits
 */
static void translateError(Lexeme *tree);

/*************************************/
/*            Main Method            */
/*************************************/

int main(int argc, char *argv[]) {
	assert(argc == 3);
	
	initParser(argv[1]);
	Lexeme *tree = parse();
	closeParser();

	fp = fopen(argv[2], "w");
	numTabs = 0;
	inObject = 0;
	translate(tree);
	fclose(fp);
	
	return 0;
}

/*************************************/
/*          Private Methods          */
/*************************************/

static void printTabs() {
	for (int i = 0; i < numTabs; i++)
		fprintf(fp, "\t");
}

static int same(char *left, char *right) {
	return !strcmp(left, right);
}

static void translate(Lexeme *tree) {
	if (tree == null)
		return;
	char *type = getType(tree);
	if (same(type, TRUEVAL)) fprintf(fp, "true");
	else if (same(type, FALSEVAL)) fprintf(fp, "false");
	else if (same(type, STRING)) fprintf(fp, "\"%s\"", getSval(tree));
	else if (same(type, INTEGER)) fprintf(fp, "%d", getIval(tree));
	else if (same(type, REAL)) fprintf(fp, "%lf", getRval(tree));
	else if (same(type, ID)) fprintf(fp, "%s", getSval(tree));
	else if (same(type, PROGRAM)) translateProgram(tree);
	else if (same(type, MINUS)) translateMinus(tree);
	else if (same(type, COLON)) translateAssign(tree);
	else if (same(type, OBJECT)) translateObject(tree);
	else if (same(type, ATTRLIST)) translateAttrList(tree);
	else if (same(type, MIXINLIST)) translateArray(tree);
	else translateError(tree);
}

static void translateProgram(Lexeme *tree) {
	while (tree != null) {
		printTabs();
		translate(car(tree));
		tree = cdr(tree);
		fprintf(fp, "\n");
	}
}

static void translateMinus(Lexeme *tree) {
	fprintf(fp, "-");
	translate(cdr(tree));
}

static void translateAssign(Lexeme *tree) {
	if (!inObject) fprintf(fp, "var ");
	translate(car(tree));
	if (inObject) fprintf(fp, ": ");
	else fprintf(fp, " = ");
	translate(cdr(tree));
}

static void translateObject(Lexeme *tree) {
	fprintf(fp, "{");
	if (car(tree) != null) {
		fprintf(fp, "\n");
		numTabs++;
		translate(car(tree));
		numTabs--;
		printTabs();
	}
	fprintf(fp, "}");
}

static void translateAttrList(Lexeme *tree) {
	while (tree != null) {
		inObject = 1;
		printTabs();
		translate(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	inObject = 0;
}

static void translateArray(Lexeme *tree) {
	fprintf(fp, "[\n");
	numTabs++;
	while (tree != null) {
		printTabs();
		translate(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	numTabs--;
	printTabs();
	fprintf(fp, "]");
}

static void translateError(Lexeme *tree) {
	fprintf(stderr, "Error: tried to print something with type %s!\n", getType(tree));
	exit(1);
}
