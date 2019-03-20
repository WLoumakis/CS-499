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
#define SIZE 33

FILE *fp;

/*************************************/
/*     Private Method Prototypes     */
/*************************************/

static int same(char *left, char *right);

static char *createTemp(char *buffer);

static Lexeme *execAll(Lexeme *env);

static Lexeme *execIntents(Lexeme *env);

static Lexeme *execEntities(Lexeme *env);

static Lexeme *execTree(Lexeme *env);
static Lexeme *execChildren(Lexeme *parent, Lexeme *children);

static void addGivens();
static void addWatson();
static void addAssistant();

static void addEnvVariables(Lexeme *env);

/*************************************/
/*            Main Method            */
/*************************************/

//IDEA: Create n different NodeJS files, then make a shell-script that loops
//		from 1 to n doing ./nodejs $i.js
int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: wjs <infile.wjs>\n");
		exit(1);
	}

	initParser(argv[1]);
	Lexeme *tree = parse();
	closeParser();

	char buffer[SIZE] = {0};
	createTemp(buffer);

	fp = initTranslator(buffer);
	Lexeme *global = create();

	eval(tree, global);

	addEnvVariables(global);
	addGivens();

	//execAll(global);

	closeTranslator();
	return 0;
}

/*************************************/
/*          Private Methods          */
/*************************************/

static int same(char *left, char *right) {
	return !strcmp(left, right);
}

static char *createTemp(char *buffer) {
	time_t cur_time;
	time(&cur_time);

	struct tm * time_info = localtime(&cur_time);
	strftime(buffer, (sizeof(char) * SIZE), "%Y.%m.%d.%H.%M.wjs.tmp", time_info);
	return buffer;
}

//TODO: The recursive descent for children is a bit tricky.
//		Essentially, I need to include all of the attributes of a given tree
//		node EXCEPT for the children attribute. Once all nodes at a level are
//		translated to NodeJS, I need to execute that script, then rewrite the
//		boilerplate code and add all of the next children. Howto?
static Lexeme *execute(Lexeme *env) {
	addGivens();
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

static void addGivens() {
	addWatson();
	addAssistant();
}

static void addWatson() {
	fprintf(fp, "var watson = require('watson-developer-cloud');\n");
}

static void addAssistant() {
	fprintf(fp, "var assistant = new watson.AssistantV1({\n");
	fprintf(fp, "\tiam_apikey: iam_apikey,\n");
	fprintf(fp, "\tversion: version,\n");
	fprintf(fp, "\turl: url\n");
	fprintf(fp, "});\n");
}

static void addEnvVariables(Lexeme *env) {
	Lexeme *table = car(env);
	Lexeme *vars = car(table);
	Lexeme *vals = cdr(table);
	while(vars != null) {
		fprintf(fp, "var ");
		translate(car(vars));
		fprintf(fp, " = ");
		translate(car(vals));
		fprintf(fp, ";\n");
		vars = cdr(vars);
		vals = cdr(vals);
	}
}
