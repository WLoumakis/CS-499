/**
 * Main module for translation and
 * evaluation of WaltsonJS code.
 * 
 * Created by Walt Loumakis.
 */

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "sll.h"
#include "queue.h"
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
Lexeme *global;

/*************************************/
/*     Private Method Prototypes     */
/*************************************/

static void addVariables();
static void addWatson();
static void addAssistant();
static void addGlobalVariables();

static void addInsertCode();
static void addIntents();
static void addEntities();
static void addDialogNodes();
static void addGenericInsert(char *str);

static void enqueueDialogNodesHelper(QUEUE *q, Lexeme *env);
static void enqueueDialogNodes(QUEUE *q);

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

	char *file = strtok(argv[1], ".");
	char *buffer = (char *)malloc(sizeof(char) * strlen(file) + 4);
	buffer = strcpy(buffer, file);
	buffer = strcat(buffer, ".js\0");

	fp = initTranslator(buffer);
	global = create();

	eval(tree, global);

	QUEUE *q = newQUEUE(null, null);

	enqueueDialogNodes(q);

	Lexeme *dialog_nodes = newLexeme(ID, UNDEFINED, UNDEFINED, "dialog_nodes", UNDEFINED);
	Lexeme *dn = newLexeme(ARRAY, UNDEFINED, UNDEFINED, UNDEFINED, UNDEFINED);
	newArray(dn, sizeQUEUE(q));

	int i = 0;
	while (sizeQUEUE(q) != 0)
		setAval(dn, i++, dequeue(q));

	Lexeme *oldDn = update(global, dialog_nodes, dn);

	addVariables();
	addInsertCode();

	closeTranslator();
	free(buffer);
	free(dialog_nodes);
	//TODO: Add more frees to complete memory management.
	return 0;
}

/*************************************/
/*          Private Methods          */
/*************************************/

static void addVariables() {
	addWatson();
	addAssistant();
	addGlobalVariables();
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

static void addGlobalVariables() {
	Lexeme *table = car(global);
	Lexeme *vars = car(table);
	Lexeme *vals = cdr(table);
	while(vars != null) {
		fprintf(fp, "var %s = ", getSval(car(vars)));
		translate(car(vals));
		fprintf(fp, ";\n");
		vars = cdr(vars);
		vals = cdr(vals);
	}
}

static void addInsertCode() {
	Lexeme *intents = newLexeme(ID, UNDEFINED, UNDEFINED, "intents", UNDEFINED);
	Lexeme *entities = newLexeme(ID, UNDEFINED, UNDEFINED, "entities", UNDEFINED);
	Lexeme *dialog_nodes = newLexeme(ID, UNDEFINED, UNDEFINED, "dialog_nodes", UNDEFINED);
	if (exists(global, intents))
		addIntents();
	if (exists(global, entities))
		addEntities();
	if (exists(global, dialog_nodes))
		addDialogNodes();
	free(intents);
	free(entities);
	free(dialog_nodes);
}

static void addIntents() {
	addGenericInsert("intents");
}

static void addEntities() {
	addGenericInsert("entities");
}

static void addDialogNodes() {
	addGenericInsert("dialog_nodes");
}

static void addGenericInsert(char *str) {
	fprintf(fp, "for (var i = 0; i < %s.length; i++) {\n", str);
	fprintf(fp, "\tassistant.createIntent(%s[i], function (err, response) {\n", str);
	fprintf(fp, "\t\tif (err)\n");
	fprintf(fp, "\t\t\tconsole.error(err);\n");
	fprintf(fp, "\t\telse\n");
	fprintf(fp, "\t\t\tconsole.log(JSON.stringify(response, null, 2));\n");
	fprintf(fp, "\t});\n");
	fprintf(fp, "}");
}

static void enqueueDialogNodesHelper(QUEUE *q, Lexeme *env) {
	Lexeme *parent = newLexeme(ID, UNDEFINED, UNDEFINED, "parent", UNDEFINED);
	Lexeme *dialog_node = newLexeme(ID, UNDEFINED, UNDEFINED, "dialog_node", UNDEFINED);
	Lexeme *p = null;
	if (exists(env, dialog_node))
		p = lookup(env, dialog_node);
	Lexeme *workspace_id = newLexeme(ID, UNDEFINED, UNDEFINED, "workspace_id", UNDEFINED);
	Lexeme *wid = lookup(global, workspace_id);
	Lexeme *children = newLexeme(ID, UNDEFINED, UNDEFINED, "children", UNDEFINED);
	Lexeme *c = null;
	if (exists(env, children))
		c = lookup(env, children);
	Lexeme *previous_sibling = newLexeme(ID, UNDEFINED, UNDEFINED, "previous_sibling", UNDEFINED);
	Lexeme *ps = null;
	int len = 0;
	if (exists(env, children))
		len = getAvalSize(c);
	for (int i = 0; i < len; i++) {
		Lexeme *curChild = getAval(c, i);
		if (!exists(curChild, workspace_id))
			insert(curChild, workspace_id, wid);
		if (p != null)
			insert(curChild, parent, p);
		if (i != 0)
			insert(curChild, previous_sibling, ps);
		enqueue(q, curChild);
		enqueueDialogNodesHelper(q, curChild);
		ps = curChild;
	}
	free(parent);
	free(dialog_node);
	free(workspace_id);
	free(children);
	free(previous_sibling);
}

static void enqueueDialogNodes(QUEUE *q) {
	Lexeme *dialog_nodes = newLexeme(ID, UNDEFINED, UNDEFINED, "dialog_nodes", UNDEFINED);
	Lexeme *dn = lookup(global, dialog_nodes);
	Lexeme *previous_sibling = newLexeme(ID, UNDEFINED, UNDEFINED, "previous_sibling", UNDEFINED);
	Lexeme *ps = null;
	int len = getAvalSize(dn);
	for (int i = 0; i < len; i++) {
		Lexeme *curNode = getAval(dn, i);
		if (i != 0)
			insert(curNode, previous_sibling, ps);
		enqueue(q, curNode);
		enqueueDialogNodesHelper(q, curNode);
		ps = curNode;
	}
	free(dialog_nodes);
	free(previous_sibling);
}
