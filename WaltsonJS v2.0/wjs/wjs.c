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
	strftime(time_str, sizeof(time_str), "%Y.%m.%d.%H.%M.tmp", time_info);

	FILE *fp = initTranslator("temp.js");
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

static Lexeme *execute(Lexeme *env) {
	Lexeme *table = car(env);
	Lexeme *vars = car(table);
	Lexeme *vals = cdr(table);
	while (vars != null) {
		char *id = getSval(car(vars));
		//if (same(id, "children"))
	}
}
