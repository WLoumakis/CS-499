/**
 * Pretty Printer module created for printing
 * a parse tree into its respective code.
 * 
 * Created by Walt Loumakis
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Types.h"
#include "Lexeme.h"
#include "Parser.h"

#define null NULL

FILE *fp;

/**************************************************************************/
/*                       Private Method Prototypes                        */
/**************************************************************************/

static void pp(Lexeme *tree);

static void printWorkspaces(Lexeme *tree);
static void printWorkspaceObj(Lexeme *tree);
static void printWorkspaceAttr(Lexeme *tree);
static void printName(Lexeme *tree);
static void printLanguage(Lexeme *tree);
static void printCounterexamples(Lexeme *tree);
static void printCounterexample(Lexeme *tree);
static void printLearningOptOut(Lexeme *tree);
static void printSystemSettings(Lexeme *tree);
static void printTooling(Lexeme *tree);
static void printStoreGenericResponses(Lexeme *tree);
static void printDisambiguation(Lexeme *tree);
static void printDisambiguationAttr(Lexeme *tree);
static void printPrompt(Lexeme *tree);
static void printNoneOfTheAbovePrompt(Lexeme *tree);
static void printEnabled(Lexeme *tree);
static void printSensitivity(Lexeme *tree);
static void printHumanAgentAssist(Lexeme *tree);
static void printHumanAgentAssistAttr(Lexeme *tree);

static void printIntents(Lexeme *tree);
static void printIntent(Lexeme *tree);
static void printIntentAttr(Lexeme *tree);
static void printIntentName(Lexeme *tree);
static void printUserExamples(Lexeme *tree);
static void printExamples(Lexeme *tree);
static void printExampleAttr(Lexeme *tree);
static void printMentions(Lexeme *tree);
static void printMentionsAttr(Lexeme *tree);

static void printEntities(Lexeme *tree);
static void printEntity(Lexeme *tree);
static void printEntityAttr(Lexeme *tree);
static void printEntityName(Lexeme *tree);
static void printEntityVals(Lexeme *tree);
static void printEntityVal(Lexeme *tree);
static void printEntityValAttr(Lexeme *tree);
static void printEntityValName(Lexeme *tree);
static void printSynonyms(Lexeme *tree);
static void printPatterns(Lexeme *tree);
static void printEntityValType(Lexeme *tree);
static void printFuzzyMatch(Lexeme *tree);

static void printTree(Lexeme *tree);
static void printNodeList(Lexeme *tree);
static void printNodeAttr(Lexeme *tree);
static void printType(Lexeme *tree);
static void printTitle(Lexeme *tree);
static void printChildren(Lexeme *tree);
static void printOutputVals(Lexeme *tree);
static void printText(Lexeme *tree);
static void printTextValAttr(Lexeme *tree);
static void printStringList(Lexeme *tree);
static void printSelectionPolicy(Lexeme *tree);
static void printDelimeter(Lexeme *tree);
static void printPause(Lexeme *tree);
static void printPauseAttr(Lexeme *tree);
static void printTime(Lexeme *tree);
static void printTyping(Lexeme *tree);
static void printImageAttr(Lexeme *tree);
static void printSource(Lexeme *tree);
static void printOptionAttr(Lexeme *tree);
static void printPreference(Lexeme *tree);
static void printOptionsObj(Lexeme *tree);
static void printOptionsObjAttr(Lexeme *tree);
static void printLabel(Lexeme *tree);
static void printValue(Lexeme *tree);
static void printConnectToAgent(Lexeme *tree);
static void printMessageToHumanAgent(Lexeme *tree);
static void printMetadata(Lexeme *tree);
static void printMetadataAttr(Lexeme *tree);
static void printFallback(Lexeme *tree);
static void printConditions(Lexeme *tree);
static void printDialogNode(Lexeme *tree);
static void printPreviousSibling(Lexeme *tree);
static void printDescription(Lexeme *tree);
static void printParent(Lexeme *tree);
static void printContext(Lexeme *tree);
static void printContextObjAttr(Lexeme *tree);
static void printNextStep(Lexeme *tree);
static void printNextStepAttr(Lexeme *tree);
static void printBehavior(Lexeme *tree);
static void printSelector(Lexeme *tree);
static void printActions(Lexeme *tree);
static void printActionsAttr(Lexeme *tree);
static void printActionType(Lexeme *tree);
static void printParameters(Lexeme *tree);
static void printParametersAttr(Lexeme *tree);
static void printResultVariable(Lexeme *tree);
static void printCredentials(Lexeme *tree);
static void printEventName(Lexeme *tree);
static void printVariable(Lexeme *tree);
static void printDigressIn(Lexeme *tree);
static void printDigressOut(Lexeme *tree);
static void printDigressOutSlots(Lexeme *tree);
static void printUserLabel(Lexeme *tree);

/**************************************************************************/
/*                              Main Method                               */
/**************************************************************************/

int main(int argc, char *argv[]) {
	assert(argc == 3);

	initParser(argv[1]);
	Lexeme *tree = parse();
	closeParser();

	fp = fopen(argv[2], "w");
	pp(tree);
	fclose(fp);

	freeParseTree(tree);

	return 0;
}

/**************************************************************************/
/*                            Private Methods                             */
/**************************************************************************/

static int same(char *left, char *right) {
	return !strcmp(left, right);
}

static void pp(Lexeme *tree) {
	if (tree == null)
		return;
	char *type = getType(tree);
	if (same(type, TRUEVAL)) fprintf(fp, "%s", TRUEVAL);
	else if (same(type, FALSEVAL)) fprintf(fp, "%s", FALSEVAL);
	else if (same(type, STRING)) fprintf(fp, "%s", getSval(tree));
	else if (same(type, NUMBER)) fprintf(fp, "%d", getIval(tree));

	else if (same(type, WORKSPACES)) printWorkspaces(tree);
	else if (same(type, WORKSPACE_OBJ)) printWorkspaceObj(tree);
	else if (same(type, WORKSPACE_ATTR)) printWorkspaceAttr(tree);
	else if (same(type, NAME)) printName(tree);
	else if (same(type, LANGUAGE)) printLanguage(tree);
	else if (same(type, COUNTEREXAMPLES)) printCounterexamples(tree);
	else if (same(type, COUNTEREXAMPLE)) printCounterexample(tree);
	else if (same(type, LEARNING_OPT_OUT)) printLearningOptOut(tree);
	else if (same(type, SYSTEM_SETTINGS)) printSystemSettings(tree);
	else if (same(type, TOOLING)) printTooling(tree);
	else if (same(type, STORE_GENERIC_RESPONSES)) printStoreGenericResponses(tree);
	else if (same(type, DISAMBIGUATION)) printDisambiguation(tree);
	else if (same(type, DISAMBIGUATION_ATTR)) printDisambiguationAttr(tree);
	else if (same(type, PROMPT)) printPrompt(tree);
	else if (same(type, NONE_OF_THE_ABOVE_PROMPT)) printNoneOfTheAbovePrompt(tree);
	else if (same(type, ENABLED)) printEnabled(tree);
	else if (same(type, SENSITIVITY)) printSensitivity(tree);
	else if (same(type, HUMAN_AGENT_ASSIST)) printHumanAgentAssist(tree);
	else if (same(type, HUMAN_AGENT_ASSIST_ATTR)) printHumanAgentAssistAttr(tree);

	else if (same(type, INTENTS)) printIntents(tree);
	else if (same(type, INTENT)) printIntent(tree);
	else if (same(type, INTENT_ATTR)) printIntentAttr(tree);
	else if (same(type, INTENT_NAME)) printIntentName(tree);
	else if (same(type, USER_EXAMPLES)) printUserExamples(tree);
	else if (same(type, EXAMPLES)) printExamples(tree);
	else if (same(type, EXAMPLE_ATTR)) printExampleAttr(tree);
	else if (same(type, MENTIONS)) printMentions(tree);
	else if (same(type, MENTIONS_ATTR)) printMentionsAttr(tree);

	else if (same(type, ENTITIES)) printEntities(tree);
	else if (same(type, ENTITY)) printEntity(tree);
	else if (same(type, ENTITY_ATTR)) printEntityAttr(tree);
	else if (same(type, ENTITY_NAME)) printEntityName(tree);
	else if (same(type, ENTITY_VALS)) printEntityVals(tree);
	else if (same(type, ENTITY_VAL)) printEntityVal(tree);
	else if (same(type, ENTITY_VAL_ATTR)) printEntityValAttr(tree);
	else if (same(type, ENTITY_VAL_NAME)) printEntityValName(tree);
	else if (same(type, SYNONYMS)) printSynonyms(tree);
	else if (same(type, PATTERNS)) printPatterns(tree);
	else if (same(type, ENTITY_VAL_TYPE)) printEntityValType(tree);
	else if (same(type, FUZZY_MATCH)) printFuzzyMatch(tree);

	else if (same(type, TREE)) printTree(tree);
	else if (same(type, NODE_LIST)) printNodeList(tree);
	else if (same(type, NODE_ATTR)) printNodeAttr(tree);
	else if (same(type, TYPE)) printType(tree);
	else if (same(type, TITLE)) printTitle(tree);
	else if (same(type, CHILDREN)) printChildren(tree);
	else if (same(type, OUTPUT_VALS)) printOutputVals(tree);
	else if (same(type, TEXT)) printText(tree);
	else if (same(type, TEXT_VAL_ATTR)) printTextValAttr(tree);
	else if (same(type, STRING_LIST)) printStringList(tree);
	else if (same(type, SELECTION_POLICY)) printSelectionPolicy(tree);
	else if (same(type, DELIMETER)) printDelimeter(tree);
	else if (same(type, PAUSE)) printPause(tree);
	else if (same(type, PAUSE_ATTR)) printPauseAttr(tree);
	else if (same(type, TIME)) printTime(tree);
	else if (same(type, TYPING)) printTyping(tree);
	else if (same(type, IMAGE_ATTR)) printImageAttr(tree);
	else if (same(type, SOURCE)) printSource(tree);
	else if (same(type, OPTION_ATTR)) printOptionAttr(tree);
	else if (same(type, PREFERENCE)) printPreference(tree);
	else if (same(type, OPTIONS_OBJS)) printOptionsObj(tree);
	else if (same(type, OPTIONS_OBJ_ATTR)) printOptionsObjAttr(tree);
	else if (same(type, LABEL)) printLabel(tree);
	else if (same(type, VALUE)) printValue(tree);
	else if (same(type, CONNECT_TO_AGENT)) printConnectToAgent(tree);
	else if (same(type, MESSAGE_TO_HUMAN_AGENT)) printMessageToHumanAgent(tree);
	else if (same(type, METADATA)) printMetadata(tree);
	else if (same(type, METADATA_ATTR)) printMetadataAttr(tree);
	else if (same(type, FALLBACK)) printFallback(tree);
	else if (same(type, CONDITIONS)) printConditions(tree);
	else if (same(type, DIALOG_NODE)) printDialogNode(tree);
	else if (same(type, PREVIOUS_SIBLING)) printPreviousSibling(tree);
	else if (same(type, DESCRIPTION)) printDescription(tree);
	else if (same(type, PARENT)) printParent(tree);
	else if (same(type, CONTEXT)) printContext(tree);
	else if (same(type, CONTEXT_OBJ_ATTR)) printContextObjAttr(tree);
	else if (same(type, NEXT_STEP)) printNextStep(tree);
	else if (same(type, NEXT_STEP_ATTR)) printNextStepAttr(tree);
	else if (same(type, BEHAVIOR)) printBehavior(tree);
	else if (same(type, SELECTOR)) printSelector(tree);
	else if (same(type, ACTIONS)) printActions(tree);
	else if (same(type, ACTIONS_ATTR)) printActionsAttr(tree);
	else if (same(type, ACTION_TYPE)) printActionType(tree);
	else if (same(type, PARAMETERS)) printParameters(tree);
	else if (same(type, PARAMETERS_ATTR)) printParametersAttr(tree);
	else if (same(type, RESULT_VARIABLE)) printResultVariable(tree);
	else if (same(type, CREDENTIALS)) printCredentials(tree);
	else if (same(type, EVENT_NAME)) printEventName(tree);
	else if (same(type, VARIABLE)) printVariable(tree);
	else if (same(type, DIGRESS_IN)) printDigressIn(tree);
	else if (same(type, DIGRESS_OUT)) printDigressOut(tree);
	else if (same(type, DIGRESS_OUT_SLOTS)) printDigressOutSlots(tree);
	else printUserLabel(tree);
}

/*************************************/
/*            Workspaces             */
/*************************************/

static void printWorkspaces(Lexeme *tree) {
	fprintf(fp, "workspaces : [\n");
	pp(car(tree));
	fprintf(fp, "]\n");
}

static void printWorkspaceObj(Lexeme *tree) {
	while (tree != null) {
		fprintf(fp, "{\n");
		pp(car(tree));
		fprintf(fp, "}");
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printWorkspaceAttr(Lexeme *tree) {
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printName(Lexeme *tree) {
	fprintf(fp, "name : ");
	pp(car(tree));
}

static void printLanguage(Lexeme *tree) {
	fprintf(fp, "language : ");
	pp(car(tree));
}

static void printCounterexamples(Lexeme *tree) {
	fprintf(fp, "counterexamples : [\n");
	pp(car(tree));
	fprintf(fp, "]");
}

static void printCounterexample(Lexeme *tree) {
	while (tree != null) {
		fprintf(fp, "text : ");
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printLearningOptOut(Lexeme *tree) {
	fprintf(fp, "learning_opt_out : ");
	pp(car(tree));
}

static void printSystemSettings(Lexeme *tree) {
	fprintf(fp, "system_settings : {\n");
	pp(car(tree));
	fprintf(fp, "}");
}

static void printTooling(Lexeme *tree) {
	fprintf(fp, "tooling : {\n");
	pp(car(tree));
	fprintf(fp, "}");
}

static void printStoreGenericResponses(Lexeme *tree) {
	fprintf(fp, "store_generic_responses : ");
	pp(car(tree));
	fprintf(fp, "\n");
}

static void printDisambiguation(Lexeme *tree) {
	fprintf(fp, "disambiguation : {\n");
	pp(car(tree));
	fprintf(fp, "}");
}

static void printDisambiguationAttr(Lexeme *tree) {
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printPrompt(Lexeme *tree) {
	fprintf(fp, "prompt : ");
	pp(car(tree));
}

static void printNoneOfTheAbovePrompt(Lexeme *tree) {
	fprintf(fp, "none_of_the_above_prompt : ");
	pp(car(tree));
}

static void printEnabled(Lexeme *tree) {
	fprintf(fp, "enabled : ");
	pp(car(tree));
}

static void printSensitivity(Lexeme *tree) {
	fprintf(fp, "sensitivity : ");
	pp(car(tree));
}

static void printHumanAgentAssist(Lexeme *tree) {
	fprintf(fp, "human_agent_assist : {\n");
	pp(car(tree));
	fprintf(fp, "}");
}


//TODO: Figure out a more comprehensive list of humanAgentAssist attributes
static void printHumanAgentAssistAttr(Lexeme *tree) {
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

/*************************************/
/*              Intents              */
/*************************************/

static void printIntents(Lexeme *tree) {
	fprintf(fp, "intents : [\n");
	pp(car(tree));
	fprintf(fp, "]");
}

static void printIntent(Lexeme *tree) {
	while (tree != null) {
		fprintf(fp, "{\n");
		pp(car(tree));
		fprintf(fp, "}");
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printIntentAttr(Lexeme *tree) {
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printIntentName(Lexeme *tree) {
	fprintf(fp, "intent : ");
	pp(car(tree));
}

static void printUserExamples(Lexeme *tree) {
	fprintf(fp, "examples : [\n");
	pp(car(tree));
	fprintf(fp, "]");
}

static void printExamples(Lexeme *tree) {
	while (tree != null) {
		fprintf(fp, "{\n");
		pp(car(tree));
		fprintf(fp, "}");
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printExampleAttr(Lexeme *tree) {
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printMentions(Lexeme *tree) {
	fprintf(fp, "mentions : [\n");
	pp(car(tree));
	fprintf(fp, "]");
}

static void printMentionsAttr(Lexeme *tree) {
	while (tree != null) {
		fprintf(fp, "{\n");
		pp(car(tree));
		fprintf(fp, "}");
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

/*************************************/
/*             Entities              */
/*************************************/

static void printEntities(Lexeme *tree) {
	fprintf(fp, "entities : [\n");
	pp(car(tree));
	fprintf(fp, "]");
}

static void printEntity(Lexeme *tree) {
	while (tree != null) {
		fprintf(fp, "{");
		pp(car(tree));
		fprintf(fp, "}");
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printEntityAttr(Lexeme *tree) {
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printEntityName(Lexeme *tree) {
	fprintf(fp, "entity: ");
	pp(car(tree));
}

static void printEntityVals(Lexeme *tree) {
	fprintf(fp, "values : [\n");
	pp(car(tree));
	fprintf(fp, "]");
}

static void printEntityVal(Lexeme *tree) {
	while (tree != null) {
		fprintf(fp, "{\n");
		pp(car(tree));
		fprintf(fp, "}");
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printEntityValAttr(Lexeme *tree) {
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printEntityValName(Lexeme *tree) {
	fprintf(fp, "value : ");
	pp(car(tree));
}

//NOTE: I made changes in the parser prior to here by adding another
//		node above the arrays to indicate they were arrays/lists.
//		I will no longer make that edit because I realized it was
//		unnecessary to the functionality of the language.
//TODO: Go back and fix this ^ defect
static void printSynonyms(Lexeme *tree) {
	fprintf(fp, "synonyms : [\n");
	while (tree != null) {
		fprintf(fp, "synonym : ");
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "]");
}

static void printPatterns(Lexeme *tree) {
	fprintf(fp, "patterns : [\n");
	while (tree != null) {
		fprintf(fp, "pattern : ");
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "]");
}

static void printEntityValType(Lexeme *tree) {
	fprintf(fp, "value_type : ");
	pp(car(tree));
}

static void printFuzzyMatch(Lexeme *tree) {
	fprintf(fp, "fuzzy_match : ");
	pp(car(tree));
}

/*************************************/
/*           Decision Tree           */
/*************************************/

static void printTree(Lexeme *tree) {
	fprintf(fp, "tree : [\n");
	pp(car(tree));
	fprintf(fp, "]");
}

static void printNodeList(Lexeme *tree) {
	while (tree != null) {
		fprintf(fp, "{\n");
		pp(car(tree));
		fprintf(fp, "}");
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printNodeAttr(Lexeme *tree) {
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printType(Lexeme *tree) {
	fprintf(fp, "type : ");
	pp(car(tree));
}

static void printTitle(Lexeme *tree) {
	fprintf(fp, "title : ");
	pp(car(tree));
}

static void printChildren(Lexeme *tree) {
	fprintf(fp, "children : [\n");
	pp(car(tree));
	fprintf(fp, "]");
}

static void printOutputVals(Lexeme *tree) {
	fprintf(fp, "output : {\n");
	pp(car(tree));
	fprintf(fp, "}");
}

static void printText(Lexeme *tree) {
	fprintf(fp, "text : ");
	pp(car(tree));
}

static void printTextValAttr(Lexeme *tree) {
	fprintf(fp, "{\n");
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "}");
}

static void printStringList(Lexeme *tree) {
	fprintf(fp, "values : [\n");
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "]");
}

static void printSelectionPolicy(Lexeme *tree) {
	fprintf(fp, "selection_policy : ");
	pp(car(tree));
}

static void printDelimeter(Lexeme *tree) {
	fprintf(fp, "delimeter : ");
	pp(car(tree));
}

static void printPause(Lexeme *tree) {
	fprintf(fp, "pause : {\n");
	pp(car(tree));
	fprintf(fp, "}");
}

static void printPauseAttr(Lexeme *tree) {
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printTime(Lexeme *tree) {
	fprintf(fp, "time : ");
	pp(car(tree));
}

static void printTyping(Lexeme *tree) {
	fprintf(fp, "typing : ");
	pp(car(tree));
}

static void printImageAttr(Lexeme *tree) {
	fprintf(fp, "image : {\n");
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "}");
}

static void printSource(Lexeme *tree) {
	fprintf(fp, "source : ");
	pp(car(tree));
}

static void printOptionAttr(Lexeme *tree) {
	fprintf(fp, "opton : {\n");
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "}");
}

static void printPreference(Lexeme *tree) {
	fprintf(fp, "preference : ");
	pp(car(tree));
}

static void printOptionsObj(Lexeme *tree) {
	fprintf(fp, "options : [\n");
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "]");
}

static void printOptionsObjAttr(Lexeme *tree) {
	fprintf(fp, "{\n");
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "}");
}

static void printLabel(Lexeme *tree) {
	fprintf(fp, "label : ");
	pp(car(tree));
}

static void printValue(Lexeme *tree) {
	fprintf(fp, "value : {\n");
	pp(car(tree));
	fprintf(fp, "}\n");
}

static void printConnectToAgent(Lexeme *tree) {
	fprintf(fp, "connect_to_agent : {\n");
	pp(car(tree));
	fprintf(fp, "}");
}

static void printMessageToHumanAgent(Lexeme *tree) {
	fprintf(fp, "message_to_human_agent : ");
	pp(car(tree));
	fprintf(fp, "\n");
}

static void printMetadata(Lexeme *tree) {
	fprintf(fp, "metadata : {\n");
	pp(car(tree));
	fprintf(fp, "}");
}

static void printMetadataAttr(Lexeme *tree) {
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
}

static void printFallback(Lexeme *tree) {
	fprintf(fp, "fallback : ");
	pp(car(tree));
}

static void printConditions(Lexeme *tree) {
	fprintf(fp, "conditions : ");
	pp(car(tree));
}

static void printDialogNode(Lexeme *tree) {
	fprintf(fp, "dialog_node : ");
	pp(car(tree));
}

static void printPreviousSibling(Lexeme *tree) {
	fprintf(fp, "previous_sibling : ");
	pp(car(tree));
}

static void printDescription(Lexeme *tree) {
	fprintf(fp, "description : ");
	pp(car(tree));
}

static void printParent(Lexeme *tree) {
	fprintf(fp, "parent : ");
	pp(car(tree));
}

static void printContext(Lexeme *tree) {
	fprintf(fp, "context : ");
	pp(car(tree));
}

static void printContextObjAttr(Lexeme *tree) {
	fprintf(fp, "{\n");
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "}");
}

static void printNextStep(Lexeme *tree) {
	fprintf(fp, "next_step : ");
	pp(car(tree));
}

static void printNextStepAttr(Lexeme *tree) {
	fprintf(fp, "{\n");
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "}");
}

static void printBehavior(Lexeme *tree) {
	fprintf(fp, "behavior : ");
	pp(car(tree));
}

static void printSelector(Lexeme *tree) {
	fprintf(fp, "selector : ");
	pp(car(tree));
}

static void printActions(Lexeme *tree) {
	fprintf(fp, "actions : [\n");
	pp(car(tree));
	fprintf(fp, "]");
}

static void printActionsAttr(Lexeme *tree) {
	fprintf(fp, "{\n");
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "}");
}

static void printActionType(Lexeme *tree) {
	fprintf(fp, "action_type : ");
	pp(car(tree));
}

static void printParameters(Lexeme *tree) {
	fprintf(fp, "parameters : ");
	pp(car(tree));
}

//TODO: Figure out a more comprehensive list of parameters attributes.
static void printParametersAttr(Lexeme *tree) {
	fprintf(fp, "{\n");
	while (tree != null) {
		pp(car(tree));
		tree = cdr(tree);
		if (tree != null)
			fprintf(fp, ",");
		fprintf(fp, "\n");
	}
	fprintf(fp, "}");
}

static void printResultVariable(Lexeme *tree) {
	fprintf(fp, "result_variable : ");
	pp(car(tree));
}

static void printCredentials(Lexeme *tree) {
	fprintf(fp, "credentials : ");
	pp(car(tree));
}

static void printEventName(Lexeme *tree) {
	fprintf(fp, "event_name : ");
	pp(car(tree));
}

static void printVariable(Lexeme *tree) {
	fprintf(fp, "variable : ");
	pp(car(tree));
}

static void printDigressIn(Lexeme *tree) {
	fprintf(fp, "digress_in : ");
	pp(car(tree));
}

static void printDigressOut(Lexeme *tree) {
	fprintf(fp, "digress_out : ");
	pp(car(tree));
}

static void printDigressOutSlots(Lexeme *tree) {
	fprintf(fp, "digress_out_slots : ");
	pp(car(tree));
}

static void printUserLabel(Lexeme *tree) {
	fprintf(fp, "user_label : ");
	pp(car(tree));
}

