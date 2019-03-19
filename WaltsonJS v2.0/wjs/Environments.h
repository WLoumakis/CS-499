/**
 * Environments module created for scoping
 * 
 * Created by Walt Loumakis
 */

#ifndef __MY_ENVIRONMENTS__
#define __MY_ENVIRONMENTS__

#include <stdio.h>
#include "Lexeme.h"

/**
 * Creates a new, empty environment.
 * 
 * @returns {Lexeme *} The environment created.
 */
extern Lexeme *create();

/**
 * Inserts a var-val pair into the environment.
 * NOTE: Inserts at the front of the table.
 * 
 * @param {Lexeme *env} The environment to insert into.
 * @param {Lexeme *var} The variable to insert.
 * @param {Lexeme *val} The value corresponding to the var.
 * @returns {Lexeme *} The value inserted.
 */
extern Lexeme *insert(Lexeme *env, Lexeme *var, Lexeme *val);

/**
 * Looks up the value of a variable within scope of the environment.
 * 
 * @param {Lexeme *env} The most local environment to search.
 * @param {Lexeme *var} The variable to search for.
 * @returns {Lexeme * | NULL} The value Lexeme if found; NULL otherwise.
 */
extern Lexeme *lookup(Lexeme *env, Lexeme *var);

/**
 * Updates a variable's value within the environment.
 * 
 * @param {Lexeme *env} The environment to update in.
 * @param {Lexeme *var} The variable whose value needs updating.
 * @param {Lexeme *newVal} The new value for the variable.
 * @returns {Lexeme * | NULL} The old value for var if found; NULL otherwise.
 */
extern Lexeme *update(Lexeme *env, Lexeme *var, Lexeme *newVal);

/**
 * Extends an environment with vars and vals.
 * 
 * @param {Lexeme *env} The environment to extend.
 * @param {Lexeme *vars} List of variables to insert into the extended environment.
 * @param {Lexeme *vals} List of values corresponding to the vars list.
 * @returns {Lexeme *} The extended environment.
 */
extern Lexeme *extend(Lexeme *env, Lexeme *vars, Lexeme *vals);

/**
 * Displays the passed in environment
 * to the passed in file.
 * 
 * @param {Lexeme *env} The environment to display.
 * @param {FILE *fp} The file to display to.
 */
extern void displayEnvironment(Lexeme *env, FILE *fp);

/**
 * Displays all environments within scope of
 * the passed in environment to the passed in file.
 * 
 * @param {Lexeme *env} The most local environment.
 * @param {FILE *fp} The file to display to.
 */
extern void displayEnvironments(Lexeme *env, FILE *fp);

/**
 * Frees all variables and values within the environment,
 * and the environment itself.
 * 
 * @param {Lexeme *env} The environment to free.
 */
extern void freeEnv(Lexeme *env);

#endif

