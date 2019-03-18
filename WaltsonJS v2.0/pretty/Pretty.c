/**
 * Pretty printer module created to
 * pretty print a parse tree into
 * a given file, given a WaltsonJS file.
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

/*************************************/
/*     Private Method Prototypes     */
/*************************************/

static void printTabs();
static int same(char *left, char *right);

static void pp(Lexeme *tree);
static void printProgram(Lexeme *tree);
static void printMinus(Lexeme *tree);
static void printAssign(Lexeme *tree);
static void printObject(Lexeme *tree);
static void printAttrList(Lexeme *tree);
static void printArray(Lexeme *tree);
static void printError(Lexeme *tree);

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
	pp(tree);
	fclose(fp);
	freeParseTree(tree);
	return 0;
}

/*************************************/
/*          Private Methods          */
/*************************************/

static int same(char *left, char *right) {
	return !strcmp(left, right);
}

static void printTabs() {
	for (int i = 0; i < numTabs; i++)
		fprintf(fp, "\t");
}

static void pp(Lexeme *tree) {
	if (tree == null)
		return;
	char *type = getType(tree);
	if (same(type, TRUEVAL)) fprintf(fp, "true");
	else if (same(type, FALSEVAL)) fprintf(fp, "false");
	else if (same(type, STRING)) fprintf(fp, "\"%s\"", getSval(tree));
	else if (same(type, INTEGER)) fprintf(fp, "%d", getIval(tree));
	else if (same(type, REAL)) fprintf(fp, "%lf", getRval(tree));
	else if (same(type, ID)) fprintf(fp, "%s", getSval(tree));
	else if (same(type, PROGRAM)) printProgram(tree);
	else if (same(type, MINUS)) printMinus(tree);
	else if (same(type, COLON)) printAssign(tree);
	else if (same(type, OBJECT)) printObject(tree);
	else if (same(type, ATTRLIST)) printAttrList(tree);
	else if (same(type, MIXINLIST)) printArray(tree);
	else printError(tree);
}

static void printProgram(Lexeme *tree) {
	while (tree != null) {
		printTabs();
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printMinus(Lexeme *tree) {
	fprintf(fp, "-");
	pp(cdr(tree));
}

static void printAssign(Lexeme *tree) {
	pp(car(tree));
	fprintf(fp, ": ");
	pp(cdr(tree));
}

static void printObject(Lexeme *tree) {
	fprintf(fp, "{");
	if (car(tree) != null) {
		fprintf(fp, "\n");
		numTabs++;
		pp(car(tree));
		numTabs--;
		printTabs();
	}
	fprintf(fp, "}");
}

static void printAttrList(Lexeme *tree) {
	while (tree != null) {
		printTabs();
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printArray(Lexeme *tree) {
	fprintf(fp, "[\n");
	numTabs++;
	while (tree != null) {
		printTabs();
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	numTabs--;
	printTabs();
	fprintf(fp, "]");
}

static void printError(Lexeme *tree) {
	fprintf(stderr, "Error: tried to print something with type %s!\n");
	exit(1);
}
