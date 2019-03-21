/**
 * Environments module created for
 * scoping purposes.
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

Lexeme *create() {
	return cons(ENV, cons(TABLE, null, null), null);
}

Lexeme *insert(Lexeme *env, Lexeme *var, Lexeme *val) {
	Lexeme *table = car(env);
	setCar(table, cons(IDLIST, var, car(table)));
	setCdr(table, cons(VALLIST, val, cdr(table)));
	return val;
}

static int sameVariable(Lexeme *left, Lexeme *right) {
	return !strcmp(getSval(left), getSval(right));
}

Lexeme *lookup(Lexeme *env, Lexeme *var) {
	while (env != null) {
		Lexeme *table = car(env);
		Lexeme *vars = car(table);
		Lexeme *vals = cdr(table);
		while (vars != null) {
			if (sameVariable(car(vars), var))
				return car(vals);
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(env);
	}
	fprintf(stderr, "Could not find %s within scope!\n", getSval(var));
	exit(1);
}

int exists(Lexeme *env, Lexeme *var) {
	while (env != null) {
		Lexeme *table = car(env);
		Lexeme *vars = car(table);
		Lexeme *vals = cdr(table);
		while (vars != null) {
			if (sameVariable(car(vars), var))
				return 1;
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(env);
	}
	return 0;
}

Lexeme *update(Lexeme *env, Lexeme *var, Lexeme *newVal) {
	while (env != null) {
		Lexeme *table = car(env);
		Lexeme *vars = car(table);
		Lexeme *vals = cdr(table);
		while (vars != null) {
			if (sameVariable(car(vars), var))
				return setCar(vals, newVal);
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(env);
	}
	fprintf(stderr, "Could not find %s within scope!\n", getSval(var));
	exit(-1);
}

static Lexeme *deleteHelper(Lexeme *vars, Lexeme *vals, Lexeme *table) {
	/* Case 1: The var is found at the head of the list. */
	if (vars == null) {
		Lexeme *retVar = car(table);
		Lexeme *retVal = cdr(table);
		setCar(table, cdr(retVar));
		setCdr(table, cdr(retVal));
		
		setCdr(retVar, null);
		setCdr(retVal, null);

		freeLexeme(car(retVar));
		freeLexeme(retVar);

		Lexeme *ret = car(retVal);
		freeLexeme(retVal);
		return ret;
	}

	/* Case 2: We found the var in delete and are looking at prevVar. */
	Lexeme *retVar = cdr(vars);
	Lexeme *retVal = cdr(vals);

	setCdr(vars, cdr(retVar));
	setCdr(vals, cdr(retVal));

	setCdr(retVar, null);
	setCdr(retVal, null);

	freeLexeme(car(retVar));
	freeLexeme(retVar);

	Lexeme *ret = car(retVal);
	freeLexeme(retVal);
	return ret;
}

Lexeme *delete(Lexeme *env, Lexeme *var) {
	while (env != null) {
		Lexeme *table = car(env);
		Lexeme *vars = car(table);
		Lexeme *vals = cdr(table);
		Lexeme *prevVar = null;
		Lexeme *prevVal = null;
		while (vars != null) {
			if (sameVariable(car(vars), var))
				return deleteHelper(prevVar, prevVal, table);
			prevVar = vars;
			prevVal = vals;
			vars = cdr(vars);
			vals = cdr(vals);
		}
		env = cdr(env);
	}
	fprintf(stderr, "Could not find %s within scope!\n", getSval(var));
	exit(-2);
}

Lexeme *extend(Lexeme *env, Lexeme *vars, Lexeme *vals) {
	return cons(ENV, cons(TABLE, vars, vals), env);
}

void displayEnvironment(Lexeme *env, FILE *fp) {
	if (env == null) return;
	Lexeme *table = car(env);
	Lexeme *vars = car(table);
	Lexeme *vals = cdr(table);
	fprintf(fp, "The local environment is. . .\n");
	while (vars != null) {
		fprintf(fp, "var %s = ", getSval(car(vars)));
		displayLexeme(car(vals), fp);
		vars = cdr(vars);
		vals = cdr(vals);
	}
	fprintf(fp, "------------------------------\n");
}

void displayEnvironments(Lexeme *env, FILE *fp) {
	fprintf(fp, "----Displaying all environments----\n");
	while (env != null) {
		displayEnvironment(env, fp);
		env = cdr(env);
	}
}

void freeEnv(Lexeme *env) {
	Lexeme *table = car(env);

	Lexeme *prevVar = car(table);
	Lexeme *vars = null;

	Lexeme *prevVal = cdr(table);
	Lexeme *vals = null;

	while (prevVar != null) {
		vars = cdr(prevVar);
		vals = cdr(prevVal);
		if (car(prevVar) != null)
			freeLexeme(car(prevVar));
		if (car(prevVal) != null)
			freeLexeme(car(prevVal));
		freeLexeme(prevVar);
		freeLexeme(prevVal);
		prevVar = vars;
		prevVal = vals;
	}
	freeLexeme(table);
	freeLexeme(env);
}

