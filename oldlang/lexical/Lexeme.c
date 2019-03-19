/**
 * Lexeme module created for Lexical items
 * corresponding to the grammar rules
 * laid out for generating NodeJS code.
 * 
 * Created by Walt Loumakis
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Lexeme.h"
#include "Types.h"

#define null NULL

/**
 * Structure definition for a Lexeme.
 */
struct lexeme {
	char *type;
	int ival;
	double rval;
	char *sval;
	int line;
	Lexeme *left;
	Lexeme *right;
};

/**************************************************************************/
/*                              Constructor                               */
/**************************************************************************/

Lexeme *newLexeme(char *type, int ival, double rval, char *sval, int line) {
	Lexeme *new = (Lexeme *)malloc(sizeof(Lexeme));
	assert(new != null);
	new->type = type;
	new->ival = ival;
	new->rval = rval;
	new->sval = sval;
	new->line = line;
	return new;
}

/**************************************************************************/
/*                             Display Method                             */
/**************************************************************************/

static int isBad(char *type) {
	if (strcmp(type, BAD_TYPE) == 0)
		return 1;
	return 0;
}

static void DieWithError(Lexeme *l) {
	fprintf(stderr, "Error on line %d: Lexeme with value \"%s\" was unrecognized.\n", l->line, l->sval);
	exit(1);
}

void displayLexeme(FILE *fp, Lexeme *l) {
	if (isBad(getType(l)))
		DieWithError(l);
	fprintf(fp, "%s", getType(l));
	if (strcmp(getType(l), STRING) == 0)
		fprintf(fp, " %s", getSval(l));
	else if (strcmp(getType(l), NUMBER) == 0)
		fprintf(fp, " %d", getIval(l));
	fprintf(fp, "\n");
}

/**************************************************************************/
/*                            Accessor Methods                            */
/**************************************************************************/

char *getType(Lexeme *l) {
	return l->type;
}

int getIval(Lexeme *l) {
	return l->ival;
}

double getRval(Lexeme *l) {
	return l->rval;
}

char *getSval(Lexeme *l) {
	return l->sval;
}

int getLine(Lexeme *l) {
	return l->line;
}

/**************************************************************************/
/*                            Mutator Methods                             */
/**************************************************************************/

char *setType(Lexeme *l, char *newType) {
	char *ret = l->type;
	l->type = newType;
	return ret;
}

int setIval(Lexeme *l, int newIval) {
	int ret = l->ival;
	l->ival = newIval;
	return ret;
}

double setRval(Lexeme *l, double newRval) {
	double ret = l->rval;
	l->rval = newRval;
	return ret;
}

char *setSval(Lexeme *l, char *newSval) {
	char *ret = l->sval;
	l->sval = newSval;
	return ret;
}

int setLine(Lexeme *l, int newLine) {
	int ret = l->line;
	l->line = newLine;
	return ret;
}

/**************************************************************************/
/*                           Parse Tree Methods                           */
/**************************************************************************/

Lexeme *cons(char *type, Lexeme *left, Lexeme *right) {
	Lexeme *root = newLexeme(type, UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED);
	root->left = left;
	root->right = right;
	return root;
}

Lexeme *car(Lexeme *l) {
	return l->left;
}

Lexeme *cdr(Lexeme *l) {
	return l->right;
}

Lexeme *setCar(Lexeme *l, Lexeme *left) {
	Lexeme *ret = l->left;
	l->left = left;
	return ret;
}

Lexeme *setCdr(Lexeme *l, Lexeme *right) {
	Lexeme *ret = l->right;
	l->right = right;
	return ret;
}
