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
	return null;
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
	return null;
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

