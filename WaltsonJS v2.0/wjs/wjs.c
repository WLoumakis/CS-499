/**
 * Main module for translation and
 * evaluation of WaltsonJS code.
 * 
 * Created by Walt Loumakis.
 */

#include <time.h>
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include "Types.h"
#include "Lexeme.h"
#include "Parser.h"
#include "Evaluator.h"
#include "Translator.h"
#include "Environments.h"

/*************************************/
/*              Globals              */
/*************************************/

#define null NULL

/*************************************/
/*     Private Method Prototypes     */
/*************************************/

static int same(char *left, char *right);

static Lexeme *execute(Lexeme *env);
static Lexeme *execChildren(char *parent, Lexeme *children);

/*************************************/
/*            Main Method            */
/*************************************/

int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: wjs <infile.wjs>\n");
		exit(1);
	}

	initParser(argv[1]);
	Lexeme *tree = parse();
	closeParser();

	time_t cur_time;
	time(&cur_time);

	struct tm * time_info = localtime(&cur_time);

	char time_str[33] = {0};
	strftime(time_str, sizeof(time_str), "%Y.%m.%d.%H.%M.wjs.tmp", time_info);

	FILE *fp = initTranslator(time_str);
	Lexeme *global = create();

	eval(tree, global);

	execute(global);

	closeTranslator();
	return 0;
}

/*************************************/
/*          Private Methods          */
/*************************************/

static int same(char *left, char *right) {
	return !strcmp(left, right);
}

//TODO: The recursive descent for children is a bit tricky.
//		Essentially, I need to include all of the attributes of a given tree
//		node EXCEPT for the children attribute. Once all nodes at a level are
//		translated to NodeJS, I need to execute that script, then rewrite the
//		boilerplate code and add all of the next children. Howto?
static Lexeme *execute(Lexeme *env) {
	Lexeme *table = car(env);
	Lexeme *vars = car(table);
	Lexeme *vals = cdr(table);
	Lexeme *dialog_node;
	while (vars != null) {
		char *id = getSval(car(vars));
		if (same(id, "dialog_node")) dialog_node = car(vals);
		else if (same(id, "children")) return execChildren(dialog_node, car(vals));
	}
}

static Lexeme *execChildren(Lexeme *parent, Lexeme *children) {

}
