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

#define DEBUG 0

#if DEBUG
#define dprintf printf
#else
#define dprintf(...)
#endif

#define null NULL
#define SIZE 33

FILE *fp;
Lexeme *global;

const char *CREATE_INTENT = "createIntent";
const char *CREATE_ENTITY = "createEntity";
const char *CREATE_DIALOG_NODE = "createDialogNode";

/*************************************/
/*     Private Method Prototypes     */
/*************************************/

static void addVariables();

static void addGuaranteed();
static void addWorkspaceId();
static void addIamApikey();
static void addVersion();
static void addUrl();
static void addWatson();
static void addAssistant();

static void addNotGuaranteed();

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
	addGuaranteed();
	addNotGuaranteed();
}

static void addGuaranteed() {
	addWorkspaceId();
	addIamApikey();
	addVersion();
	addUrl();
	addWatson();
	addAssistant();
}

static void addWorkspaceId() {
	Lexeme *workspace_id = newLexeme(ID, UNDEFINED, UNDEFINED, "workspace_id", UNDEFINED);
	Lexeme *wid = lookupLocal(global, workspace_id);
	fprintf(fp, "var workspace_id = \"%s\";\n", getSval(wid));
}

static void addIamApikey() {
	Lexeme *iam_apikey = newLexeme(ID, UNDEFINED, UNDEFINED, "iam_apikey", UNDEFINED);
	Lexeme *ia = lookupLocal(global, iam_apikey);
	fprintf(fp, "var iam_apikey = \"%s\";\n", getSval(ia));
}

static void addVersion() {
	Lexeme *version = newLexeme(ID, UNDEFINED, UNDEFINED, "version", UNDEFINED);
	Lexeme *v = lookupLocal(global, version);
	fprintf(fp, "var version = \"%s\";\n", getSval(v));
}

static void addUrl() {
	Lexeme *url = newLexeme(ID, UNDEFINED, UNDEFINED, "url", UNDEFINED);
	Lexeme *u = lookupLocal(global, url);
	fprintf(fp, "var url = \"%s\";\n", getSval(u));
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

static int isGuaranteed(char *str) {
	if (strcmp(str, "workspace_id") == 0 ||
		strcmp(str, "iam_apikey") == 0 ||
		strcmp(str, "version") == 0 ||
		strcmp(str, "url") == 0 ||
		strcmp(str, "watson") == 0 ||
		strcmp(str, "assistant") == 0)
			return 1;
	return 0;
}

static void addNotGuaranteed() {
	Lexeme *table = car(global);
	Lexeme *vars = car(table);
	Lexeme *vals = cdr(table);
	while (vars != null) {
		if (!isGuaranteed(getSval(car(vars)))) {
			fprintf(fp, "var %s = ", getSval(car(vars)));
			translate(car(vals));
			fprintf(fp, ";\n");
		}
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

static const char *getInsertName(char *str) {
	if (strcmp(str, "intents") == 0)
		return CREATE_INTENT;
	else if (strcmp(str, "entities") == 0)
		return CREATE_DIALOG_NODE;
	else if (strcmp(str, "dialog_nodes") == 0)
		return CREATE_DIALOG_NODE;
	else {
		fprintf(stderr, "Could not understand an insert of %s!\n", str);
		exit(6);
	}
}

static void addGenericInsert(char *str) {
	fprintf(fp, "for (var i = 0; i < %s.length; i++) {\n", str);
	fprintf(fp, "\tassistant.%s(%s[i], function (err, response) {\n", getInsertName(str), str);
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
	Lexeme *p = lookupLocal(env, dialog_node);

	Lexeme *workspace_id = newLexeme(ID, UNDEFINED, UNDEFINED, "workspace_id", UNDEFINED);
	Lexeme *wid = lookupLocal(global, workspace_id);
	if (!existsLocal(env, workspace_id))
		insert(env, workspace_id, wid);
	
	Lexeme *children = newLexeme(ID, UNDEFINED, UNDEFINED, "children", UNDEFINED);
	Lexeme *c = null;
	if (existsLocal(env, children))
		c = lookupLocal(env, children);
	
	Lexeme *previous_sibling = newLexeme(ID, UNDEFINED, UNDEFINED, "previous_sibling", UNDEFINED);
	Lexeme *ps = null;

	int len = 0;
	if (c != null)
		len = getAvalSize(c);
	
	for (int i = 0; i < len; i++) {
		Lexeme *curChild = getAval(c, i);
		if (p != null)
			insert(curChild, parent, p);
		if (i != 0 && !exists(curChild, previous_sibling))
			insert(curChild, previous_sibling, ps);
		enqueue(q, curChild);
		enqueueDialogNodesHelper(q, curChild);
		ps = lookupLocal(curChild, dialog_node);
	}

	if (c != null)
		delete(env, children);

	free(dialog_node);
	free(children);
}

static void enqueueDialogNodes(QUEUE *q) {
	Lexeme *dialog_nodes = newLexeme(ID, UNDEFINED, UNDEFINED, "dialog_nodes", UNDEFINED);
	Lexeme *dn = null;
	if (exists(global, dialog_nodes))
		dn = lookupLocal(global, dialog_nodes);

	Lexeme *previous_sibling = newLexeme(ID, UNDEFINED, UNDEFINED, "previous_sibling", UNDEFINED);
	Lexeme *ps = null;

	int len = 0;
	if (dn != null)
		len = getAvalSize(dn);

	for (int i = 0; i < len; i++) {
		Lexeme *curNode = getAval(dn, i);
		if (i != 0)
			insert(curNode, previous_sibling, ps);
		enqueue(q, curNode);
		enqueueDialogNodesHelper(q, curNode);
		ps = curNode;
	}

	free(dialog_nodes);
}
