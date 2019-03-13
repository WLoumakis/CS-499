/**
 * Lexeme module created for Lexical items
 * corresponding to the grammar rules
 * laid out for generating NodeJS code.
 * 
 * Created by Walt Loumakis
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "Types.h"
#include "Lexeme.h"

#define null NULL

/**
 * Structure definition for a Lexeme.
 */
struct lexeme {
	char *type;
	int ival;
	double rval;
	char *sval;
	Lexeme **aval;
	int avalSize;
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
	new->aval = null;
	new->avalSize = 0;
	new->left = null;
	new->right = null;
	return new;
}

/**************************************************************************/
/*                             Display Method                             */
/**************************************************************************/

static int isBad(char *type) {
	if (strcmp(type, BAD_NUMBER) == 0 ||
		strcmp(type, UNKNOWN_CHAR) == 0)
		return 1;
	return 0;
}

static int printErrorMsg(Lexeme *l) {
	fprintf(stderr, "Error on line %d: %s", l->line, l->type);
	if (strcmp(l->type, UNKNOWN_CHAR) == 0)
		fprintf(stderr, " '%s'", getSval(l));
	fprintf(stderr, "\n");
	return 1;
}

int displayLexeme(Lexeme *l, FILE *fp) {
	if (strcmp(l->type, STRING) == 0)
		fprintf(fp, "STRING \"%s\"", getSval(l));
	else if (strcmp(l->type, ID) == 0)
		fprintf(fp, "ID %s", getSval(l));
	else if (strcmp(l->type, INTEGER) == 0)
		fprintf(fp, "INTEGER %d", getIval(l));
	else if (strcmp(l->type, REAL) == 0)
		fprintf(fp, "REAL %lf", getRval(l));
	else if (isBad(l->type))
		return printErrorMsg(l);
	else
		fprintf(fp, "%s", l->type);
	fprintf(fp, "\n");
	return 0;
}

/**************************************************************************/
/*                             Freeing Method                             */
/**************************************************************************/

extern void freeLexeme(Lexeme *l) {
	if (strcmp(getType(l), STRING) == 0 ||
		strcmp(getType(l), ID) == 0 ||
		isBad(l->type)) {
		free(l->sval);
	}
	free(l);
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

Lexeme *getAval(Lexeme *l, int index) {
	assert(index >= 0 && index < l->avalSize);
	return l->aval[index];
}

int getAvalSize(Lexeme *l) {
	return l->avalSize;
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

Lexeme *setAval(Lexeme *l, int index, Lexeme *new) {
	assert(index >= 0 && index < l->avalSize);
	Lexeme *ret = l->aval[index];
	l->aval[index] = new;
	return ret;
}

int setAvalSize(Lexeme *l, int newAvalSize) {
	int ret = l->avalSize;
	l->avalSize = newAvalSize;
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
