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

/*************************************/
/*          Public Methods           */
/*************************************/

void initTranslator(char *filename) {
	fp = fopen(filename, "w");
	assert(fp != null);
}

void closeTranslator() {
	fclose(fp);
}

void translate(Lexeme *tree) {
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

void translateProgram(Lexeme *tree) {
	while (tree != null) {
		printTabs();
		translate(car(tree));
		tree = cdr(tree);
		fprintf(fp, "\n");
	}
}

void translateMinus(Lexeme *tree) {
	fprintf(fp, "-");
	translate(cdr(tree));
}

void translateAssign(Lexeme *tree) {
	if (!inObject) fprintf(fp, "var ");
	translate(car(tree));
	if (inObject) fprintf(fp, ": ");
	else fprintf(fp, " = ");
	translate(cdr(tree));
}

void translateObject(Lexeme *tree) {
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

void translateAttrList(Lexeme *tree) {
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

void translateArray(Lexeme *tree) {
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

void translateError(Lexeme *tree) {
	fprintf(stderr, "Error: tried to print something with type %s!\n", getType(tree));
	exit(1);
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
